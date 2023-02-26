#include "db_manager.h"

TDBManager::TDBManager(const std::string& db_name, const std::string& table_name) :
    connection("dbname=" + db_name),
    table_name(table_name)
{
    // Check if the connection is successful
    if (!connection.is_open()) {
        throw std::runtime_error("Failed to connect to database");
    }
}

void TDBManager::PostData(const std::string& data) {
    // Create a transaction object
    pqxx::work txn(connection);

    // Execute the SQL statement to insert data into the table
    txn.exec("INSERT INTO " + table_name + " (data) VALUES ('" + data + "')");

    // Commit the transaction
    txn.commit();
}

std::string TDBManager::GetData() {
    // Create a transaction object
    pqxx::work txn(connection);

    // Execute the SQL statement to select data from the table
    pqxx::result result = txn.exec("SELECT data FROM " + table_name);

    // Convert the result set to a string and return it
    std::string data;
    for (auto row : result) {
        data += row[0].as<std::string>() + "\n";
    }
    return data;
}