#include "db_manager.h"
#include <stdexcept>
#include <string>
#include <vector>

// class DBManager : public IDBManager {
// public:
//     DBManager(const std::string& connection_string) {
//         try {
//             // Connect to the database
//             conn = std::make_unique<pqxx::connection>(connection_string);
//         } catch (const std::exception& e) {
//             throw std::runtime_error("Failed to connect to the database: " + std::string(e.what()));
//         }
//     }

//     void addUser(const std::string& username,
//                  const std::string& password_hash,
//                  const std::string& password_salt) override {
//         try {
//             // Execute the SQL statement to insert a user
//             pqxx::work txn(*conn);
//             txn.exec_params(
//                 "INSERT INTO users (username, password_hash, password_salt) VALUES ($1, $2, $3)",
//                 username,
//                 password_hash,
//                 password_salt
//             );
//             txn.commit();
//         } catch (const std::exception& e) {
//             throw std::runtime_error("Failed to add user: " + std::string(e.what()));
//         }
//     }

//     void removeUser(const std::string& username) override {
//         try {
//             // Execute the SQL statement to remove a user
//             pqxx::work txn(*conn);
//             txn.exec_params(
//                 "DELETE FROM users WHERE username = $1",
//                 username
//             );
//             txn.commit();
//         } catch (const std::exception& e) {
//             throw std::runtime_error("Failed to remove user: " + std::string(e.what()));
//         }
//     }

//     void createChat(const std::string& first_username,
//                     const std::string& second_username,
//                     const std::string& chat_name) override {
//         try {
//             // Execute the SQL statement to insert a chat
//             pqxx::work txn(*conn);
//             pqxx::result res = txn.exec_params(
//                 "INSERT INTO chats (chat_name) VALUES ($1) RETURNING chat_id",
//                 chat_name
//             );
//             int chat_id = res[0][0].as<int>();

//             // Insert chat members
//             txn.exec_params(
//                 "INSERT INTO chat_members (chat_id, username) VALUES ($1, $2), ($1, $3)",
//                 chat_id,
//                 first_username,
//                 second_username
//             );
//             txn.commit();
//         } catch (const std::exception& e) {
//             throw std::runtime_error("Failed to create chat: " + std::string(e.what()));
//         }
//     }

//     void removeChat(int chat_id) override {
//         try {
//             // Execute the SQL statement to remove a chat
//             pqxx::work txn(*conn);
//             txn.exec_params(
//                 "DELETE FROM chats WHERE chat_id = $1",
//                 chat_id
//             );
//             txn.commit();
//         } catch (const std::exception& e) {
//             throw std::runtime_error("Failed to remove chat: " + std::string(e.what()));
//         }
//     }

//     bool DBManager::addMessage(int chat_id,
//                                const std::string& sender_name,
//                                const std::string& content,
//                                const std::string& timestamp) {
//         pqxx::work txn(*db);
//         try {
//             // Get the sender_id from the users table
//             std::string get_sender_id = "SELECT id FROM users WHERE username = " + txn.quote(sender_name);
//             pqxx::result sender_result = txn.exec(get_sender_id);
//             if (sender_result.empty()) {
//                 std::cerr << "User " << sender_name << " does not exist." << std::endl;
//                 txn.abort();
//                 return false;
//             }
//             int sender_id = sender_result[0]["id"].as<int>();

//             // Get the receiver_id from the chat_users table
//             std::string get_receiver_id = "SELECT user_id FROM chat_users WHERE chat_id = " + txn.quote(chat_id) + " AND user_id <> " + txn.quote(sender_id);
//             pqxx::result receiver_result = txn.exec(get_receiver_id);
//             if (receiver_result.empty()) {
//                 std::cerr << "No receiver found for chat " << chat_id << "." << std::endl;
//                 txn.abort();
//                 return false;
//             }
//             int receiver_id = receiver_result[0]["user_id"].as<int>();

//             // Insert the message into the messages table
//             std::string insert_message = "INSERT INTO messages (chat_id, sender_id, receiver_id, content, timestamp) VALUES (" + txn.quote(chat_id) + ", " + txn.quote(sender_id) + ", " + txn.quote(receiver_id) + ", " + txn.quote(content) + ", " + txn.quote(timestamp) + ")";
//             txn.exec(insert_message);
//             txn.commit();
//             std::cout << "Message added successfully." << std::endl;
//             return true;
//         } catch (const std::exception &e) {
//             std::cerr << "Error adding message: " << e.what() << std::endl;
//             txn.abort();
//             return false;
//         }
//     }
// }

bool LocalDBManager::addUser(const std::string& username,
                             const std::string& name,
                             const std::string& password_hash,
                             const std::string& password_salt) {
    if (users_.contains(username)) {
        return false;
    }

    User new_user {
        .username = username,
        .name = name,
        .password_hash = password_hash,
        .password_salt = password_salt,
    };
    users_[username] = new_user;
    return true;
}

bool LocalDBManager::removeUser(const std::string& username) {
    if (!users_.contains(username)) {
        return false;
    }

    for (auto iter = chats_.begin(); iter != chats_.end(); ) {
        Chat& chat = iter->second;
        if (chat.members.find(username) == chat.members.end()) {
            iter = chats_.erase(iter);
        } else {
            ++iter;
        }
    }
    users_.erase(username);
    return true;
}

std::string LocalDBManager::createChat(const std::string& first_username,
                                const std::string& second_username,
                                const std::string& chat_name) {
    if (!users_.contains(first_username) || !users_.contains(second_username)) {
        return "";
    }

    Chat new_chat {
        .chat_id = std::to_string(++next_chat_id_),
        .chat_name = chat_name,
    };
    new_chat.members.insert(first_username);
    new_chat.members.insert(second_username);
    chats_[new_chat.chat_id] = new_chat;
    return new_chat.chat_id;
}

bool LocalDBManager::removeChat(const std::string& chat_id) {
    if (!chats_.contains(chat_id)) {
        return false;
    }

    chats_.erase(chat_id);
    return true;
}

bool LocalDBManager::addMessage(const std::string& chat_id,
                                const std::string& sender_name,
                                const std::string& content,
                                const std::string& timestamp) {
    if (!chats_.contains(chat_id)) {
        return false;
    }

    Chat& chat = chats_[chat_id];
    if (!chat.members.contains(sender_name)) {
        return false;
    }

    chat.messages.push_back(content);
    return true;
}

std::string LocalDBManager::getPasswordHash(const std::string& username) const {
    if (users_.contains(username)) {
        return users_.at(username).password_hash;
    }
    return "";
}

std::string LocalDBManager::getPasswordSalt(const std::string& username) const {
    if (users_.contains(username)) {
        return users_.at(username).password_salt;
    }
    return "";
}

std::string LocalDBManager::getName(const std::string& username) const {
    if (users_.contains(username)) {
        return users_.at(username).name;
    }
    return "";
}

std::vector<std::string> LocalDBManager::getChatIdsByUsername(const std::string& username) const {
    std::vector<std::string> chat_ids;
    for (const auto& [chat_id, chat] : chats_) {
        if (chat.members.contains(username)) {
            chat_ids.push_back(chat.chat_id);
        }
    }
    return chat_ids;
}

std::vector<std::string> LocalDBManager::getUsernamesList() const {
    std::vector<std::string> user_list;
    for (const auto& [username, user] : users_) {
        user_list.emplace_back(username);
    }
    return user_list;
}

std::vector<std::string> LocalDBManager::getChatIdList() const {
    std::vector<std::string> chat_list;
    for (const auto& [chat_id, chat] : chats_) {
        chat_list.emplace_back(chat_id);
    }
    return chat_list;
}

std::string LocalDBManager::getChatName(const std::string& chat_id) const {
    if (chats_.contains(chat_id)) {
        return chats_.at(chat_id).chat_name;
    }
    return "";
}

std::set<std::string> LocalDBManager::getChatMembers(const std::string& chat_id) const {
    if (!chats_.contains(chat_id)) {
        return {};
    }
    std::set<std::string> members;
    for (const auto& member : chats_.at(chat_id).members) {
        members.insert(member);
    }
    return members;
}

std::vector<std::string> LocalDBManager::getChatMessages(const std::string& chat_id) const {
    if (!chats_.contains(chat_id)) {
        return {};
    }
    std::vector<std::string> messages;
    for (const auto& message : chats_.at(chat_id).messages) {
        messages.emplace_back(message);
    }
    return messages;
}