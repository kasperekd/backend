#include "WebServer.h"

#include <crow.h>
// #include <crow/json.h>

#include <fstream>
#include <sstream>

std::mutex cache_mutex;

void WebServer::start() {
    crow::SimpleApp app;

    // Главная страница
    CROW_ROUTE(app, "/")
    ([this]() { return crow::response(readFile("web/index.html")); });

    // Эндпоинт для получения координат базовой станции по Cell ID
    CROW_ROUTE(app, "/api/base-stations/<int>")
    ([this](long long cell_id) {
        // Получить данные из кэша или базы данных
        return crow::response(getBaseStationCoordinatesJson(cell_id));
    });

    // Эндпоинт для получения ВСЕХ координат базовой станции
    CROW_ROUTE(app, "/api/base-stations/multiple")
    ([this]() { return crow::response(getAllBaseStationCoordinatesJson()); });

    // Эндпоинты для статических файлов
    CROW_ROUTE(app, "/app.js")
    ([this]() { return crow::response(readFile("web/app.js")); });

    CROW_ROUTE(app, "/styles.css")
    ([this]() { return crow::response(readFile("web/styles.css")); });

    std::cout << "Web server with REST API started on http://:8080"
              << std::endl;

    app.port(8080).multithreaded().run();
}

std::string WebServer::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::optional<BaseStationData> WebServer::getFromCache(long long cell_id) {
    std::lock_guard<std::mutex> lock(cache_mutex);
    auto it = cache.find(cell_id);

    if (it != cache.end()) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(
                now - it->second.timestamp)
                .count() < cache_validity_duration_seconds) {
            return it->second;
        } else {
            cache.erase(it);  // Устарело
        }
    }

    return std::nullopt;
}

void WebServer::addToCache(long long cell_id, double lat, double lon) {
    std::lock_guard<std::mutex> lock(cache_mutex);
    cache[cell_id] = {lat, lon, std::chrono::steady_clock::now()};
}

crow::json::wvalue WebServer::getBaseStationCoordinatesJson(long long cell_id) {
    auto cached_data = getFromCache(cell_id);
    if (cached_data) {
        crow::json::wvalue result;
        result["latitude"] = cached_data->latitude;
        result["longitude"] = cached_data->longitude;
        result["cell_id"] = cell_id;
        return result;
    }

    try {
        pqxx::connection conn(
            "dbname=signal_db user=user password=password host=db port=5432");

        if (!conn.is_open()) {
            throw std::runtime_error("Failed to connect to database");
        }

        pqxx::work txn(conn);

        std::string query_str =
            "SELECT calculated_latitude, calculated_longitude FROM "
            "base_station_coordinates WHERE cell_id = " +
            std::to_string(cell_id);
        // std::cout << "Executing query: " << query_str << std::endl;

        auto query = txn.exec(query_str);

        if (!query.empty()) {
            double latitude = query[0][0].as<double>();
            double longitude = query[0][1].as<double>();

            // std::cout << "Latitude: " << latitude
            //   << ", Longitude: " << longitude << std::endl;

            addToCache(cell_id, latitude, longitude);

            crow::json::wvalue result;
            result["latitude"] = latitude;
            result["longitude"] = longitude;
            result["cell_id"] = cell_id;
            return result;
        } else {
            throw std::runtime_error("No data found for cell_id: " +
                                     std::to_string(cell_id));
        }
    } catch (const std::exception& e) {
        crow::json::wvalue response;
        response["error"] = "Database error: " + std::string(e.what());
        std::cerr << e.what() << std::endl;
        return response;
    }
}

crow::json::wvalue WebServer::getAllBaseStationCoordinatesJson() {
    try {
        pqxx::connection conn(
            "dbname=signal_db user=user password=password host=db port=5432");

        if (!conn.is_open()) {
            throw std::runtime_error("Failed to connect to database");
        }

        pqxx::work txn(conn);

        std::string query_str =
            "SELECT cell_id, calculated_latitude, calculated_longitude FROM "
            "base_station_coordinates";
        // std::cout << "Executing query: " << query_str << std::endl;

        auto query = txn.exec(query_str);

        std::vector<crow::json::wvalue> vector_of_wvalue;
        for (const auto& row : query) {
            crow::json::wvalue base_station;
            base_station["cell_id"] = row[0].as<long long>();
            base_station["latitude"] = row[1].as<double>();
            base_station["longitude"] = row[2].as<double>();

            vector_of_wvalue.push_back(std::move(base_station));
        }

        crow::json::wvalue final = std::move(vector_of_wvalue);

        std::cout << "Result: " << final.estimate_length() << std::endl;
        return final;

    } catch (const std::exception& e) {
        crow::json::wvalue response;
        response["error"] = "Database error: " + std::string(e.what());
        std::cerr << e.what() << std::endl;
        return response;
    }
}