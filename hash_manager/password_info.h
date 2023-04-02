#include <string>

class PasswordInfo {
    std::string hash;
    std::string salt;
public:
    PasswordInfo (std::string h, std::string s);
    std::string getHash() const;
    std::string getSalt() const;
};