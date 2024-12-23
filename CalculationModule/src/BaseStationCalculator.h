#ifndef BASE_STATION_CALCULATOR_H
#define BASE_STATION_CALCULATOR_H

#include <vector>

#include "BaseStationCalculator.h"
#include "CalculationModule.h"  // Подключаем структуру CellData и другие зависимости

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

class BaseStationCalculator {
   public:
    BaseStationCalculator();

    BaseStationCoordinates calculate(const std::vector<CellData>& cellData);

   private:
    std::pair<double, double> triangulate(
        const std::vector<CellData>& cellData);
    double calculateDistance(int signalStrength);
    double calculateWeightedAverage(const std::vector<double>& values,
                                    const std::vector<int>& weights);
};

#endif  // BASE_STATION_CALCULATOR_H
