#ifndef RECEIVESERVER_H
#define RECEIVESERVER_H

#include <string>

#include "PostgresDB.h"

#define DEBUG 1

class ReceiveServer {
   public:
    ReceiveServer() = default;
    ~ReceiveServer() = default;

    void start();

   private:
    void onMessage(const std::string& message);
    void parseAndSaveData(const std::string& rawJson, PostgresDB& db);
};

#endif  // RECEIVESERVER_H
