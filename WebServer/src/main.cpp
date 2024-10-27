#include <iostream>

#include "WebServer.h"

int main() {
    std::cout << "Starting Web Server..." << std::endl;
    WebServer server;
    server.start();
    return 0;
}
