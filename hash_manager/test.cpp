#include "salt_hash.cpp"

int main() {
    // Test GenerateRandomSalt returns random salts
    int salt_length = 16;
    std::string salt1 = GenerateRandomSalt(salt_length);
    std::string salt2 = GenerateRandomSalt(salt_length);
    assert(salt1 != salt2);

    //Test that the salt only contains characters within the ASCII range of 0-255
    salt_length = 16;
    std::string salt = GenerateRandomSalt(salt_length);
    for (char c : salt) {
        assert(0 <= c);
        assert(c <= 255);
    }

    // Test that calling HashPassword twice with the same input password results in different salts and hashes
    std::string password = "MyPassword123";
    PasswordInfo password_info_1 = HashPassword(password);
    PasswordInfo password_info_2 = HashPassword(password);
    assert(password_info_1.getHash() != password_info_2.getHash());
    assert(password_info_1.getSalt() != password_info_2.getSalt());

    // Test that the output hash is different from the input password
    password = "my_password";
    PasswordInfo hash_salt = HashPassword(password);
    assert(hash_salt.getHash() != password);

    // Test PasswordHashWithSalt that changing the input salt results in a different output hash
    password = "my_password";
    hash_salt = HashPassword(password);
    std::string new_salt = GenerateRandomSalt(16);
    std::string new_hash = PasswordHashWithSalt(password, new_salt);
    assert(new_hash != hash_salt.getHash());

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
    PasswordInfo result = HashPassword(password);
    std::string hash = result.getHash();
    salt = result.getSalt();
    assert(hash.length() == 64); // Check that the hash length is correct
    assert(salt.length() == 16); // Check that the salt length is correct
    assert(CheckPassword(password, result)); // Check that the password can be checked with the hash and salt

    // Test CheckPassword function with correct password
    password = "MyPassword123";
    hash = "7ec624e3f1f505a1d49e1f14b34b22a4382429b9e1f84ba9aae77b5f9e1bec56";
    salt = "abcdefghijklmnopqrstuvwxyz123456";
    assert(CheckPassword(password, PasswordInfo(hash, salt))); // Check that the correct password is accepted

    // Test CheckPassword function with incorrect password
    password = "WrongPassword";
    hash = "7ec624e3f1f505a1d49e1f14b34b22a4382429b9e1f84ba9aae77b5f9e1bec56";
    salt = "abcdefghijklmnopqrstuvwxyz123456";
    assert(!CheckPassword(password, PasswordInfo(hash, salt))); // Check that the incorrect password is rejected

    std::cout << "All tests passed!" << std::endl;
    return 0;
}


// Use: g++ -Wall -o a.out password_info.cpp test.cpp -lssl -lcrypto to run tests
// ./a.out
