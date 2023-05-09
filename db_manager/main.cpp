#include "db_manager.h"
#include <iostream>
#include <ostream>

int main() {
    const std::string connection_string = "dbname=messenger_db user=admin password=root host=db";
    LocalDBManager db_manager;

    std::string username1 = "<username>";
    std::string name1 = "Loki";
    std::string password_hash1 = "password_hash";
    std::string password_salt1 = "password_salt";
    if (db_manager.addUser(username1, name1, password_hash1, password_salt1)) {
        std::cout << "User 1 added successfully." << std::endl;
    } else {
        std::cout << "Error: Failed to add user 1." << std::endl;
    }

    std::string username2 = "testuser2";
    std::string name2 = "Komila";
    std::string password_hash2 = "passwordhash2";
    std::string password_salt2 = "salt2";
    if (db_manager.addUser(username2, name2, password_hash2, password_salt2)) {
        std::cout << "User 2 added successfully." << std::endl;
    } else {
        std::cout << "Error: Failed to add user 2." << std::endl;
    }

    std::string retrieved_hash = db_manager.getPasswordHash(username1);
    std::string retrieved_salt = db_manager.getPasswordSalt(username1);
    std::string retrieved_name = db_manager.getName(username1);
    if (retrieved_hash == password_hash1 &&
        retrieved_salt == password_salt1 &&
        retrieved_name == name1) {
        std::cout << "Succesfully retrieved hash, salt, name" << std::endl;
    } else {
        std::cout << "Error: Retrieved hash, salt, name does not match expected value" << std::endl;
    }

    std::string chat_name = "Test Chat";
    std::string chat_id = db_manager.createChat(username1, username2, chat_name);
    if (chat_id != "") {
        std::cout << "Chat created successfully." << std::endl;
    } else {
        std::cout << "Error: Failed to create chat." << std::endl;
    }

    std::vector<std::string> chats = db_manager.getChatIdsByUsername(username1);
    if (chats.size() != 1 || chats[0] != chat_id) {
        std::cout << "Error: user chats do not match expected value" << std::endl;
    } else {
        std::cout << "Succesfully got chats" << std::endl;
    }

    std::vector<std::string> user_list = db_manager.getUsernamesList();
    for (const auto& name : user_list) {
        std::cout << "Username: " << name << std::endl;
    }

    std::vector<std::string> chat_list = db_manager.getChatIdList();
    for (const auto& id : chat_list) {
        std::cout << "ChatID: " << id << std::endl;
    }

    std::set<std::string> chat_members = db_manager.getChatMembers(chat_id);
    for (const auto& member : chat_members) {
        std::cout << "Member of chat " << chat_id << " " << member << std::endl;
    }

    std::string sender_name = username1;
    std::string content = "Test message content";
    std::string timestamp = "2023-05-08 12:00:00";
    if (db_manager.addMessage(chat_id, sender_name, content, timestamp)) {
        std::cout << "Message added successfully." << std::endl;
    } else {
        std::cout << "Error: Failed to add message." << std::endl;
    }

    std::vector<std::string> chat_messages = db_manager.getChatMessages(chat_id);
    for (const auto& message : chat_messages) {
        std::cout << "Message in chat " << message << std::endl;
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
