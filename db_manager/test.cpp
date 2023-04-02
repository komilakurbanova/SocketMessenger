#include <iostream>
#include "db_manager.h"

using namespace std;

int main() {
    string dbname = "mydatabase";
    string user = "myuser";
    string password = "mypassword";
    DBManager db(dbname, user, password);

    // Connect to database
    if (!db.connect()) {
        cerr << "Failed to connect to database!" << endl;
        return 1;
    }

    // Create a test table
    string create_table_query = "CREATE TABLE IF NOT EXISTS test_table (id SERIAL PRIMARY KEY, name TEXT, age INTEGER)";
    if (!db.execute(create_table_query)) {
        cerr << "Failed to create test_table!" << endl;
        return 1;
    }

    // Insert some test data
    string insert_query = "INSERT INTO test_table (name, age) VALUES ('John', 30), ('Jane', 25)";
    if (!db.execute(insert_query)) {
        cerr << "Failed to insert test data!" << endl;
        return 1;
    }

    // Retrieve data as vector of integers
    string select_age_query = "SELECT age FROM test_table";
    vector<int> ages = db.get_values<int>(select_age_query);
    if (ages.empty()) {
        cerr << "Failed to retrieve ages!" << endl;
        return 1;
    }
    cout << "Ages: ";
    for (int age : ages) {
        cout << age << " ";
    }
    cout << endl;

    // Retrieve data as vector of strings
    string select_name_query = "SELECT name FROM test_table";
    vector<string> names = db.get_values<string>(select_name_query);
    if (names.empty()) {
        cerr << "Failed to retrieve names!" << endl;
        return 1;
    }
    cout << "Names: ";
    for (string name : names) {
        cout << name << " ";
    }
    cout << endl;

    // Disconnect from database
    db.disconnect();

    return 0;
}
