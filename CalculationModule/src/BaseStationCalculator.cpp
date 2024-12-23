#include "BaseStationCalculator.h"

#include <numeric>
#include <stdexcept>

#include "CalculationModule.h"

// Конструктор
BaseStationCalculator::BaseStationCalculator() {}

// Основной метод расчета координат
BaseStationCoordinates BaseStationCalculator::calculate(
    const std::vector<CellData>& cellData) {
    if (cellData.empty()) {
        throw std::invalid_argument(
            "Cell data is empty. Cannot calculate base station coordinates.");
    }

    std::vector<double> latitudes;
    std::vector<double> longitudes;
    std::vector<int> signalStrengths;

    for (const auto& cell : cellData) {
        latitudes.push_back(cell.latitude);
        longitudes.push_back(cell.longitude);
        signalStrengths.push_back(
            cell.signal_strength);  // Используем signal_strength как вес
    }

    // Вычисление средневзвешенных значений
    double weightedLatitude =
        calculateWeightedAverage(latitudes, signalStrengths);
    double weightedLongitude =
        calculateWeightedAverage(longitudes, signalStrengths);

    return {cellData[0].cell_id,  // Предполагаем, что данные для одного cell_id
            weightedLatitude, weightedLongitude};
}

// Вспомогательный метод для расчета средневзвешенного
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
