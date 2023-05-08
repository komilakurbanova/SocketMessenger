#include "db_manager.h"
#include <iostream>
#include <string>

int main() {
    const std::string connection_string = "dbname=messenger_db user=admin password=root host=db";
    LocalDBManager db;

    std::string username1 = "testuser1";
    std::string password_hash1 = "passwordhash1";
    std::string password_salt1 = "salt1";
    if (db.addUser(username1, password_hash1, password_salt1)) {
        std::cout << "User 1 added successfully." << std::endl;
    } else {
        std::cout << "Failed to add user 1." << std::endl;
    }

    std::string username2 = "testuser2";
    std::string password_hash2 = "passwordhash2";
    std::string password_salt2 = "salt2";
    if (db.addUser(username2, password_hash2, password_salt2)) {
        std::cout << "User 2 added successfully." << std::endl;
    } else {
        std::cout << "Failed to add user 2." << std::endl;
    }

    std::string chat_name = "Test Chat";
    if (db.createChat(username1, username2, chat_name)) {
        std::cout << "Chat created successfully." << std::endl;
    } else {
        std::cout << "Failed to create chat." << std::endl;
    }

    std::string chat_id = "2";
    std::string sender_name = username1;
    std::string content = "Test message content";
    std::string timestamp = "2023-05-08 12:00:00";
    if (db.addMessage(chat_id, sender_name, content, timestamp)) {
        std::cout << "Message added successfully." << std::endl;
    } else {
        std::cout << "Failed to add message." << std::endl;
    }

    if (db.removeUser(username1)) {
        std::cout << "User removed successfully." << std::endl;
    } else {
        std::cout << "Failed to remove user." << std::endl;
    }

    if (db.removeChat(chat_id)) {
        std::cout << "Chat removed successfully." << std::endl;
    } else {
        std::cout << "Failed to remove chat." << std::endl;
    }

    return 0;
}
