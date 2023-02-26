#pragma once

#include <string>
#include <pqxx/pqxx>
#include <stdexcept>

class TDBManager {
protected:
    bool PathIsValid(const std::string& db_name);

    // Database connection object
    pqxx::connection connection;

    // Name of the table to manage
    std::string table_name;

public:
    TDBManager(const std::string& db_name, const std::string& table_name);

    // Function to post data to the database
    void PostData(const std::string& data);

    // Function to get data from the database
    std::string GetData();
};