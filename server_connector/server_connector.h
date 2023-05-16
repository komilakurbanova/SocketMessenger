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

    // void Connect() {
    //     socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));
    // }

    void AddUser(User& user) {
        ProtocolPacket packet;
        packet.operationType = OperationType::ADD_USER;
        packet.operationData.user = user;
        communicator_.SerializeAndSendPacket(packet, socket_);
    }

    std::vector<std::string> GetAllUserNames() {
        ProtocolPacket packet;
        packet.operationType = OperationType::GET_ALL_USERS;
        communicator_.SerializeAndSendPacket(packet, socket_);

        ProtocolPacket recv_packet;
        communicator_.ReceiveAndDeserializePacket(recv_packet, socket_);
        // if (recv_packet.operationType != OperationType::GET_ALL_USERS) {
        //     throw std::exception("error in GetAllUserNames");
        // } // TODO
        return recv_packet.getAllUserNames();
    }

    void PushMessage(Message);

private:
    boost::asio::ip::tcp::socket socket_;
    Communicator communicator_;
};
