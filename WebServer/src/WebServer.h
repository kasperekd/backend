#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <crow.h>
// #include <crow/json.h>

#include <mutex>
#include <pqxx/pqxx>
#include <string>
#include <unordered_map>

// std::mutex cache_mutex;

struct BaseStationData {
    double latitude;
    double longitude;
    std::chrono::steady_clock::time_point timestamp;
};

class WebServer {
   private:
    std::unordered_map<long long, BaseStationData> cache;
    int cache_validity_duration_seconds = 60;

    std::optional<BaseStationData> getFromCache(long long cell_id);
    void addToCache(long long cell_id, double lat, double lon);

   public:
    crow::json::wvalue getBaseStationCoordinatesJson(long long cell_id);
    crow::json::wvalue getAllBaseStationCoordinatesJson();
    std::string readFile(const std::string& path);
    void start();
};

#endif  // WEBSERVER_H
