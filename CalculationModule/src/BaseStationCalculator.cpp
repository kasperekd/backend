// #include "BaseStationCalculator.h"

// #include <math.h>

// #include <numeric>
// #include <stdexcept>

// #include "CalculationModule.h"

// BaseStationCalculator::BaseStationCalculator() {}

// BaseStationCoordinates BaseStationCalculator::calculate(
//     const std::vector<CellData>& cellData) {
//     if (cellData.empty()) {
//         throw std::invalid_argument(
//             "Cell data is empty. Cannot calculate base station
//             coordinates.");
//     }

//     auto [latitude, longitude] = triangulate(cellData);
//     return {cellData[0].cell_id, latitude, longitude};
// }

// double BaseStationCalculator::calculateDistance(int signalStrength) {
//     // Пример формулы для расчета расстояния от RSSI:
//     // distance = 10 ^ ((A - RSSI) / 20)
//     constexpr int A =
//         -40;  // RSSI на расстоянии 1 метра (можно уточнить эмпирически)
//     return std::pow(10.0, (A - signalStrength) / 20.0);
// }

// std::pair<double, double> BaseStationCalculator::triangulate(
//     const std::vector<CellData>& cellData) {
//     if (cellData.size() < 3) {
//         throw std::invalid_argument(
//             "Not enough data points for triangulation (minimum 3
//             required).");
//     }

//     std::vector<double> latitudes, longitudes, distances;

//     for (const auto& cell : cellData) {
//         double distance = calculateDistance(cell.signal_strength);
//         latitudes.push_back(cell.latitude);
//         longitudes.push_back(cell.longitude);
//         distances.push_back(distance);
//     }

//     // Усреднение координат пересечения окружностей (грубое приближение)
//     double weightedLatitude = 0.0, weightedLongitude = 0.0;
//     double weightSum = 0.0;

//     for (size_t i = 0; i < cellData.size(); ++i) {
//         double weight = 1.0 / distances[i];  // Вес по обратному расстоянию
//         weightedLatitude += latitudes[i] * weight;
//         weightedLongitude += longitudes[i] * weight;
//         weightSum += weight;
//     }

//     if (weightSum == 0) {
//         throw std::runtime_error("Error in triangulation: weight sum is
//         zero.");
//     }

//     return {weightedLatitude / weightSum, weightedLongitude / weightSum};
// }

// double BaseStationCalculator::calculateWeightedAverage(
//     const std::vector<double>& values, const std::vector<int>& weights) {
//     if (values.size() != weights.size()) {
//         throw std::invalid_argument("Mismatched sizes for values and
//         weights.");
//     }

//     double weightedSum = 0.0;
//     int weightSum = 0;

//     for (size_t i = 0; i < values.size(); ++i) {
//         weightedSum += values[i] * weights[i];
//         weightSum += weights[i];
//     }

//     if (weightSum == 0) {
//         throw std::runtime_error(
//             "Total weight is zero. Cannot calculate weighted average.");
//     }

//     return weightedSum / weightSum;
// }

#include "BaseStationCalculator.h"

#include <cmath>
#include <stdexcept>
#include <vector>

#include "CalculationModule.h"

BaseStationCalculator::BaseStationCalculator() {}

BaseStationCoordinates BaseStationCalculator::calculate(
    const std::vector<CellData>& cellData) {
    if (cellData.empty()) {
        throw std::invalid_argument(
            "Cell data is empty. Cannot calculate base station coordinates.");
    }

    auto [latitude, longitude] = calculateByDirection(cellData);
    return {cellData[0].cell_id, latitude, longitude};
}

double BaseStationCalculator::calculateDistance(int signalStrength) {
    constexpr int A =
        -40;  // RSSI на расстоянии 1 метра (можно уточнить эмпирически)
    return std::pow(10.0, (A - signalStrength) / 20.0);
}

std::pair<double, double> BaseStationCalculator::calculateByDirection(
    const std::vector<CellData>& cellData) {
    if (cellData.size() < 2) {
        throw std::invalid_argument(
            "Not enough data points for directional calculation (minimum 2 "
            "required).");
    }

    std::vector<std::pair<double, double>> directions;
    for (const auto& cell : cellData) {
        double angle = calculateAngle(cell.signal_strength, cell.rsrq);
        directions.emplace_back(cell.latitude + std::cos(angle),
                                cell.longitude + std::sin(angle));
    }

    double intersectionLatitude = 0.0;
    double intersectionLongitude = 0.0;
    size_t count = 0;

    for (size_t i = 0; i < directions.size(); ++i) {
        for (size_t j = i + 1; j < directions.size(); ++j) {
            auto intersection = calculateIntersection(
                {cellData[i].latitude, cellData[i].longitude}, directions[i],
                {cellData[j].latitude, cellData[j].longitude}, directions[j]);

            if (intersection.has_value()) {
                intersectionLatitude += intersection->first;
                intersectionLongitude += intersection->second;
                ++count;
            }
        }
    }

    if (count == 0) {
        throw std::runtime_error(
            "Unable to determine base station location from provided "
            "directions.");
    }

    return {intersectionLatitude / count, intersectionLongitude / count};
}

double BaseStationCalculator::calculateAngle(int signalStrength, int rsrq) {
    // Пример: угол на основе пропорции уровня сигнала
    return (90.0 - (signalStrength / 100.0 * 90.0)) *
           (M_PI / 180.0);  // перевод в радианы
}

std::optional<std::pair<double, double>>
BaseStationCalculator::calculateIntersection(
    const std::pair<double, double>& point1,
    const std::pair<double, double>& direction1,
    const std::pair<double, double>& point2,
    const std::pair<double, double>& direction2) {
    double x1 = point1.first, y1 = point1.second;
    double dx1 = direction1.first - x1, dy1 = direction1.second - y1;
    double x2 = point2.first, y2 = point2.second;
    double dx2 = direction2.first - x2, dy2 = direction2.second - y2;

    double determinant = dx1 * dy2 - dx2 * dy1;

    if (std::fabs(determinant) < 1e-6) {
        return std::nullopt;  // Прямые параллельны
    }

    double t = ((x2 - x1) * dy2 - (y2 - y1) * dx2) / determinant;
    double intersectionX = x1 + t * dx1;
    double intersectionY = y1 + t * dy1;

    return {{intersectionX, intersectionY}};
}
