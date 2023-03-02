#include "main.cpp"

int main() {
    // Test GenerateRandomSalt returns random salts
    int salt_length = 16;
    std::string salt1 = GenerateRandomSalt(salt_length);
    std::string salt2 = GenerateRandomSalt(salt_length);
    assert(salt1 != salt2);
    
    // Test that calling HashPassword twice with the same input password results in different salts and hashes
    std::string password = "MyPassword123";
    auto hash_salt_1 = HashPassword(password);
    auto hash_salt_2 = HashPassword(password);
    assert(hash_salt_1.first != hash_salt_2.first);
    assert(hash_salt_1.second != hash_salt_2.second);

    // Test that the output hash is different from the input password
    password = "my_password";
    auto hash_salt = HashPassword(password);
    assert(hash_salt.first != password);

    // Test PasswordHashWithSalt that changing the input salt results in a different output hash
    password = "my_password";
    hash_salt = HashPassword(password);
    std::string new_salt = GenerateRandomSalt(16);
    std::string new_hash = PasswordHashWithSalt(password, new_salt);
    assert(new_hash != hash_salt.first);

    // Test HashPassword that an exception is thrown when the input password is empty
    bool exception_thrown = false;
    try{
        HashPassword("");
        } catch(...) {
        exception_thrown = true;
    }
    assert(exception_thrown);

    // Test HashPassword function
    password = "MyPassword123";
    std::pair<std::string, std::string> result = HashPassword(password);
    std::string hash = result.first;
    std::string salt = result.second;
    assert(hash.length() == 64); // Check that the hash length is correct
    assert(salt.length() == 16); // Check that the salt length is correct
    assert(CheckPassword(password, hash, salt)); // Check that the password can be checked with the hash and salt

    // Test CheckPassword function with correct password
    password = "MyPassword123";
    hash = "7ec624e3f1f505a1d49e1f14b34b22a4382429b9e1f84ba9aae77b5f9e1bec56";
    salt = "abcdefghijklmnopqrstuvwxyz123456";
    assert(CheckPassword(password, hash, salt)); // Check that the correct password is accepted

    // Test CheckPassword function with incorrect password
    password = "WrongPassword";
    hash = "7ec624e3f1f505a1d49e1f14b34b22a4382429b9e1f84ba9aae77b5f9e1bec56";
    salt = "abcdefghijklmnopqrstuvwxyz123456";
    assert(!CheckPassword(password, hash, salt)); // Check that the incorrect password is rejected

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
