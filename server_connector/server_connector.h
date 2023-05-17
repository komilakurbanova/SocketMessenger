#pragma once

#include "../server/communicator.h"
#include "../protocol/data_protocol.h"

#include "../lib/lib.h"
#include <exception>
#include <string>
#include <vector>

class ServerConnector {
public:
    ServerConnector(boost::asio::io_context* io_context, std::string ip = "127.0.0.1", int64_t port = 12345) : socket_(*io_context) {
        socket_.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port));
    };

    void AddUser(const User& user) {
        ProtocolPacket packet;
        packet.operationType = OperationType::ADD_USER;
        packet.operationData.user = user;
        communicator_.SerializeAndSendPacket(packet, socket_);
    }

    std::vector<User> GetAllUsers() {
        ProtocolPacket packet;
        packet.operationType = OperationType::GET_ALL_USERS;
        communicator_.SerializeAndSendPacket(packet, socket_);

        ProtocolPacket recv_packet;
        communicator_.ReceiveAndDeserializePacket(recv_packet, socket_);
        // if (recv_packet.operationType != OperationType::GET_ALL_USERS) {
        //     throw std::exception("error in GetAllUserNames");
        // } // TODO
        return recv_packet.getAllUsers();
    }

    void CreateChat(const std::string& first_username, const std::string& second_username, const std::string& chat_name) {
        ProtocolPacket packet;
        packet.operationType = OperationType::CREATE_CHAT;
        packet.operationData.firstChatMember = first_username;
        packet.operationData.secondChatMember = second_username;
        packet.operationData.newChatName = chat_name;
        communicator_.SerializeAndSendPacket(packet, socket_);
    }

    std::vector<Chat> GetAllChats(const User& user) {
        ProtocolPacket packet;
        packet.operationType = OperationType::GET_ALL_CHATS;
        packet.operationData.user = user;
        communicator_.SerializeAndSendPacket(packet, socket_);

        ProtocolPacket recv_packet;
        communicator_.ReceiveAndDeserializePacket(recv_packet, socket_);
        // TODO ошибки
        return recv_packet.getAllChats();
    }

    void PushMessage(Message);

private:
    boost::asio::ip::tcp::socket socket_;
    Communicator communicator_;
};
