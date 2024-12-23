#ifndef CALCULATION_MODULE_H
#define CALCULATION_MODULE_H

#include <chrono>
#include <string>
#include <thread>

#include "BaseStationCalculator.h"
#include "CalculationModule.h"

#ifndef BSC_STUCT
struct CellData {
    int id;
    double latitude;
    double longitude;
    long long timestamp;
    int cell_id;
    int signal_strength;
    int tracking_area_code;
    std::string operator_name;
    int rsrp;
    int rsrq;
};
struct BaseStationCoordinates {
    int cell_id;
    double calculated_latitude;
    double calculated_longitude;
};
#define BSC_STUCT
#endif  // BSC_STUCT

class CalculationModule {
   public:
    CalculationModule(const std::string& dbConnString, int intervalMinutes);
    void start();

   private:
    std::string dbConnString;
    int intervalMinutes;
    bool running;

    void processCalculations();
    std::vector<std::vector<CellData>> fetchGroupedDataFromDatabase();
    void writeResultsToDatabase(const BaseStationCoordinates& result);
};

#endif  // CALCULATION_MODULE_H
