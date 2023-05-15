#include <string>

struct User {
    std::string username;
    std::string name;
    std::string password_hash;
    std::string password_salt;
};

struct Message {
    std::string sender_name;
    std::string content;
    std::string timestamp;
};

struct Chat {
    std::string chat_id;
    std::string chat_name;
    std::set<std::string> members;
    std::vector<Message> messages;
};

