#pragma once

#include <string>
#include <vector>

#include "../lib/lib.h"

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
    GET_ALL_CHAT_MESSAGES
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
        || operationType == OperationType::GET_ALL_CHAT_MESSAGES;
}

bool IsMessageOperation(OperationType operationType) {
    return operationType == OperationType::ADD_MESSAGE;
}

struct OperationData {
    User user;
    Chat chat;
    Message message;
    std::vector<Message> allMessages;
    std::vector<Chat> allChats;
};

struct ProtocolPacket {
    OperationType operationType;
    OperationData operationData;

    Message getMessage() const;
    User getUser() const;
    Chat getChat() const;
    std::vector<Message> getAllMessages() const;
    std::vector<Chat> getAllChats() const;
};
