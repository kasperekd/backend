#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <string>

class WebSocketServer {
   public:
    WebSocketServer() = default;
    ~WebSocketServer() = default;

    void start();

    void onMessage(const std::string& message);
};

#endif  // WEBSOCKETSERVER_H
