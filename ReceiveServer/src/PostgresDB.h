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
    pqxx::result executeQuery(const std::string& query,
                              const std::vector<std::string>& params);
    void executeQueryWithParams(const std::string& query,
                                const std::vector<std::string>& params);
    void prepareStatement(const std::string& stmtName,
                          const std::string& query);
    void executePreparedQuery(const std::string& stmtName,
                              const std::vector<std::string>& params);

   private:
    std::string m_connectionString;
    std::unique_ptr<pqxx::connection> m_connection;
};

#endif  // POSTGRESDB_H
