#include <iostream>
#include <openssl/sha.h>
#include <cstring>
#include <string>
#include <random>
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

std::pair<std::string, std::string> PasswordHashWithSalt(const std::string &password) {
    try {
        const std::string &salt = GenerateRandomSalt(16);
        std::string saltedPassword = salt + password;
        unsigned char hash[SHA256_DIGEST_LENGTH]; // Хеш-значение будет сохранено здесь
        SHA256((unsigned char *) saltedPassword.c_str(), saltedPassword.length(), hash);
        std::string hashedPassword = "";
        char buf[3];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            snprintf(buf, 3, "%02x",
                     hash[i]); // Конвертируем каждый байт хеша в двухсимвольную строку шестнадцатеричной цифры
            hashedPassword += buf;
        }
        return std::make_pair(hashedPassword, salt);
    }
    catch (const std::exception &ex) {
        throw std::runtime_error("Error hashing password: " + std::string(ex.what()));
    }
}

bool CheckPassword(const std::string &password, const std::string &storedHash, const std::string &salt) {
    try {
        std::string hash = PasswordHashWithSalt(password).first;
        return hash == storedHash;
    }
    catch (const std::exception &ex) {
        throw std::runtime_error("Error checking password: " + std::string(ex.what()));
    }
}

int main() {
    try {
        std::string password = "MyPassword123";
        std::pair<std::string, std::string> result = PasswordHashWithSalt(password);
        std::string hash = result.first;
        std::string salt = result.second;
        std::cout << "Salt: " << salt << std::endl;
        std::cout << "Hashed password: " << hash << std::endl;
        return 0;
    }
    catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
}
