#include <boost/asio.hpp>
#include <thread>
#include "../protocol/data_protocol.h"
#include "communicator.h"
#include "../db_manager/db_manager.h"
// #include "protocol.h"
#include <mutex>
#include <thread>

Communicator MainCommunicator;
LocalDBManager LocalDB;
std::mutex ServerMutex;

void ListenAndServe(boost::asio::ip::tcp::socket socket) {
    while (true) {
        ProtocolPacket pp;
        // std::cout << "abobka" << std::endl;
        MainCommunicator.ReceiveAndDeserializePacket(pp, socket);
        // std::cout << "abobka" << std::endl;
        ProtocolPacket answer;
        switch (pp.operationType) {
            case OperationType::ADD_USER:
                std::cout << "[INFO] Added new user with username " << pp.getUser().username << std::endl;
                LocalDB.addUser(pp.getUser().username, pp.getUser().name, pp.getUser().password_hash, pp.getUser().password_salt);
                break;
            case OperationType::REMOVE_USER:
                LocalDB.removeUser(pp.getUser().username);
                break;
            case OperationType::CREATE_CHAT:
                LocalDB.createChat(pp.getFirstChatMember(), pp.getSecondChatMember(), pp.getNewChatName());
                break;
            case OperationType::REMOVE_CHAT:
                LocalDB.removeChat(pp.getChat().chat_id);
                break;
            case OperationType::ADD_MESSAGE:
                LocalDB.addMessage(pp.getMessage().chat_id, pp.getMessage().sender_username, pp.getMessage().content);
                break;
            case OperationType::GET_USER_PASSWORD_HASH:
                // TODO
                break;
            case OperationType::GET_USER_PASSWORD_SALT:
                // TODO
                break;
            case OperationType::GET_USER_NAME:
                // TODO: vupilit'
                break;
            case OperationType::GET_USER_CHAT_IDS:
                // TODO: vupilit'
                break;
            case OperationType::GET_ALL_USERS:
                answer.operationData.allUserNames = LocalDB.getUsernamesList();
                answer.operationType = OperationType::GET_ALL_USERS;
                ServerMutex.lock();
                MainCommunicator.SerializeAndSendPacket(answer, socket);
                ServerMutex.unlock();
                break;
            case OperationType::GET_ALL_CHATS:
                answer.operationData.allChats = LocalDB.getChatsByUsername(pp.getUser().username);
                answer.operationType = OperationType::GET_ALL_CHATS;
                ServerMutex.lock();
                MainCommunicator.SerializeAndSendPacket(answer, socket);
                ServerMutex.unlock();
                break;
            case OperationType::GET_CHAT_NAME:
                /// TODO
                break;
            case OperationType::GET_CHAT_MEMBERS:
                // TODO
                break;
            case OperationType::GET_CHAT_MESSAGES:
                answer.operationData.allMessages = LocalDB.getChatMessages(pp.getChat().chat_id);
                answer.operationType = OperationType::GET_CHAT_MESSAGES;
                ServerMutex.lock();
                MainCommunicator.SerializeAndSendPacket(answer, socket);
                ServerMutex.unlock();
                break;
        }
    }
}

int main(int argc, char* argv[])
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345));

    std::vector <std::thread> amogus;

    for (;;) {
        boost::asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "accepted" << std::endl;

        amogus.emplace_back(ListenAndServe, std::move(socket));
    }

    for (auto &u: amogus) {
        u.join();
    }
    return 0;
}