#include <map>
// #include <pqxx/pqxx>
#include <string>
#include <vector>
#include <set>

class IDBManager {
public:
    virtual bool addUser(const std::string& username, const std::string& password_hash, const std::string& password_salt) = 0;
    virtual bool removeUser(const std::string& username) = 0;
    virtual bool createChat(const std::string& first_username, const std::string& second_username, const std::string& chat_name) = 0;
    virtual bool removeChat(const std::string& chat_id) = 0;
    virtual bool addMessage(const std::string& chat_id, const std::string& sender_name, const std::string& content, const std::string& timestamp) = 0;
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

    bool addUser(const std::string& username, const std::string& password_hash, const std::string& password_salt) override;
    bool removeUser(const std::string& username) override;
    bool createChat(const std::string& first_username, const std::string& second_username, const std::string& chat_name) override;
    bool removeChat(const std::string& chat_id) override;
    bool addMessage(const std::string& chat_id, const std::string& sender_name, const std::string& content, const std::string& timestamp) override;

private:
    int next_chat_id_ = 1;
    std::map<std::string, User> users_;
    std::map<std::string, Chat> chats_;
};
