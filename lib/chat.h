#include <string>
#include <vector>

#include "message.h"

struct Chat {
    std::string ChatName;
    unsigned long long ChatId;
    std::vector<std::string> Members;
    std::vector<Message> messages;
};
