#include "WebServer.h"

#include <crow.h>

#include <fstream>
#include <sstream>

void WebServer::start() {
    crow::SimpleApp app;

    // Главная страница
    CROW_ROUTE(app, "/")
    ([this]() { return crow::response(readFile("web/index.html")); });

    // Эндпоинт для JSON данных
    CROW_ROUTE(app, "/api/data")
    ([]() {
        crow::json::wvalue data;
        data["message"] = "Hello from the Web Server!";
        return data;
    });

    // статические файлы
    CROW_ROUTE(app, "/app.js")
    ([this]() { return crow::response(readFile("web/app.js")); });

    CROW_ROUTE(app, "/styles.css")
    ([this]() { return crow::response(readFile("web/styles.css")); });

    std::cout << "Web server with REST API started on http://localhost:8080"
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
