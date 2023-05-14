#include <atomic>
#include <string>

enum class OperationType {
    ADD_USER,
    REMOVE_USER,
    CREATE_CHAT,
    REMOVE_CHAT,
    ADD_MESSAGE,
    GET_USER_PASSWORD_HASH,
    GET_USER_PASSWORD_SALT,
    GET_USER_NAME,
    GET_USER_CHAT_IDS,
    GET_ALL_USERS,
    GET_ALL_CHATS,
    GET_CHAT_NAME,
    GET_CHAT_MEMBERS,
    GET_CHAT_MESSAGES
};

bool IsUserOperation(OperationType operationType) {
    return operationType == OperationType::ADD_USER
        || operationType == OperationType::REMOVE_USER
        || operationType == OperationType::GET_USER_PASSWORD_HASH
        || operationType == OperationType::GET_USER_PASSWORD_SALT
        || operationType == OperationType::GET_USER_NAME
        || operationType == OperationType::GET_USER_CHAT_IDS
        || operationType == OperationType::GET_ALL_USERS;
}

bool IsChatOperation(OperationType operationType) {
    return operationType == OperationType::CREATE_CHAT
        || operationType == OperationType::REMOVE_CHAT
        || operationType == OperationType::GET_ALL_CHATS
        || operationType == OperationType::GET_CHAT_NAME
        || operationType == OperationType::GET_CHAT_MEMBERS
        || operationType == OperationType::GET_CHAT_MESSAGES;
}

bool IsMessageOperation(OperationType operationType) {
    return operationType == OperationType::ADD_MESSAGE;
}

struct User {
    std::string username;
    std::string name;
    std::string password_hash;
    std::string password_salt;
};

struct Chat {
    std::string chat_id;
    std::string first_username;
    std::string second_username;
    std::string chat_name;
};

struct Message {
    std::string sender_name;
    std::string content;
    std::string timestamp;
};

struct OperationData {
    User user;
    Chat chat;
    Message message;
};

struct ProtocolPacket {
    std::atomic_uint64_t uniqProtoId;
    OperationType operationType;
    OperationData operationData;

    std::string getUsername() const;
    std::string getName() const;
    std::string getPasswordHash() const;
    std::string getPasswordSalt() const;
    std::string getChatId() const;
    std::string getFirstUsername() const;
    std::string getSecondUsername() const;
    std::string getChatName() const;
    std::string getSenderName() const;
    std::string getContent() const;
    std::string getTimestamp() const;
};
