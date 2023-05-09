#include <map>
// #include <pqxx/pqxx>
#include <string>
#include <vector>
#include <set>
#include <iostream>

class IDBManager {
public:
    virtual bool addUser(const std::string& username, const std::string& name, const std::string& password_hash, const std::string& password_salt) = 0;
    virtual bool removeUser(const std::string& username) = 0;
    virtual bool createChat(const std::string& first_username, const std::string& second_username, const std::string& chat_name) = 0;
    virtual bool removeChat(const std::string& chat_id) = 0;
    virtual bool addMessage(const std::string& chat_id, const std::string& sender_name, const std::string& content, const std::string& timestamp) = 0;

    virtual std::string getPasswordHash(const std::string& username) const = 0;
    virtual std::string getPasswordSalt(const std::string& username) const = 0;
    virtual std::string getName(const std::string& username) const = 0;
    virtual std::vector<std::string> getChatsByUsername(const std::string& username) const = 0;
    virtual std::vector<std::pair<std::string, std::string>> getUserList() const = 0;
    virtual std::vector<std::string> getMessagesByChat(const std::string& chat_id) const = 0;
};

// class DBManager : public IDBManager {
// public:
//     DBManager(const std::string& connection_string);
//     ~DBManager();

//     bool addUser(const std::string& username, const std::string& password_hash, const std::string& password_salt) override;
//     bool removeUser(const std::string& username) override;
//     bool createChat(const std::string& first_username, const std::string& second_username, const std::string& chat_name) override;
//     bool removeChat(const std::string& chat_id) override;
//     bool addMessage(const std::string& chat_id, const std::string& sender_name, const std::string& content, const std::string& timestamp) override;

// private:
//     pqxx::connection connection_;
// };

struct User {
    std::string username;
    std::string name;
    std::string password_hash;
    std::string password_salt;
};

struct Chat {
    std::string chat_id;
    std::string chat_name;
    std::set<std::string> members;
    std::vector<std::string> messages;
};

class LocalDBManager : public IDBManager {
public:
    LocalDBManager() {};

    bool addUser(const std::string& username, const std::string& name, const std::string& password_hash, const std::string& password_salt) override;
    bool removeUser(const std::string& username) override;
    bool createChat(const std::string& first_username, const std::string& second_username, const std::string& chat_name) override;
    bool removeChat(const std::string& chat_id) override;
    bool addMessage(const std::string& chat_id, const std::string& sender_name, const std::string& content, const std::string& timestamp) override;

    std::string getPasswordHash(const std::string& username) const override;
    std::string getPasswordSalt(const std::string& username) const override;
    std::string getName(const std::string& username) const override;
    std::vector<std::string> getChatsByUsername(const std::string& username) const override;
    std::vector<std::pair<std::string, std::string>> getUserList() const override;
    std::vector<std::string> getMessagesByChat(const std::string& chat_id) const override;

protected:
    int next_chat_id_ = 1;
    std::map<std::string, User> users_;
    std::map<std::string, Chat> chats_;
};
