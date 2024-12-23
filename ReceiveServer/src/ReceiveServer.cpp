#include "ReceiveServer.h"

#include <crow.h>

#include <iostream>
#include <type_traits>

#include "PostgresDB.h"

void tryReconnect(PostgresDB& db, int wait_seconds) {
    while (true) {
        try {
            db.connect();
            std::cout << "Successfully connected to the database." << std::endl;
            return;
        } catch (const std::exception& e) {
            std::cerr << "Error connecting to the database: " << e.what()
                      << std::endl;
            std::cout << "Retrying in " << wait_seconds << " seconds..."
                      << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
        }
    }
}
void ReceiveServer::start() {
    crow::SimpleApp app;

    PostgresDB db(
        "dbname=signal_db user=user password=password host=db port=5432");

    tryReconnect(db, 5);

    // GET HEALTH
    app.route_dynamic("/api/health")
        .methods("GET"_method)([](const crow::request& req) {
#ifdef DEBUG
            std::cout << "Health check request received" << std::endl;
#endif  // DEBUG
            return crow::response(200, "Server is up and running");
        });

    // POST cellinfo
    app.route_dynamic("/api/cellinfo")
        .methods("POST"_method)([this, &db](const crow::request& req) {
            try {
                parseAndSaveData(req.body, db);

#ifdef DEBUG
                std::cout << "Data successfully parsed and saved." << std::endl;
#endif
                return crow::response(200,
                                      "Data successfully saved to database.");
            } catch (const std::exception& e) {
#ifdef DEBUG
                std::cerr << "Error: " << e.what() << std::endl;
#endif
                return crow::response(400, std::string("Error: ") + e.what());
            }
        });

    std::cout << "ReceiveServer server started on :9000" << std::endl;
    app.port(9000).multithreaded().run();
}

void ReceiveServer::parseAndSaveData(const std::string& rawJson,
                                     PostgresDB& db) {
    try {
        std::cout << rawJson << std::endl;
        auto data = crow::json::load(rawJson);

        if (!data || data.error()) {
            throw std::runtime_error("Invalid JSON format. Expected an array.");
        }

        for (const auto& entry : data) {
            if (!entry.has("type") || !entry.has("coordinates") ||
                !entry.has("timestamp") || !entry.has("cellId") ||
                !entry.has("signalStrength") || !entry.has("operator")) {
                throw std::runtime_error(
                    "Missing required fields in JSON entry.");
            }

            std::string trackingAreaCode =
                entry.has("trackingAreaCode")
                    ? std::to_string(entry["trackingAreaCode"].i())
                    : "NULL";
            std::string rsrp =
                entry.has("RSRP") ? std::to_string(entry["RSRP"].i()) : "NULL";
            std::string rsrq =
                entry.has("RSRQ") ? std::to_string(entry["RSRQ"].i()) : "NULL";

            std::string coordinates = entry["coordinates"].s();
            size_t commaPos = coordinates.find(',');
            if (commaPos == std::string::npos) {
                throw std::runtime_error("Invalid coordinates format.");
            }
            double latitude = std::stod(coordinates.substr(0, commaPos));
            double longitude = std::stod(coordinates.substr(commaPos + 1));

            // SQL-запрос
            std::string query =
                "INSERT INTO cell_data (type, latitude, longitude, timestamp, "
                "cell_id, signal_strength, tracking_area_code, operator, rsrp, "
                "rsrq) "
                "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)";

            std::vector<std::string> params = {
                entry["type"].s(),
                std::to_string(latitude),
                std::to_string(longitude),
                std::to_string(entry["timestamp"].i()),
                std::to_string(entry["cellId"].i()),
                std::to_string(entry["signalStrength"].i()),
                trackingAreaCode,
                entry["operator"].s(),
                rsrp,
                rsrq};

            db.executeQuery(query, params);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(
            std::string("Error parsing and saving data: ") + e.what());
    }
}
