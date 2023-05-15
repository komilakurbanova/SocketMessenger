#include <string>

struct User {
    unsigned long long UserId;
    std::string Username;
    std::string PasswordHash;
    std::string PasswordSalt;
};