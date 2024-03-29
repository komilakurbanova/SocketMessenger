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
        MainCommunicator.ReceiveAndDeserializePacket(pp, socket);
        ProtocolPacket answer;
        switch (pp.operationType) {
            case OperationType::ADD_USER:
                ServerMutex.lock();
                if (LocalDB.isUserAdded(pp.getUser().username)) {
                    std::cout << "[INFO] User was already added " << pp.getUser().username << std::endl;
                    answer.operationData.user = User();
                } else {
                    std::cout << "[INFO] Added new user with username " << pp.getUser().username << std::endl;
                    LocalDB.addUser(pp.getUser().username,
                                    pp.getUser().name,
                                    pp.getUser().password_hash,
                                    pp.getUser().password_salt);
                    answer.operationData.user = LocalDB.getUser(pp.getUser().username);
                }

                MainCommunicator.SerializeAndSendPacket(answer, socket);
                ServerMutex.unlock();
                break;
            case OperationType::REMOVE_USER:
                LocalDB.removeUser(pp.getUser().username);
                break;
            case OperationType::CREATE_CHAT:
                ServerMutex.lock();
                {
                    std::string chat_id = LocalDB.createChat(pp.getFirstChatMember(), pp.getSecondChatMember(), pp.getNewChatName());
                    answer.operationData.newChatId = chat_id;
                    MainCommunicator.SerializeAndSendPacket(answer, socket);
                }
                ServerMutex.unlock();
                break;
            case OperationType::REMOVE_CHAT:
                LocalDB.removeChat(pp.getChat().chat_id);
                break;
            case OperationType::ADD_MESSAGE:
                LocalDB.addMessage(pp.getMessage().chat_id, pp.getMessage().sender_username, pp.getMessage().content);
                break;
            case OperationType::GET_USER:
                ServerMutex.lock();
                answer.operationData.user = LocalDB.getUser(pp.getUser().username);
                MainCommunicator.SerializeAndSendPacket(answer, socket);
                ServerMutex.unlock();
                break;
            case OperationType::GET_USER_CHAT_IDS:
                // TODO: vupilit'
                break;
            case OperationType::GET_ALL_USERS:
                ServerMutex.lock();
                answer.operationData.allUsers = LocalDB.getAllUsers();
                answer.operationType = OperationType::GET_ALL_USERS;
                MainCommunicator.SerializeAndSendPacket(answer, socket);
                ServerMutex.unlock();
                break;
            case OperationType::GET_ALL_CHATS:
                ServerMutex.lock();
                answer.operationData.allChats = LocalDB.getChatsByUsername(pp.getUser().username);
                answer.operationType = OperationType::GET_ALL_CHATS;
                MainCommunicator.SerializeAndSendPacket(answer, socket);
                ServerMutex.unlock();
                break;
            case OperationType::GET_CHAT_MESSAGES:
                ServerMutex.lock();
                answer.operationData.allMessages = LocalDB.getChatMessages(pp.getChat().chat_id);
                answer.operationType = OperationType::GET_CHAT_MESSAGES;
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