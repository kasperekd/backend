#include <iostream>

#include "BaseStationCalculator.h"
#include "CalculationModule.h"

int main() {
    const std::string dbConnString =
        "dbname=signal_db user=user password=password host=db port=5432";
    // "dbname=mydb user=myuser password=mypass hostaddr=127.0.0.1 port=5432";
    const int intervalMinutes = 1;  // Интервал расчета в минутах

    CalculationModule module(dbConnString, intervalMinutes);

    try {
        module.start();  // Запуск цикла
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
