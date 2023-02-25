#include <exception>
#include <mutex>
#include <list>

class Client {
public:
    Client(uint64_t id, int socket) : Id(id), Socket(socket) {}
    uint64_t Id;
    int Socket;
};

inline bool operator==(const Client& a, const Client& b) {
    return a.Id == b.Id;
} 

class Clients {
public:
    void AddClient(const Client& client) {
        ClientsLock.lock();
        data.push_back(client);
        ClientsLock.unlock();
    }
    
    void RemoveClient(const Client& client) { // mama ima cringe (когда-то кто-то перепишет 200%)
        ClientsLock.lock(); 
        auto it = std::find(data.begin(), data.end(), client);
        if (it == data.end()) {
            throw std::runtime_error("No such client with id " + std::to_string(client.Id));
        }
        data.erase(it);
        ClientsLock.unlock(); 
    }
    
    std::mutex ClientsLock; // сейчас так, потом надо на что-то адекватное переписать
    std::list <Client> data;
private:
};