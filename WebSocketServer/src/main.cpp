#include <iostream>

#include "WebSocketServer.h"

int main() {
    std::cout << "Starting WebSocket Server..." << std::endl;
    WebSocketServer wsServer;
    wsServer.start();
    return 0;
}
