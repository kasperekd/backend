#include "BaseStationCalculator.h"

#include <math.h>

#include <numeric>
#include <stdexcept>

#include "CalculationModule.h"

BaseStationCalculator::BaseStationCalculator() {}

// BaseStationCoordinates BaseStationCalculator::calculate(
//     const std::vector<CellData>& cellData) {
//     if (cellData.empty()) {
//         throw std::invalid_argument(
//             "Cell data is empty. Cannot calculate base station
//             coordinates.");
//     }

//     std::vector<double> latitudes;
//     std::vector<double> longitudes;
//     std::vector<int> signalStrengths;

//     for (const auto& cell : cellData) {
//         latitudes.push_back(cell.latitude);
//         longitudes.push_back(cell.longitude);
//         signalStrengths.push_back(cell.signal_strength);
//     }

//     double weightedLatitude =
//         calculateWeightedAverage(latitudes, signalStrengths);
//     double weightedLongitude =
//         calculateWeightedAverage(longitudes, signalStrengths);

//     return {cellData[0].cell_id, weightedLatitude, weightedLongitude};
// }

BaseStationCoordinates BaseStationCalculator::calculate(
    const std::vector<CellData>& cellData) {
    if (cellData.empty()) {
        throw std::invalid_argument(
            "Cell data is empty. Cannot calculate base station coordinates.");
    }

    auto [latitude, longitude] = triangulate(cellData);
    return {cellData[0].cell_id, latitude, longitude};
}

double BaseStationCalculator::calculateDistance(int signalStrength) {
    // Пример формулы для расчета расстояния от RSSI:
    // distance = 10 ^ ((A - RSSI) / 20)
    constexpr int A =
        -40;  // RSSI на расстоянии 1 метра (можно уточнить эмпирически)
    return std::pow(10.0, (A - signalStrength) / 20.0);
}

std::pair<double, double> BaseStationCalculator::triangulate(
    const std::vector<CellData>& cellData) {
    if (cellData.size() < 3) {
        throw std::invalid_argument(
            "Not enough data points for triangulation (minimum 3 required).");
    }

    std::vector<double> latitudes, longitudes, distances;

    for (const auto& cell : cellData) {
        double distance = calculateDistance(cell.signal_strength);
        latitudes.push_back(cell.latitude);
        longitudes.push_back(cell.longitude);
        distances.push_back(distance);
    }

    // Усреднение координат пересечения окружностей (грубое приближение)
    double weightedLatitude = 0.0, weightedLongitude = 0.0;
    double weightSum = 0.0;

    for (size_t i = 0; i < cellData.size(); ++i) {
        double weight = 1.0 / distances[i];  // Вес по обратному расстоянию
        weightedLatitude += latitudes[i] * weight;
        weightedLongitude += longitudes[i] * weight;
        weightSum += weight;
    }

    if (weightSum == 0) {
        throw std::runtime_error("Error in triangulation: weight sum is zero.");
    }

    return {weightedLatitude / weightSum, weightedLongitude / weightSum};
}

double BaseStationCalculator::calculateWeightedAverage(
    const std::vector<double>& values, const std::vector<int>& weights) {
    if (values.size() != weights.size()) {
        throw std::invalid_argument("Mismatched sizes for values and weights.");
    }

    double weightedSum = 0.0;
    int weightSum = 0;

    for (size_t i = 0; i < values.size(); ++i) {
        weightedSum += values[i] * weights[i];
        weightSum += weights[i];
    }

    if (weightSum == 0) {
        throw std::runtime_error(
            "Total weight is zero. Cannot calculate weighted average.");
    }

    return weightedSum / weightSum;
}
