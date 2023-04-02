#include "db_manager.h"
#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

DBManager::DBManager(string dbname, string user, string password, string host, string port) :
        conn_(nullptr),
        dbname_(dbname),
        user_(user),
        password_(password),
        host_(host),
        port_(port)
{}

DBManager::~DBManager() {
    disconnect();
}

bool DBManager::connect() {
    if (conn_ != nullptr && conn_->is_open()) {
        return true;
    }

    try {
        string conn_str = "dbname=" + dbname_ + " user=" + user_ + " password=" + password_ +
                          " host=" + host_ + " port=" + port_;
        conn_ = new connection(conn_str);
        if (conn_->is_open()) {
            cout << "Connected to database successfully!" << endl;
            return true;
        }
    }
    catch (const exception& e) {
        cerr << "Error connecting to database: " << e.what() << endl;
    }

    return false;
}

void DBManager::disconnect() {
    if (conn_ != nullptr && conn_->is_open()) {
        conn_->disconnect();
        cout << "Disconnected from database." << endl;
    }
}

bool DBManager::execute(string sql_query) {
    if (conn_ == nullptr || !conn_->is_open()) {
        cerr << "Not connected to database!" << endl;
        return false;
    }

    try {
        work txn(*conn_);
        txn.exec(sql_query);
        txn.commit();
        return true;
    }
    catch (const exception& e) {
        cerr << "Error executing SQL query: " << e.what() << endl;
        return false;
    }
}

result DBManager::execute_query(string sql_query) {
    if (conn_ == nullptr || !conn_->is_open()) {
        cerr << "Not connected to database!" << endl;
        return result();
    }

    try {
        work txn(*conn_);
        result r = txn.exec(sql_query);
        txn.commit();
        return r;
    }
    catch (const exception& e) {
        cerr << "Error executing SQL query: " << e.what() << endl;
        return result();
    }
}

template<typename T>
T DBManager::get_value(string sql_query) {
    if (conn_ == nullptr || !conn_->is_open()) {
        cerr << "Not connected to database!" << endl;
        return T();
    }

    try {
        work txn(*conn_);
        result r = txn.exec(sql_query);
        txn.commit();

        if (r.size() == 0 || r[0].size() == 0) {
            cerr << "No value found for SQL query: " << sql_query << endl;
            return T();
        }

        T value;
        from_string(r[0][0].c_str(), value);
        return value;
    }
    catch (const exception& e) {
        cerr << "Error executing SQL query: " << e.what() << endl;
        return T();
    }
}

template<typename T>
vector<T> DBManager::get_values(string sql_query) {
    if (conn_ == nullptr || !conn_->is_open()) {
        cerr << "Not connected to database!" << endl;
        return vector<T>();
    }

    try {
        work txn(*conn_);
        result r = txn.exec(sql_query);
        txn.commit();

        vector<T> values;
        for (auto row : r) {
            for (auto col : row) {
                T value;
                from_string(col.c_str(), value);
                values.push_back(value);
            }
        }

        return values;
    }
    catch (const exception& e) {
        cerr << "Error executing SQL query: " << e.what() << endl;
        return vector<T>();
    }
}
