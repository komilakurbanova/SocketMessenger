#pragma once

#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <ctype.h>
#include "../db_manager/db_manager.h"
#include "../protocol/data_protocol.h"

enum Flags {
    ESC = -2,
    BACK = -1,
    KEY_BACK = 127,
    KEY_ESC = 27,
    LOGIN = 0,
    SIGNUP = 1,
    NEW = 2,
    OPEN = 3,
    DELETE = 4,
    BUFSIZE = 20, // максимальная длина пароля/логина/имени
    CHOOSE_CHAT = 0,
    NEW_CHAT = 1,
    MAXSIZE = 1024, // максимальная длина сообщения
    MAX_ROWS = 20, // максимальное число строк в чатах
};

const std::string connection_string = "dbname=messenger_db user=admin password=root host=db";
LocalDBManager db;

void index();
std::vector<std::string> signup();
std::vector<std::string> login();
void home(const std::string &username);
