#include "BaseStationCalculator.h"

#include <math.h>

#include <numeric>
#include <stdexcept>

#include "CalculationModule.h"

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

// #include "BaseStationCalculator.h"

// #include <cmath>
// #include <stdexcept>
// #include <vector>

// #include "CalculationModule.h"

// BaseStationCalculator::BaseStationCalculator() {}

// BaseStationCoordinates BaseStationCalculator::calculate(
//     const std::vector<CellData>& cellData) {
//     if (cellData.empty()) {
//         throw std::invalid_argument(
//             "Cell data is empty. Cannot calculate base station
//             coordinates.");
//     }

//     auto [latitude, longitude] = calculateByDirection(cellData);
//     return {cellData[0].cell_id, latitude, longitude};
// }

// double BaseStationCalculator::calculateDistance(int signalStrength) {
//     constexpr int A =
//         -40;  // RSSI на расстоянии 1 метра (можно уточнить эмпирически)
//     return std::pow(10.0, (A - signalStrength) / 20.0);
// }

// std::pair<double, double> BaseStationCalculator::calculateByDirection(
//     const std::vector<CellData>& cellData) {
//     if (cellData.size() < 2) {
//         throw std::invalid_argument(
//             "Not enough data points for directional calculation (minimum 2 "
//             "required).");
//     }

//     std::vector<std::pair<double, double>> directions;
//     for (const auto& cell : cellData) {
//         double angle = calculateAngle(cell.signal_strength, cell.rsrq);
//         directions.emplace_back(cell.latitude + std::cos(angle),
//                                 cell.longitude + std::sin(angle));
//     }

//     double intersectionLatitude = 0.0;
//     double intersectionLongitude = 0.0;
//     size_t count = 0;

//     for (size_t i = 0; i < directions.size(); ++i) {
//         for (size_t j = i + 1; j < directions.size(); ++j) {
//             auto intersection = calculateIntersection(
//                 {cellData[i].latitude, cellData[i].longitude}, directions[i],
//                 {cellData[j].latitude, cellData[j].longitude},
//                 directions[j]);

//             if (intersection.has_value()) {
//                 intersectionLatitude += intersection->first;
//                 intersectionLongitude += intersection->second;
//                 ++count;
//             }
//         }
//     }

//     if (count == 0) {
//         throw std::runtime_error(
//             "Unable to determine base station location from provided "
//             "directions.");
//     }

//     return {intersectionLatitude / count, intersectionLongitude / count};
// }

// double BaseStationCalculator::calculateAngle(int signalStrength, int rsrq) {
//     // Пример: угол на основе пропорции уровня сигнала
//     return (90.0 - (signalStrength / 100.0 * 90.0)) *
//            (M_PI / 180.0);  // перевод в радианы
// }

// std::optional<std::pair<double, double>>
// BaseStationCalculator::calculateIntersection(
//     const std::pair<double, double>& point1,
//     const std::pair<double, double>& direction1,
//     const std::pair<double, double>& point2,
//     const std::pair<double, double>& direction2) {
//     double x1 = point1.first, y1 = point1.second;
//     double dx1 = direction1.first - x1, dy1 = direction1.second - y1;
//     double x2 = point2.first, y2 = point2.second;
//     double dx2 = direction2.first - x2, dy2 = direction2.second - y2;

//     double determinant = dx1 * dy2 - dx2 * dy1;

//     if (std::fabs(determinant) < 1e-6) {
//         return std::nullopt;  // Прямые параллельны
//     }

//     double t = ((x2 - x1) * dy2 - (y2 - y1) * dx2) / determinant;
//     double intersectionX = x1 + t * dx1;
//     double intersectionY = y1 + t * dy1;

//     return {{intersectionX, intersectionY}};
// }

// #include <bits/algorithmfwd.h>

// #include <map>
// BaseStationCalculator::BaseStationCalculator() {}

// BaseStationCoordinates BaseStationCalculator::calculate(
//     const std::vector<CellData>& cellData) {
//     if (cellData.empty()) {
//         throw std::invalid_argument(
//             "Cell data is empty. Cannot calculate base station
//             coordinates.");
//     }

//     auto [latitude, longitude] =
//     calculateByDirectionWithClustering(cellData); return
//     {cellData[0].cell_id, latitude, longitude};
// }

// double BaseStationCalculator::calculateDistance(int signalStrength) {
//     constexpr int A = -67;  // RSSI на расстоянии 1 метра
//     return std::pow(10.0, (A - signalStrength) / 20.0);
// }

// std::pair<double, double>
// BaseStationCalculator::calculateByDirectionWithClustering(
//     const std::vector<CellData>& cellData) {
//     if (cellData.size() < 2) {
//         throw std::invalid_argument(
//             "Not enough data points for directional calculation (minimum 2 "
//             "required).");
//     }

//     std::vector<std::pair<double, double>> directions;
//     for (const auto& cell : cellData) {
//         double angle = calculateAngle(cell.signal_strength, cell.rsrq);
//         directions.emplace_back(cell.latitude + std::cos(angle),
//                                 cell.longitude + std::sin(angle));
//     }

//     std::vector<std::pair<double, double>> intersections;

//     for (size_t i = 0; i < directions.size(); ++i) {
//         for (size_t j = i + 1; j < directions.size(); ++j) {
//             auto intersection = calculateIntersection(
//                 {cellData[i].latitude, cellData[i].longitude}, directions[i],
//                 {cellData[j].latitude, cellData[j].longitude},
//                 directions[j]);

//             if (intersection.has_value()) {
//                 intersections.push_back(intersection.value());
//             }
//         }
//     }

//     if (intersections.empty()) {
//         throw std::runtime_error(
//             "Unable to determine base station location from provided "
//             "directions.");
//     }

//     return calculateClusterCentroid(intersections);
// }

// double BaseStationCalculator::calculateAngle(int signalStrength, int rsrq) {
//     return (90.0 - (signalStrength / 100.0 * 90.0)) *
//            (M_PI / 180.0);  // перевод в радианы
// }

// std::optional<std::pair<double, double>>
// BaseStationCalculator::calculateIntersection(
//     const std::pair<double, double>& point1,
//     const std::pair<double, double>& direction1,
//     const std::pair<double, double>& point2,
//     const std::pair<double, double>& direction2) {
//     double x1 = point1.first, y1 = point1.second;
//     double dx1 = direction1.first - x1, dy1 = direction1.second - y1;
//     double x2 = point2.first, y2 = point2.second;
//     double dx2 = direction2.first - x2, dy2 = direction2.second - y2;

//     double determinant = dx1 * dy2 - dx2 * dy1;

//     if (std::fabs(determinant) < 1e-6) {
//         return std::nullopt;  // Прямые параллельны
//     }

//     double t = ((x2 - x1) * dy2 - (y2 - y1) * dx2) / determinant;
//     double intersectionX = x1 + t * dx1;
//     double intersectionY = y1 + t * dy1;

//     return {{intersectionX, intersectionY}};
// }

// std::pair<double, double> BaseStationCalculator::calculateClusterCentroid(
//     const std::vector<std::pair<double, double>>& points) {
//     constexpr double clusterRadius = 0.001;  // Радиус кластера в градусах
//     std::map<size_t, std::vector<std::pair<double, double>>> clusters;
//     size_t clusterIndex = 0;

//     for (const auto& point : points) {
//         bool addedToCluster = false;
//         for (auto& [index, cluster] : clusters) {
//             for (const auto& clusterPoint : cluster) {
//                 if (distanceBetweenPoints(point, clusterPoint) <=
//                     clusterRadius) {
//                     cluster.push_back(point);
//                     addedToCluster = true;
//                     break;
//                 }
//             }
//             if (addedToCluster) break;
//         }
//         if (!addedToCluster) {
//             clusters[clusterIndex++].push_back(point);
//         }
//     }

//     // Находим наиболее крупный кластер
//     auto largestCluster = std::max_element(
//         clusters.begin(), clusters.end(), [](const auto& a, const auto& b) {
//             return a.second.size() < b.second.size();
//         });

//     if (largestCluster == clusters.end() || largestCluster->second.empty()) {
//         throw std::runtime_error("No valid clusters found for averaging.");
//     }

//     // Вычисляем центроид самого крупного кластера
//     double avgLatitude = 0.0, avgLongitude = 0.0;
//     for (const auto& point : largestCluster->second) {
//         avgLatitude += point.first;
//         avgLongitude += point.second;
//     }

//     size_t clusterSize = largestCluster->second.size();
//     return {avgLatitude / clusterSize, avgLongitude / clusterSize};
// }

// double BaseStationCalculator::distanceBetweenPoints(
//     const std::pair<double, double>& point1,
//     const std::pair<double, double>& point2) {
//     double latDiff = point1.first - point2.first;
//     double lonDiff = point1.second - point2.second;
//     return std::sqrt(latDiff * latDiff + lonDiff * lonDiff);
// }

BaseStationCalculator::BaseStationCalculator() {}

BaseStationCoordinates BaseStationCalculator::calculate(
    const std::vector<CellData>& cellData) {
    if (cellData.empty()) {
        throw std::invalid_argument(
            "Cell data is empty. Cannot calculate base station coordinates.");
    }

    // Шаг 1: Предварительная обработка данных
    auto processedData = preprocessData(cellData);
    if (processedData.size() < 3) {
        throw std::runtime_error(
            "Not enough valid data points for trilateration (minimum 3 "
            "required).");
    }

    // Шаг 2: Расчёт координат с использованием трилатерации
    auto coordinates = trilateration(processedData);
    if (!coordinates.has_value()) {
        throw std::runtime_error(
            "Unable to determine base station location via trilateration.");
    }

    // Возвращаем результат
    return {cellData[0].cell_id, coordinates->first, coordinates->second};
}

std::vector<std::tuple<double, double, double>>
BaseStationCalculator::preprocessData(const std::vector<CellData>& cellData) {
    std::map<std::pair<double, double>, std::vector<int>> groupedSignals;

    // Группируем сигналы по координатам
    for (const auto& cell : cellData) {
        groupedSignals[{cell.latitude, cell.longitude}].push_back(
            cell.signal_strength);
    }

    std::vector<std::tuple<double, double, double>> processedData;

    // Вычисляем медианное значение силы сигнала и расстояние для каждой группы
    for (const auto& [coords, signals] : groupedSignals) {
        auto& [lat, lon] = coords;

        if (signals.empty()) continue;

        // Сортируем для медианного значения
        auto sortedSignals = signals;
        std::sort(sortedSignals.begin(), sortedSignals.end());
        int medianSignal = sortedSignals[sortedSignals.size() / 2];

        double distance = calculateDistance(medianSignal);
        processedData.emplace_back(lat, lon, distance);
    }

    return processedData;
}

double BaseStationCalculator::calculateDistance(int signalStrength) {
    constexpr int A = -67;  // RSSI на расстоянии 1 метра
    return std::pow(10.0, (A - signalStrength) / 20.0);  // Метры
}

std::optional<std::pair<double, double>> BaseStationCalculator::trilateration(
    const std::vector<std::tuple<double, double, double>>& points) {
    if (points.size() < 3) {
        return std::nullopt;  // Трилатерация невозможна
    }

    // Используем первые три точки для трилатерации
    auto [x1, y1, r1] = points[0];
    auto [x2, y2, r2] = points[1];
    auto [x3, y3, r3] = points[2];

    double A = 2 * (x2 - x1);
    double B = 2 * (y2 - y1);
    double C = r1 * r1 - r2 * r2 - x1 * x1 + x2 * x2 - y1 * y1 + y2 * y2;

    double D = 2 * (x3 - x2);
    double E = 2 * (y3 - y2);
    double F = r2 * r2 - r3 * r3 - x2 * x2 + x3 * x3 - y2 * y2 + y3 * y3;

    double denominator = A * E - B * D;
    if (std::fabs(denominator) < 1e-6) {
        return std::nullopt;  // Прямые пересечения параллельны
    }

    double px = (C * E - B * F) / denominator;
    double py = (A * F - C * D) / denominator;

    return {{px, py}};
}

double BaseStationCalculator::calculateWeight(int rsrq) {
    // Чем выше качество (RSRQ), тем больше вес (положительное значение)
    constexpr int maxRsrq = -3;   // Теоретически лучший RSRQ
    constexpr int minRsrq = -20;  // Наихудший RSRQ

    return std::max(0.1, 1.0 - (maxRsrq - rsrq) / double(maxRsrq - minRsrq));
}

double BaseStationCalculator::distanceBetweenPoints(
    const std::pair<double, double>& point1,
    const std::pair<double, double>& point2) {
    double latDiff = point1.first - point2.first;
    double lonDiff = point1.second - point2.second;
    return std::sqrt(latDiff * latDiff + lonDiff * lonDiff);
}
