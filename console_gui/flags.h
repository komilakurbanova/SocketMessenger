#pragma once

enum Flags {
    ESC = -2,
    BACK = -1,
    KEY_BACK = 127,
    KEY_ESC = 27,
    LOGIN = 0,
    SIGNUP = 1,
    NEW = 0,
    OPEN = 1,
    DELETE = 2,
    BUFSIZE = 20, // максимальная длина пароля/логина/имени
    MAXSIZE = 1024, // максимальная длина сообщения
    MAX_ROWS = 20, // максимальное число строк в чатах
};