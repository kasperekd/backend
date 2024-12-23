#include <iostream>

#include "BaseStationCalculator.h"
#include "CalculationModule.h"

int main() {
    const std::string dbConnString =
        "dbname=signal_db user=user password=password host=db port=5432";
    const int intervalMinutes = 1;

    CalculationModule module(dbConnString, intervalMinutes);

    try {
        module.start();
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
