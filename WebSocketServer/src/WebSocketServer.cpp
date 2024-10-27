#include "WebSocketServer.h"

#include <unistd.h>

#include <iostream>

void WebSocketServer::start() {
    std::cout << "WebSocket server started on ws://localhost:9000 (mocked)"
              << std::endl;
    sleep(10);
    std::cout << "exit..." << std::endl;
}
