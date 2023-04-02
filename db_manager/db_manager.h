#pragma once

#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

class DBManager {
protected:
    pqxx::connection* conn_;
    string dbname_;
    string user_;
    string password_;
    string host_;
    string port_;

public:
    DBManager(string dbname, string user, string password, string host = "localhost", string port = "5432");
    ~DBManager();

    bool connect();
    void disconnect();

    bool execute(string sql_query);

    pqxx::result execute_query(string sql_query);

    template<typename T>
    T get_value(string sql_query);

    template<typename T>
    vector<T> get_values(string sql_query);
};
