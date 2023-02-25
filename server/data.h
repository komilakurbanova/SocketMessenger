#include <iostream>
#include "clients.h"

class Data {
public:
    ~Data() = default;
    const Client& GetClient() const {
        return client;
    } 
private:
    Client client;
};

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Data>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Data>& obj) {
    return As<T>(obj) != nullptr;
}

class Message : public Data {
public:
    std::string_view GetContent() const {
        return Content;
    }
private:
    std::string Content;
};

class Connect : public Data {
public:
private:
};

class Disconnect : public Data {
public:
private:
};