#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>
#include <thread>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <random> 
#include <fcntl.h>

#include "server.h"
#include "data.h"

std::mt19937_64 rnd(42);

static const int PORT = 7777;
static const int MaxSyncConn = 5; // <= 5

TCPServer::TCPServer() {
    Status_ = Status::Sleep;
}

void TCPServer::ClientsLoop() {
    while (Status_ == Status::Active) {
        // std::cerr << "Clients Loop started\n";
        Clients_.ClientsLock.lock(); // oh no cringe
        for (auto u: Clients_.data) {
            // Хуйня для неблокирующего чтения
            int flags = fcntl(u.Socket, F_GETFL, 0);
            flags |= O_NONBLOCK;
            fcntl(u.Socket, F_SETFL, flags);

            // Ожидание данных
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            int bytes_received = recv(u.Socket, buffer, sizeof(buffer), 0);
            if (bytes_received == -1) {
                if (errno == EWOULDBLOCK) { // Данных от клиента нет
                    // ну прост ждем)
                } else { // Произошла непонятная
                    std::cerr << "[ERROR] Untracked error!\n";
                }
            } else if (bytes_received == 0)  { // Клиент отключился
                std::cout << "[Disconnect] Client " + std::to_string(u.Id) + " has disconnected\n";
                // чорт надо адекватно сделать))
                Clients_.ClientsLock.unlock();
                Clients_.RemoveClient(Client(u.Id, -1)); 
                Clients_.ClientsLock.lock();
            } else { // Клиент прислал данные
                std::cerr << "[Message] From " + std::to_string(u.Id) + ": " + buffer + "\n";
                // чета делать
            }
        }
        Clients_.ClientsLock.unlock();
        sleep(1);
    }
}

void TCPServer::ConnectionsLoop() {
    while (Status_ == Status::Active) {
        std::cerr << "Look for new connections\n";
        
        int ClientSock;
        struct sockaddr_in ClientAddr;
        int Addrlen = sizeof(ClientAddr);
        if ((ClientSock = accept(ServerSock_, (struct sockaddr*)&ClientAddr, (socklen_t*)&Addrlen)) < 0) {
            throw std::runtime_error("Can't add new connection");
        }
    
        uint64_t NewClientId = rnd();
        Clients_.AddClient(Client(NewClientId, ClientSock));

        std::cerr << "[Connect] Given Id: " + std::to_string(NewClientId) + ":" + std::to_string(ClientSock) + "\n";
    }
}

void TCPServer::Start() {
    if (Status_ == Status::Active) {
        return;
    }

    std::cerr << "Server starting...\n";

    Status_ = Status::Active;
    int Opt = 1;
    if ((ServerSock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		throw std::runtime_error("Socket initialization faled");
	}

    if (setsockopt(ServerSock_, SOL_SOCKET, SO_REUSEADDR, &Opt, sizeof(Opt))) {
		throw std::runtime_error("Setsockopt failed");
	}

    int Addrlen = sizeof(Address_);
    Address_.sin_family = AF_INET;
	Address_.sin_addr.s_addr = INADDR_ANY;
	Address_.sin_port = htons(PORT);

    if (bind(ServerSock_, (struct sockaddr*)&Address_, sizeof(Address_)) < 0) {
		throw std::runtime_error("Bind failed");
	}

    if (listen(ServerSock_, MaxSyncConn) < 0) {
		throw std::runtime_error("Listen failed");
	}

    std::cerr << "Server started\n";

    std::thread ConnLoop([this]() {
        return ConnectionsLoop();
    });

    std::thread CliLoop([this]() {
        return ClientsLoop();
    });
    
    ConnLoop.join();
    CliLoop.join();
}
