#include <iostream>

#include "ReceiveServer.h"

int main() {
    std::cout << "Starting Receive Server..." << std::endl;
    ReceiveServer rServer;
    rServer.start();
    return 0;
}
