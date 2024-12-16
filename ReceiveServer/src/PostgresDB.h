#ifndef POSTGRESDB_H
#define POSTGRESDB_H

#include <optional>
#include <pqxx/pqxx>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

class PostgresDB {
   public:
    PostgresDB(const std::string& connectionString);
    ~PostgresDB();

    void connect();
    void disconnect();

    // pqxx::result executeQuery(const std::string& query);
    pqxx::result executeQuery(
        const std::string& query,
        const std::vector<std::optional<std::string>>& params);

   private:
    std::string m_connectionString;
    std::unique_ptr<pqxx::connection> m_connection;
};

#endif  // POSTGRESDB_H
