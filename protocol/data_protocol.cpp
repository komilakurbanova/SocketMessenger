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

std::vector<std::string> ProtocolPacket::getAllUserNames() const {
    return this->operationData.allUserNames;
}

// TODO mojno perepisat'

std::string ProtocolPacket::getFirstChatMember() const {
    return this->operationData.firstChatMember;
}

std::string ProtocolPacket::getSecondChatMember() const {
    return this->operationData.secondChatMember;
}

std::string ProtocolPacket::getNewChatName() const {
    return this->operationData.newChatName;
}
