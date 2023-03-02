#include <iostream>
#include <openssl/sha.h>
#include <cstring>
#include <string>
#include <random>
#include <cassert>
#include <stdexcept>

std::string GenerateRandomSalt(int length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);
    std::string salt;
    salt.resize(length);
    for (int i = 0; i < length; i++) {
        salt[i] = static_cast<char>(distrib(gen));
    }
    return salt;
}

std::string PasswordHashWithSalt(const std::string& password, const std::string& salt) {
    if (!password.length()) {
        throw std::runtime_error("Error hashing password: Empty password");
    }
    std::string tmp_password = salt + password;
    unsigned char hash[SHA256_DIGEST_LENGTH]; // The hash value will be stored here
    SHA256((unsigned char *) tmp_password.c_str(), tmp_password.length(), hash);
    std::string hashed_password = "";
    char buf[3];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        snprintf(buf, 3, "%02x",
                    hash[i]); // Convert each byte of the hash into a two-character hex string
        hashed_password += buf;
    }
    return hashed_password;
}

std::pair<std::string, std::string> HashPassword(const std::string &password) {
    try {
        int salt_length = 16;
        const std::string &salt = GenerateRandomSalt(salt_length);
        std::string hash = PasswordHashWithSalt(password, salt);
        return std::make_pair(hash, salt);
    }
    catch (const std::exception &ex) {
        throw std::runtime_error("Error hashing password: " + std::string(ex.what()));
    }
}

bool CheckPassword(const std::string &password, const std::string &stored_hash, const std::string &salt) {
    try {
        std::string hash = PasswordHashWithSalt(password, salt);
        return hash == stored_hash;
    }
    catch (const std::exception &ex) {
        throw std::runtime_error("Error checking password: " + std::string(ex.what()));
    }
}

