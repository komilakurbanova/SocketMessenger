#include <string>

class TDBManager {
protected:
    bool PathIsValid(const std::string& db_path);

public:
    TDBManager(const std::string& db_path);

    void PostData();
    void GetData();
};