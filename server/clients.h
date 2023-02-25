#include <exception>
#include <mutex>
#include <list>

class Client {
public:
    Client(uint64_t id) : Id(id) {}
    uint64_t Id;
};

class Clients {
public:
    void AddClient(Client& client) {
        ClientsLock.lock();
        data.push_back(client);
        ClientsLock.unlock();
    }
    
    void RemoveClient(Client& client) { // mama ima cringe (когда-то кто-то перепишет 200%)
        ClientsLock.lock(); 
        auto it = std::find(data.begin(), data.end(), client);
        if (it == data.end()) {
            throw std::runtime_error("No such client with id " + std::to_string(client.Id));
        }
        data.erase(it);
        ClientsLock.unlock(); 
    }
    
    std::mutex ClientsLock; // сейчас так, потом перепишу на свой лист с блокировкой одной ноды
private:
    std::list <Client> data;
};