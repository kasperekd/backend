#ifndef BASE_STATION_CALCULATOR_H
#define BASE_STATION_CALCULATOR_H

#include <bits/algorithmfwd.h>

#include <map>
#include <optional>
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
    double calculateAngle(int signalStrength, int rsrq);

    std::pair<double, double> calculateByDirectionWithClustering(
        const std::vector<CellData>& cellData);
    std::pair<double, double> calculateClusterCentroid(
        const std::vector<std::pair<double, double>>& points);
    double distanceBetweenPoints(const std::pair<double, double>& point1,
                                 const std::pair<double, double>& point2);
    std::optional<std::pair<double, double>> calculateIntersection(
        const std::pair<double, double>& point1,
        const std::pair<double, double>& direction1,
        const std::pair<double, double>& point2,
        const std::pair<double, double>& direction2);

    std::pair<double, double> calculateByDirection(
        const std::vector<CellData>& cellData);
    double calculateWeightedAverage(const std::vector<double>& values,
                                    const std::vector<int>& weights);

    std::vector<std::tuple<double, double, double>> preprocessData(
        const std::vector<CellData>& cellData);
    std::optional<std::pair<double, double>> trilateration(
        const std::vector<std::tuple<double, double, double>>& points);
    double BaseStationCalculator::calculateWeight(int rsrq);
};

#endif  // BASE_STATION_CALCULATOR_H
