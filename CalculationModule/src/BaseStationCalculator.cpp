#include "BaseStationCalculator.h"

#include <math.h>

#include <numeric>
#include <stdexcept>

#include "CalculationModule.h"

BaseStationCalculator::BaseStationCalculator() {}

BaseStationCoordinates BaseStationCalculator::calculate(
    const std::vector<CellData>& cellData) {
    if (cellData.empty()) {
        throw std::invalid_argument(
            "Cell data is empty. Cannot calculate base station coordinates.");
    }

    auto processedData = preprocessData(cellData);
    if (processedData.size() < 3) {
        throw std::runtime_error(
            "Not enough valid data points for trilateration (minimum 3 "
            "required)");
    }

    auto coordinates = trilateration(processedData);
    if (!coordinates.has_value()) {
        double avgLat = 0, avgLon = 0;
        for (const auto& [lat, lon, dist] : processedData) {
            avgLat += lat;
            avgLon += lon;
        }
        size_t pointCount = processedData.size();
        return {cellData[0].cell_id, avgLat / pointCount, avgLon / pointCount};
    }

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

        // для медианного значения
        auto sortedSignals = signals;
        std::sort(sortedSignals.begin(), sortedSignals.end());
        int medianSignal = sortedSignals[sortedSignals.size() / 2];

        double distance = calculateDistance(medianSignal);
        processedData.emplace_back(lat, lon, distance);
    }

    return processedData;
}

double BaseStationCalculator::calculateDistance(int signalStrength) {
    constexpr int A = -40;  // RSSI на расстоянии 1 метра
    return std::pow(10.0, (A - signalStrength) / 20.0);  // Метры
}

std::optional<std::pair<double, double>> BaseStationCalculator::trilateration(
    const std::vector<std::tuple<double, double, double>>& points) {
    if (points.size() < 3) {
        return std::nullopt;  // Трилатерация невозможна
    }

    // Перебираем комбинации трёх точек для устойчивости
    std::optional<std::pair<double, double>> result;
    double minError = std::numeric_limits<double>::max();

    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            for (size_t k = j + 1; k < points.size(); ++k) {
                auto [x1, y1, r1] = points[i];
                auto [x2, y2, r2] = points[j];
                auto [x3, y3, r3] = points[k];

                double A = 2 * (x2 - x1);
                double B = 2 * (y2 - y1);
                double C =
                    r1 * r1 - r2 * r2 - x1 * x1 + x2 * x2 - y1 * y1 + y2 * y2;

                double D = 2 * (x3 - x2);
                double E = 2 * (y3 - y2);
                double F =
                    r2 * r2 - r3 * r3 - x2 * x2 + x3 * x3 - y2 * y2 + y3 * y3;

                double denominator = A * E - B * D;
                if (std::fabs(denominator) < 1e-6) {
                    continue;  // Эти три окружности не пересекаются
                }

                double px = (C * E - B * F) / denominator;
                double py = (A * F - C * D) / denominator;

                // Рассчитываем ошибку как отклонение от радиусов
                double error = std::abs(std::hypot(px - x1, py - y1) - r1) +
                               std::abs(std::hypot(px - x2, py - y2) - r2) +
                               std::abs(std::hypot(px - x3, py - y3) - r3);

                if (error < minError) {
                    minError = error;
                    result = {{px, py}};
                }
            }
        }
    }

    if (result.has_value()) {
        return result;
    }

    return std::nullopt;  // Если не удалось найти результат
}

double BaseStationCalculator::calculateWeight(int rsrq) {
    // Чем выше качество (RSRQ), тем больше вес (положительное значение)
    constexpr int maxRsrq = -3;
    constexpr int minRsrq = -20;

    return std::max(0.1, 1.0 - (maxRsrq - rsrq) / double(maxRsrq - minRsrq));
}

double BaseStationCalculator::distanceBetweenPoints(
    const std::pair<double, double>& point1,
    const std::pair<double, double>& point2) {
    double latDiff = point1.first - point2.first;
    double lonDiff = point1.second - point2.second;
    return std::sqrt(latDiff * latDiff + lonDiff * lonDiff);
}
