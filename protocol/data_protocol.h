#pragma once
#include "../lib/lib.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/access.hpp>
#include <vector>

enum class OperationType {
    ADD_USER,
    REMOVE_USER,
    CREATE_CHAT,
    REMOVE_CHAT,
    ADD_MESSAGE,
    GET_USER,
    GET_USER_CHAT_IDS,
    GET_ALL_USERS,
    GET_ALL_CHATS,
    GET_CHAT_NAME,
    GET_CHAT_MEMBERS,
    GET_CHAT_MESSAGES
};

struct OperationData {
    User user;
    Chat chat;
    Message message;
    std::vector<Message> allMessages;
    std::vector<Chat> allChats;
    std::vector<User> allUsers;
    std::string firstChatMember;
    std::string secondChatMember;
    std::string newChatName;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & user;
        ar & chat;
        ar & message;
        ar & allMessages;
        ar & allChats;
        ar & allUsers;
        ar & firstChatMember;
        ar & secondChatMember;
        ar & newChatName;
    }
};

struct ProtocolPacket {
    OperationType operationType;
    OperationData operationData;

    Message getMessage() const;
    User getUser() const;
    Chat getChat() const;
    std::vector<Message> getAllMessages() const;
    std::vector<Chat> getAllChats() const;
    std::vector<User> getAllUsers() const;
    std::string getFirstChatMember() const;
    std::string getSecondChatMember() const;
    std::string getNewChatName() const;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & operationType;
        ar & operationData;
    }
};