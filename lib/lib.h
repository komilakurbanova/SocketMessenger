#pragma once

#include <string>
#include <set>
#include <vector>

struct User {
    std::string username;
    std::string name;
    std::string password_hash;
    std::string password_salt;
};

struct Message {
    std::string sender_username;
    std::string chat_id;
    std::string content;
};

struct Chat {
    std::string chat_id;
    std::string chat_name;
    std::set<std::string> members;  // set<usernames>
    std::vector<Message> messages;
};

