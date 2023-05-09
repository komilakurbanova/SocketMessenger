#include "db_manager.h"
#include <iostream>

int main() {
    const std::string connection_string = "dbname=messenger_db user=admin password=root host=db";
    LocalDBManager db_manager;

    std::string username1 = "<username>";
    std::string password_hash1 = "password_hash";
    std::string password_salt1 = "password_salt";
    if (db_manager.addUser(username1, password_hash1, password_salt1)) {
        std::cout << "User 1 added successfully." << std::endl;
    } else {
        std::cout << "Error: Failed to add user 1." << std::endl;
    }

    std::string username2 = "testuser2";
    std::string password_hash2 = "passwordhash2";
    std::string password_salt2 = "salt2";
    if (db_manager.addUser(username2, password_hash2, password_salt2)) {
        std::cout << "User 2 added successfully." << std::endl;
    } else {
        std::cout << "Error: Failed to add user 2." << std::endl;
    }

    // Get the hash and salt for the user
    std::string retrieved_hash = db_manager.getPasswordHash(username1);
    std::string retrieved_salt = db_manager.getPasswordSalt(username1);
    if (retrieved_hash == password_hash1 || retrieved_salt == password_salt1) {
        std::cout << "Succesfully retrieved hash and salt" << std::endl;
    } else {
        std::cout << "Error: Retrieved hash or salt does not match expected value" << std::endl;
    }

    std::string chat_name = "Test Chat";
    if (db_manager.createChat(username1, username2, chat_name)) {
        std::cout << "Chat created successfully." << std::endl;
    } else {
        std::cout << "Error: Failed to create chat." << std::endl;
    }

    std::vector<std::string> chats = db_manager.getChatsByUsername(username1);
    if (chats.size() != 1 || chats[0] != "<chat_id>") {
        std::cout << "Succesfully got chats" << std::endl;
    } else {
        std::cout << "Error: user chats do not match expected value" << std::endl;
    }

    std::vector<std::string> user_list = db_manager.getUserList();
    for (const auto& name : user_list) {
        std::cout << "Username: " << name << std::endl;
    }

    std::string chat_id = "2";
    std::string sender_name = username1;
    std::string content = "Test message content";
    std::string timestamp = "2023-05-08 12:00:00";
    if (db_manager.addMessage(chat_id, sender_name, content, timestamp)) {
        std::cout << "Message added successfully." << std::endl;
    } else {
        std::cout << "Error: Failed to add message." << std::endl;
    }

    if (db_manager.removeUser(username1)) {
        std::cout << "User removed successfully." << std::endl;
    } else {
        std::cout << "Error: Failed to remove user." << std::endl;
    }

    if (db_manager.removeChat(chat_id)) {
        std::cout << "Chat removed successfully." << std::endl;
    } else {
        std::cout << "Error: Failed to remove chat." << std::endl;
    }

    return 0;
}
