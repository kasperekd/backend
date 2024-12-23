#include "CalculationModule.h"

#include <iostream>
#include <map>
#include <pqxx/pqxx>
#include <stdexcept>

// Конструктор
CalculationModule::CalculationModule(const std::string& dbConnString,
                                     int intervalMinutes)
    : dbConnString(dbConnString),
      intervalMinutes(intervalMinutes),
      running(true) {}

// Метод запуска
void CalculationModule::start() {
    while (running) {
        try {
            processCalculations();
            std::this_thread::sleep_for(std::chrono::minutes(intervalMinutes));
        } catch (const std::exception& ex) {
            std::cerr << "Error in CalculationModule: " << ex.what()
                      << std::endl;
        }
    }
}

// Основной процесс вычислений
void CalculationModule::processCalculations() {
    // Получаем данные из БД, сгруппированные по cell_id
    auto groupedData = fetchGroupedDataFromDatabase();
    BaseStationCalculator calculator;

    for (const auto& group : groupedData) {
        try {
            std::cout << "cell_id: " << group[0].cell_id << std::endl;
            auto result = calculator.calculate(group);
            writeResultsToDatabase(result);
        } catch (const std::exception& ex) {
            std::cerr << "Error calculating coordinates for cell group: "
                      << ex.what() << std::endl;
        }
    }
}

// Получение данных из БД, сгруппированных по cell_id
std::vector<std::vector<CellData>>
CalculationModule::fetchGroupedDataFromDatabase() {
    std::map<int, std::vector<CellData>> groupedData;
    try {
        pqxx::connection connection(dbConnString);
        pqxx::work txn(connection);

        auto result = txn.exec(
            "SELECT id, latitude, longitude, timestamp, cell_id, "
            "signal_strength, "
            "tracking_area_code, operator, rsrp, rsrq "
            "FROM cell_data ORDER BY cell_id");

        for (const auto& row : result) {
            CellData cell = {row["id"].as<int>(),
                             row["latitude"].as<double>(),
                             row["longitude"].as<double>(),
                             row["timestamp"].as<long long>(),
                             row["cell_id"].as<int>(),
                             row["signal_strength"].as<int>(),
                             row["tracking_area_code"].as<int>(),
                             row["operator"].c_str(),
                             row["rsrp"].as<int>(),
                             row["rsrq"].as<int>()};
            // std::cout << cell.signal_strength << "\n";
            groupedData[cell.cell_id].push_back(cell);
        }

    } catch (const std::exception& ex) {
        std::cerr << "Database fetch error: " << ex.what() << std::endl;
    }

    // Преобразуем map в vector<vector<CellData>> для удобства
    std::vector<std::vector<CellData>> grouped;
    for (auto& [cell_id, group] : groupedData) {
        grouped.push_back(std::move(group));
    }
    return grouped;
}

// Запись результата в БД
void CalculationModule::writeResultsToDatabase(
    const BaseStationCoordinates& result) {
    try {
        pqxx::connection connection(dbConnString);
        pqxx::work txn(connection);
        std::cout << result.cell_id << " " << result.calculated_latitude
                  << "\n";
        txn.exec_params(
            "INSERT INTO base_station_coordinates (cell_id, "
            "calculated_latitude, calculated_longitude) "
            "VALUES ($1, $2, $3) ON CONFLICT (cell_id) DO UPDATE SET "
            "calculated_latitude = EXCLUDED.calculated_latitude, "
            "calculated_longitude = EXCLUDED.calculated_longitude",
            result.cell_id, result.calculated_latitude,
            result.calculated_longitude);

        txn.commit();
    } catch (const std::exception& ex) {
    }
}
