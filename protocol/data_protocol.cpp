#include "data_protocol.h"

Message ProtocolPacket::getMessage() const {
    return this->operationData.message;
}

User ProtocolPacket::getUser() const {
    return this->operationData.user;
}

Chat ProtocolPacket::getChat() const {
    return this->operationData.chat;
}

std::vector<Message> ProtocolPacket::getAllMessages() const {
    return this->operationData.allMessages;
}

std::vector<Chat> ProtocolPacket::getAllChats() const {
    return this->operationData.allChats;
}
