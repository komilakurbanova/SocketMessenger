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
    BUFSIZE = 20,
    CHOOSE_CHAT = 0,
    NEW_CHAT = 1,
};

const std::string connection_string = "dbname=messenger_db user=admin password=root host=db";
LocalDBManager db;

void index();

// выбор одного варианта из списка, вернет либо индекс выбранного, либо флаги возврата и выхода
int choose_one_of_list(int max_rows, int max_cols, std::vector<std::string> &list_to_show, int mode) {
    init_pair(2, COLOR_BLUE, COLOR_WHITE); // цвет выделения
    int num_rows = std::min(max_rows, static_cast<int>(list_to_show.size()));
    int choice = 0; //Выбор пользователя
    while (true) {
        clear();
        int first_row = std::max(0, choice - num_rows / 2);
        for (int i = first_row; i < std::min(first_row + num_rows, static_cast<int>(list_to_show.size())); i++) {
            if (i == choice) // текущий совпадает с выбором пользователя
            {
                attron(COLOR_PAIR(2));
                addch('>'); // выводим указатель
                if (mode == CHOOSE_CHAT) {
                    // TODO отправить серверу
                    // ProtocolPacket info = {OperationType::GET_CHAT_NAME, {list_to_show[i], "", "", ""}};
                    // TODO получить имя от сервера
                    
                    std::string chat_name = db.getChatName(list_to_show[i]);
                    printw("%s\n", chat_name.c_str());
                } else if (mode == NEW_CHAT) {
                    // TODO отправить серверу
                    // ProtocolPacket info = {OperationType::GET_NAME, {list_to_show[i], "", "", ""}};
                    // TODO получить имя от сервера
                    
                    std::string name = db.getName(list_to_show[i]);
                    printw("%s\n", name.c_str());
                }
                attroff(COLOR_PAIR(2));
            } else {
                addch(' ');
                if (mode == CHOOSE_CHAT) {
                    // TODO отправить серверу
                    // ProtocolPacket info = {OperationType::GET_CHAT_NAME, {list_to_show[i], "", "", ""}};
                    // TODO получить имя от сервера
                    
                    std::string chat_name = db.getChatName(list_to_show[i]);
                    printw("%s\n", chat_name.c_str());
                } else if (mode == NEW_CHAT) {
                    // TODO отправить серверу
                    // ProtocolPacket info = {OperationType::GET_NAME, {list_to_show[i], "", "", ""}};
                    // TODO получить имя от сервера
                    
                    std::string name = db.getName(list_to_show[i]);
                    printw("%s\n", name.c_str());
                }
                attroff(COLOR_PAIR(2));
            }
        }
        refresh();
        switch (getch()) {
            case KEY_ESC: // закрыть программу
                endwin();
                exit(0);
            case KEY_BACK:
                return BACK;
            case KEY_UP:
                if (choice > 0) // указатель вверх
                    --choice;
                if (choice < first_row) // прокрутка вверх
                    first_row = choice;
                break;
            case KEY_DOWN:
                if (choice < list_to_show.size() - 1) // указатель вниз
                    ++choice;
                if (choice >= first_row + num_rows) // прокрутка вниз
                    first_row = choice - num_rows + 1;
                break;
            case KEY_ENTER:
            case '\n':
                return choice; // какой индекс из списка выбран
        }
    }
}

int choose_system_call(const std::vector<std::string> &buttons) {
    WINDOW *win = newwin(7 + static_cast<int>(buttons.size()), 30, 5, 5);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);
    int choice = 0;
    while (true) {
        clear();
        box(win, 0, 0);
        mvwprintw(win, 2, 4, "Choose button:");
        for (int i = 0; i < buttons.size(); ++i) {
            if (i == choice) {
                mvwaddch(win, 4 + i, 5, '>'); // выводим указатель
            } else {
                mvwaddch(win, 4 + i, 5, ' ');
            }
            mvwprintw(win, 4 + i, 7, buttons[i].c_str());
        }
        refresh();
        wrefresh(win);
        int ch = getch();
        if (ch == KEY_ESC) { // закрыть программу
            delwin(win);
            endwin();
            exit(0);
        } else if (ch == KEY_BACK) {
            index();
        } else if (ch == KEY_UP && choice > 0) {
            --choice; // указатель вверх
        } else if (ch == KEY_DOWN && choice < buttons.size() - 1) {
            ++choice; // указатель вниз
        } else if (ch == KEY_ENTER || ch == '\n') {
            delwin(win);
            break;
        }
    }
    delwin(win);
    return choice;
}

std::vector<std::string> signup();

std::vector<std::string> login();

void home(const std::string &username);

void send_message(const std::string &message) {
    clear();
    refresh();
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    attron(A_BOLD);
    mvprintw(10, max_cols / 2 - message.size() / 2, "%s\n\n", message.c_str());
    attroff(A_BOLD);
    refresh();
    sleep(2);
}

bool is_sanitizes_input_not_empty(
        char *value) { // TODO как проверять ввод специальных символов, которые читаются как несколько? (никак)
    size_t len = strlen(value);
    size_t cnt = 0;
    while (cnt < len) {
        if (!isgraph(value[cnt])) {
            for (auto j = cnt; j < len - 1; ++j) {
                value[j] = value[j + 1];
            }
            value[len - 1] = '\0';
            --len;
        } else {
            ++cnt;
        }
    }
    return len;
}

std::vector<std::string>
registration_forms(int diff, const std::string &button, const std::vector<std::string> &fields) {
    std::vector<std::string> field_values;
    clear();
    start_color();
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    // Рисуем рамку вокруг формы
    WINDOW *form_win = newwin(6 + diff, 50, max_rows / 2 - 5, max_cols / 2 - BUFSIZE);
    box(form_win, 0, 0);
    refresh();
    wrefresh(form_win);
    // Выводим поля ввода и метки к ним
    for (int i = 0; i < fields.size(); ++i) {
        mvprintw(max_rows / 2 - 3 + i, max_cols / 2 - 18, "%s: ", fields[i].c_str());
        refresh();
        // Создаем окно для поля ввода
        WINDOW *field_win = newwin(1, BUFSIZE, max_rows / 2 - 3 + i, max_cols / 2 + 2);
        box(field_win, 0, 0);
        wattron(field_win, A_BOLD);
        wattron(field_win, COLOR_PAIR(3));
        wrefresh(field_win);

        // Получаем от пользователя данные и сохраняем в массив символов
        char *value = (char *) calloc(BUFSIZE, sizeof(value)); // ограничение длины фиксировано, можно изменить
        wmove(field_win, 0, 1);
        echo();
        wgetstr(field_win, value);
        noecho();

        if (is_sanitizes_input_not_empty(value)) {
            // Строка содержит хотя бы один печатный символ, строка теперь sanitized
            field_values.push_back(value);
        } else {
            // Строка пуста
            free(value);
            send_message("You entered empty string in " + fields[i] + "!");
            return registration_forms(diff, button, fields);
        }
    }
    // Создаем кнопку
    WINDOW *button_win = newwin(3, 15, max_rows / 2 + diff, max_cols / 2 - 2);
    box(button_win, 0, 0);
    mvwprintw(button_win, 1, 4, button.c_str());
    wrefresh(button_win);
    // Ожидаем, пока пользователь нажмет на кнопку
    int ch = getch();
    if (ch == KEY_ESC) {
        delwin(button_win);
        endwin();
        exit(0);
    } else if (ch == KEY_BACK) {
        index();
    }
    while (ch != KEY_ENTER && ch != '\n') {
        ch = getch();
    }

    endwin();
    return field_values;
}

std::string choose_chat(const std::string &username) {
    clear();
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    // вызов бд, получить чаты
    std::string chat_id = "";
    // TODO отправить серверу !!!!!!!!!
    auto chat_ids = db.getChatIdsByUsername(username);
    curs_set(0);
    keypad(stdscr, true);
    int idx = choose_one_of_list(max_rows, max_cols, chat_ids,
                                 CHOOSE_CHAT); // индекс выбранного из всего списка, либо команды
    if (idx == ESC) {
        endwin();
        exit(0);
    } else if (idx == BACK) {
        refresh();
        index();
    } else {
        clear();
        attron(A_BOLD);
        chat_id = chat_ids[idx];
        // TODO отправить серверу, имя чата
        // ProtocolPacket info = {OperationType::GET_CHAT_NAME, {chat_id, "", "", ""}};
        // TODO получить имя от сервера

        printw("%s\n", db.getChatName(chat_id).c_str());
        // TODO тут уже выбрали чат, просто показали, какой чат вообще
        attroff(A_BOLD);
        refresh();
        getch();
    }
    endwin();
    return chat_id;
}


std::string create_chat(const std::string &username) {
    clear();
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    std::string chat_id = "";
    // TODO отправить серверу !!!!!!!!!
    auto users = db.getUsernamesList();
    int idx = choose_one_of_list(max_rows, max_cols, users, NEW_CHAT);
    if (idx == ESC) {
        endwin();
        exit(0);
    } else if (idx == BACK) {
        refresh();
        index();
    } else {
        auto chosen_username = users[idx];
        if (chosen_username == username) {
            send_message("Choose someone else, you cannot create a chat with yourself");
            create_chat(username);
            return chat_id;
        }
        // TODO отправить серверу, получить имя пользователя
        // ProtocolPacket info = {OperationType::GET_USER_NAME, {username, "", ""}};
        // TODO получить имя от сервера
        std::string name = db.getName(chosen_username);

        // TODO отправить серверу, создать чат
        // ProtocolPacket info = {OperationType::CREATE_CHAT, {username, chosen_username,
        //                                                     name}};
        // TODO получить от сервера ответ: chat_id
        chat_id = db.createChat(username, chosen_username,
                                name); //TODO придумать имя чату, пока это просто имя собеседника
        if (chat_id.size() > 0) {
            send_message("Chat " + name + " was created!");
        } else {
            send_message("Oops, something went wrong!");
        }
    }
    return chat_id;
}

void index() {
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    clear();
    start_color();
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    refresh();
    std::vector<std::string> buttons = {"Login", "Sign Up"};

    int choice = LOGIN + choose_system_call(buttons);

    std::string username = "";
    if (choice == SIGNUP) {
        auto field_values = signup();
        if (field_values.empty()) {
            endwin();
            exit(0);
        }
        username = field_values[1];
        // TODO отправить серверу, узнать имя
        // ProtocolPacket info = {OperationType::GET_USER_NAME, {username, "", ""}};
        // TODO получить имя от сервера

        send_message("Welcome, " + db.getName(username) + "!");
    } else if (choice == LOGIN) {
        auto field_values = login();
        if (field_values.empty()) {
            endwin();
            exit(0);
        }
        username = field_values[0];
        // TODO отправить серверу, узнать имя
        // ProtocolPacket info = {OperationType::GET_USER_NAME, {username, "", ""}};
        // TODO получить имя от сервера

        send_message("Hello, " + db.getName(username) + "!");
    } else {
        endwin();
        exit(1);
    }
    home(username);
}

void home(const std::string &username) { // TODO важно!
    clear();
    std::vector<std::string> buttons = {"New chat", "Open chat", "Delete chat"};
    int choice = NEW + choose_system_call(buttons);
    std::string chat_id = "";
    if (choice == NEW) {
        chat_id = create_chat(username);
    } else if (choice == OPEN || choice == DELETE) {
        chat_id = choose_chat(username);
    } else {
        endwin();
        exit(1);
    }
    if (choice == DELETE) {
        // TODO удалить из бд
        home(username);
    } else {
        // TODO открыть чат, начать слушать порт
        return;
    }
}

std::vector<std::string> login() {
    auto field_values = registration_forms(0, "Login", {"Username", "Password"});
    const std::string username = field_values[0];
    // TODO вызов сервера. Есть ли такой пользователь?
    // ProtocolPacket info = {OperationType::GET_USER_NAME, {username, "", ""}};
    // TODO проверить статус
    if (db.getName(username).size() == 0) {
        send_message("This user does not exist");
        login();
    }
    // TODO вызов сервера. Получить пароль
    // ProtocolPacket info = {OperationType::GET_USER_PASSWORD_HASH, {username, "", ""}};
    // TODO получить имя от сервера

    if (db.getPasswordHash(username) != field_values[1]) {
        send_message("Wrong password. Try again");
        login();
    }
    return field_values;
}

std::vector<std::string> signup() {
    auto field_values = registration_forms(4, "Sign Up", {"Name", "Username", "Password"});
    // TODO отправить серверу
    // ProtocolPacket info = {OperationType::ADD_USER, {field_values[1], field_values[0], field_values[2], "blabla salt"}};
    // TODO получить от сервера ответ: вышло или нет

    if (db.addUser(field_values[1], field_values[0], field_values[2], "blabla salt")) {
        return field_values;
    }
    return {};
}

int main() {
    // добавим какого-то пользователя для тестов
    std::string username1 = "testuser1";
    std::string name1 = "Loki";
    std::string password_hash1 = "passwordhash1";
    std::string password_salt1 = "salt1";

    // TODO отправить серверу
    // ProtocolPacket info = {OperationType::ADD_USER, {username, "", ""}};
    // TODO получить имя от сервера

    if (db.addUser(username1, name1, password_hash1, password_salt1)) {
        std::cout << "User 1 added successfully." << std::endl;
    } else {
        std::cout << "Failed to add user 1." << std::endl;
    }

    // Инициализация ncurses, открытие окна
    initscr();
    index();
    endwin();
    // Завершение работы окна
    return 0;
}
