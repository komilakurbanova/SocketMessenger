#include "clients.h"
#include <netinet/in.h>

enum Status {
    Active, 
    Sleep
};

class TCPServer {
public:
    TCPServer();
    void ClientsLoop();
    void ConnectionsLoop();
    void Start();
private:
    Status Status_;
    Clients Clients_;
    int ServerSock_;
    struct sockaddr_in Address_;
};
