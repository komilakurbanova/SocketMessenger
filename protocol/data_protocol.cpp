#include "data_protocol.h"

std::string ProtocolPacket::getUsername() const {
    if (operationType != OperationType::ADD_USER && operationType != OperationType::REMOVE_USER) {
        throw std::runtime_error("Invalid operation type for getUsername");
    }
    return operationData.user.username;
}

std::string ProtocolPacket::getName() const {
    if (operationType != OperationType::ADD_USER) {
        throw std::runtime_error("Invalid operation type for getName");
    }
    return operationData.user.name;
}

std::string ProtocolPacket::getPasswordHash() const {
    if (operationType != OperationType::ADD_USER) {
        throw std::runtime_error("Invalid operation type for getPasswordHash");
    }
    return operationData.user.password_hash;
}

std::string ProtocolPacket::getPasswordSalt() const {
    if (operationType != OperationType::ADD_USER) {
        throw std::runtime_error("Invalid operation type for getPasswordSalt");
    }
    return operationData.user.password_salt;
}

std::string ProtocolPacket::getChatId() const {
    if (operationType != OperationType::CREATE_CHAT && operationType != OperationType::REMOVE_CHAT && operationType != OperationType::ADD_MESSAGE) {
        throw std::runtime_error("Invalid operation type for getChatId");
    }
    return operationData.chat.chat_id;
}

std::string ProtocolPacket::getFirstUsername() const {
    if (operationType != OperationType::CREATE_CHAT) {
        throw std::runtime_error("Invalid operation type for getFirstUsername");
    }
    return operationData.chat.first_username;
}

std::string ProtocolPacket::getSecondUsername() const {
    if (operationType != OperationType::CREATE_CHAT) {
        throw std::runtime_error("Invalid operation type for getSecondUsername");
    }
    return operationData.chat.second_username;
}

std::string ProtocolPacket::getChatName() const {
    if (operationType != OperationType::CREATE_CHAT) {
        throw std::runtime_error("Invalid operation type for getChatName");
    }
    return operationData.chat.chat_name;
}

std::string ProtocolPacket::getSenderName() const {
    if (operationType != OperationType::ADD_MESSAGE) {
        throw std::runtime_error("Invalid operation type for getSenderName");
    }
    return operationData.message.sender_name;
}

std::string ProtocolPacket::getContent() const {
    if (operationType != OperationType::ADD_MESSAGE) {
        throw std::runtime_error("Invalid operation type for getContent");
    }
    return operationData.message.content;
}

std::string ProtocolPacket::getTimestamp() const {
    if (operationType != OperationType::ADD_MESSAGE) {
        throw std::runtime_error("Invalid operation type for getTimestamp");
    }
    return operationData.message.timestamp;
}
