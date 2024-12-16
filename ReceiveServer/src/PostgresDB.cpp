#include "PostgresDB.h"

#include <iostream>
#include <stdexcept>

PostgresDB::PostgresDB(const std::string& connectionString)
    : m_connectionString(connectionString), m_connection(nullptr) {}

PostgresDB::~PostgresDB() { disconnect(); }

void PostgresDB::connect() {
    try {
        m_connection = std::make_unique<pqxx::connection>(m_connectionString);
        if (m_connection->is_open()) {
            std::cout << "Connected to database successfully!" << std::endl;
        } else {
            throw std::runtime_error("Failed to open database connection.");
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Connection error: ") + e.what());
    }
}

void PostgresDB::disconnect() {
    if (m_connection && m_connection->is_open()) {
        m_connection->disconnect();
        std::cout << "Disconnected from database successfully!" << std::endl;
    }
}

pqxx::result PostgresDB::executeQuery(
    const std::string& query,
    const std::vector<std::optional<std::string>>& params) {
    try {
        // Преобразуем std::optional<std::string> в массив указателей const
        // char*
        std::vector<const char*> c_params;
        c_params.reserve(params.size());
        for (const auto& param : params) {
            if (param.has_value()) {
                c_params.push_back(param->c_str());
            } else {
                c_params.push_back(nullptr);  // NULL значение
            }
        }

        pqxx::work txn(*m_connection);

        // Передаём параметры напрямую в exec_params через распаковку аргументов
        pqxx::result res = txn.exec_params(query, c_params.data(),
                                           c_params.data() + c_params.size());
        txn.commit();
        return res;
    } catch (const std::exception& e) {
        throw std::runtime_error(
            std::string("Prepared query execution error: ") + e.what());
    }
}
