#pragma once

#include <map>
// #include <pqxx/pqxx>
#include <string>
#include <vector>
#include <set>
#include <iostream>

#include "../lib/lib.h"

class IDBManager {
public:
    virtual bool addUser(const std::string& username, const std::string& name,
                         const std::string& password_hash, const std::string& password_salt) = 0;
    virtual User getUser(const std::string& username) = 0;
    virtual bool removeUser(const std::string& username) = 0;
    virtual std::string createChat(const std::string& first_username,
                                   const std::string& second_username,
                                   const std::string& chat_name) = 0;
    virtual bool removeChat(const std::string& chat_id) = 0;
    virtual bool addMessage(const std::string& chat_id, const std::string& sender_username, const std::string& content) = 0;

    virtual std::string getPasswordHash(const std::string& username) const = 0;
    virtual std::string getPasswordSalt(const std::string& username) const = 0;
    virtual std::string getName(const std::string& username) const = 0;
    virtual std::vector<std::string> getChatIdsByUsername(const std::string& username) const = 0;
    virtual std::vector<Chat> getChatsByUsername(const std::string& username) const = 0;
    virtual std::vector<std::string> getUsernamesList() const = 0;

    virtual std::vector<std::string> getChatIdList() const = 0;
    virtual std::string getChatName(const std::string& chat_id) const = 0;
    virtual std::set<std::string> getChatMembers(const std::string& chat_id) const = 0;
    virtual std::vector<Message> getChatMessages(const std::string& chat_id) const = 0;

    virtual std::vector<User> getAllUsers() const = 0;
    virtual Chat getChat(const std::string& chat_name) = 0;
    virtual bool isUserAdded(const std::string& username) = 0;
};

// class DBManager : public IDBManager {
// public:
//     DBManager(const std::string& connection_string);
//     ~DBManager();

//     bool addUser(const std::string& username, const std::string& password_hash, const std::string& password_salt) override;
//     bool removeUser(const std::string& username) override;
//     bool createChat(const std::string& first_username, const std::string& second_username, const std::string& chat_name) override;
//     bool removeChat(const std::string& chat_id) override;
//     bool addMessage(const std::string& chat_id, const std::string& sender_username, const std::string& content) override;

// private:
//     pqxx::connection connection_;
// };

class LocalDBManager : public IDBManager {
public:
    LocalDBManager() {};

    bool addUser(const std::string& username, const std::string& name,
                 const std::string& password_hash, const std::string& password_salt) override;
    bool removeUser(const std::string& username) override;
    // returns chat_id
    std::string createChat(const std::string& first_username, const std::string& second_username, const std::string& chat_name) override;
    bool removeChat(const std::string& chat_id) override;
    bool addMessage(const std::string& chat_id, const std::string& sender_username, const std::string& content) override;

    std::string getPasswordHash(const std::string& username) const override;
    std::string getPasswordSalt(const std::string& username) const override;
    std::string getName(const std::string& username) const override;
    std::vector<std::string> getChatIdsByUsername(const std::string& username) const override;
    std::vector<Chat> getChatsByUsername(const std::string& username) const override;
    std::vector<std::string> getUsernamesList() const override;

    std::vector<std::string> getChatIdList() const override;
    std::string getChatName(const std::string& chat_id) const override;
    std::set<std::string> getChatMembers(const std::string& chat_id) const override;
    std::vector<Message> getChatMessages(const std::string& chat_id) const override;

    std::vector<User> getAllUsers() const override;
    User getUser(const std::string& username) override;
    Chat getChat(const std::string& chat_name) override;
    bool isUserAdded(const std::string& username) override;

protected:
    int next_chat_id_ = 1;
    std::map<std::string, User> users_;
    std::map<std::string, Chat> chats_;
};
