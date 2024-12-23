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

pqxx::result PostgresDB::executeQuery(const std::string& query,
                                      const std::vector<std::string>& params) {
    try {
        // std::cout << "Parameters count: " << params.size() << std::endl;
        // for (size_t i = 0; i < params.size(); ++i) {
        //     std::cout << "Param[" << i << "]: " << params[i] << std::endl;
        // }

        if (params.size() != 10) {
            throw std::runtime_error(
                "Incorrect number of parameters for query. Expected 10.");
        }

        std::vector<const char*> c_params;
        c_params.reserve(params.size());
        for (const auto& param : params) {
            if (param.empty()) {
                throw std::runtime_error(
                    "Empty string detected in query parameters.");
            }
            c_params.push_back(param.c_str());
        }

        pqxx::work txn(*m_connection);

        pqxx::result res =
            txn.exec_params(query, c_params[0], c_params[1], c_params[2],
                            c_params[3], c_params[4], c_params[5], c_params[6],
                            c_params[7], c_params[8], c_params[9]);
        txn.commit();

        return res;
    } catch (const std::exception& e) {
        throw std::runtime_error(
            std::string("Prepared query execution error: ") + e.what());
    }
}
