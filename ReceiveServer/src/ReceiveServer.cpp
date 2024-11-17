#include "ReceiveServer.h"

#include <crow.h>

#include <iostream>

void ReceiveServer::start() {
    crow::SimpleApp app;

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
        .methods("POST"_method)([this](const crow::request& req) {
            try {
                crow::json::rvalue json_data = crow::json::load(req.body);
                if (json_data) {
#ifdef DEBUG
                    std::cout << "Received data: " << json_data << std::endl;
#endif  // DEBUG
                    return crow::response(200, "Data received successfully.");
                } else {
#ifdef DEBUG
                    std::cout << "Invalid JSON data" << std::endl;
#endif  // DEBUG
                    return crow::response(400, "Invalid JSON format.");
                }
            } catch (const std::exception& e) {
#ifdef DEBUG
                std::cout << "Error parsing JSON: " << e.what() << std::endl;
#endif  // DEBUG
                return crow::response(400, "Invalid JSON format.");
            }
        });

    std::cout << "ReceiveServer server started on :9000" << std::endl;
    app.port(9000).multithreaded().run();
}
