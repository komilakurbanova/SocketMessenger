#include "password_info.h"

PasswordInfo::PasswordInfo (std::string h, std::string s) : hash(h), salt(s) {}

std::string PasswordInfo::getHash() const {
    return hash;
}

std::string PasswordInfo::getSalt() const {
    return salt;
}
