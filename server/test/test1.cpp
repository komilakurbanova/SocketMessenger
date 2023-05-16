#include "../communicator.h"
#include <boost/asio.hpp>


#include "../../protocol/data_protocol.h"

Communicator comm;

int main(int argc, char** argv) {
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket(io_context);
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));

    ProtocolPacket pp;
    pp.operationType = OperationType::ADD_USER;
    pp.operationData.user = User{.username=argv[1], .password_hash=argv[2], .password_salt=argv[3]};

    comm.SerializeAndSendPacket(pp, socket);
    
    ProtocolPacket pp2;
    pp2.operationType = OperationType::GET_ALL_USERS;
    comm.SerializeAndSendPacket(pp2, socket);
    ProtocolPacket pp3;
    comm.ReceiveAndDeserializePacket(pp3, socket);
    for (auto u: pp3.getAllUserNames()) {
        std::cout << u << ", ";
    }
    std::cout << "\n";

    return 0;
}