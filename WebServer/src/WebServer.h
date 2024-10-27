#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>

class WebServer {
   public:
    WebServer() = default;
    ~WebServer() = default;

    void start();

   private:
    std::string readFile(const std::string& path);
};

#endif  // WEBSERVER_H
