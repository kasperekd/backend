#ifndef RECEIVESERVER_H
#define RECEIVESERVER_H

#include <string>

#define DEBUG 1

class ReceiveServer {
   public:
    ReceiveServer() = default;
    ~ReceiveServer() = default;

    void start();

   private:
    void onMessage(const std::string& message);
};

#endif  // RECEIVESERVER_H
