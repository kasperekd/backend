#include "ReceiveServer.h"

#include <crow.h>

#include <iostream>

#include "PostgresDB.h"

void ReceiveServer::start() {
    crow::SimpleApp app;

    PostgresDB db(
        "dbname=signal_db user=user password=password host=localhost "
        "port=5432");
    db.connect();

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
        auto data = crow::json::load(rawJson);

        if (!data || data.error()) {
            throw std::runtime_error("Invalid JSON format. Expected an array.");
        }

        for (const auto& entry : data) {
            if (!entry.has("type") || !entry.has("coordinates") ||
                !entry.has("timestamp") || !entry.has("cellId") ||
                !entry.has("signalStrength") ||
                !entry.has("trackingAreaCode") || !entry.has("operator") ||
                !entry.has("RSRP") || !entry.has("RSRQ")) {
                throw std::runtime_error(
                    "Missing required fields in JSON entry.");
            }

            // Парсинг координат
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

            db.executeQuery(
                query,
                {entry["type"].s(), std::to_string(latitude),
                 std::to_string(longitude),
                 std::to_string(entry["timestamp"].i()),
                 std::to_string(entry["cellId"].i()),
                 std::to_string(entry["signalStrength"].i()),
                 std::to_string(entry["trackingAreaCode"].i()),
                 entry["operator"].s(), std::to_string(entry["RSRP"].i()),
                 std::to_string(entry["RSRQ"].i())});
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(
            std::string("Error parsing and saving data: ") + e.what());
    }
}