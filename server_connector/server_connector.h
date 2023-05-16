#pragma once

#include "../server/communicator.h"
#include "../protocol/data_protocol.h"

#include "../lib/lib.h"

class ServerConnector {
public:
    ServerConnector() {
    }; 

    void Connect() {
        socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));
    }

    void AddUser() {
        ProtocolPacket packet;
        communicator.SerializeAndSendPacket(packet, socket);
    }

    void PushMessage(Message);
private:
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket;
    Communicator communicator;
};
