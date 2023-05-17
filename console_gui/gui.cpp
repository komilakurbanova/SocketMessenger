#include "gui.h"
#include <exception>
#include <vector>

boost::asio::io_context io_context;
ServerConnector connector(&io_context);

// выбор одного варианта из списка, вернет либо индекс выбранного, либо флаги возврата и выхода

void print_chat_name(Chat chat) {
    printw("%s\n", chat.chat_name.c_str());
}

void print_name_of_user(User user) {
    printw("%s\n", user.name.c_str());
}

int choose_one_of_list_chat(int max_rows, int max_cols, std::vector<Chat>& list_to_show, int mode) {
    init_pair(2, COLOR_BLUE, COLOR_WHITE); // цвет выделения
    int num_rows = std::min(max_rows, static_cast<int>(list_to_show.size()));
    int choice = 0; //Выбор пользователя
    while (true) {
        clear();
        int first_row = std::max(0, choice - num_rows / 2);
        for (int i = first_row; i < std::min(first_row + num_rows, static_cast<int>(list_to_show.size())); ++i) {
            if (i == choice) // текущий совпадает с выбором пользователя
            {
                attron(COLOR_PAIR(2));
                addch('>'); // выводим указатель
                if (mode == CHOOSE_CHAT) {
                    print_chat_name(list_to_show[i]);
                } else if (mode == NEW_CHAT) {
                    send_system_message("wrong call of choose_one_of_list_chat");
                }
                attroff(COLOR_PAIR(2));
            } else {
                addch(' ');
                if (mode == CHOOSE_CHAT) {
                    print_chat_name(list_to_show[i]);
                } else if (mode == NEW_CHAT) {
                    send_system_message("wrong call of choose_one_of_list_chat");
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

int choose_one_of_list_user(int max_rows, int max_cols, std::vector<User>& list_to_show, int mode) {
    init_pair(2, COLOR_BLUE, COLOR_WHITE); // цвет выделения
    int num_rows = std::min(max_rows, static_cast<int>(list_to_show.size()));
    int choice = 0; //Выбор пользователя
    while (true) {
        clear();
        int first_row = std::max(0, choice - num_rows / 2);
        for (int i = first_row; i < std::min(first_row + num_rows, static_cast<int>(list_to_show.size())); ++i) {
            if (i == choice) // текущий совпадает с выбором пользователя
            {
                attron(COLOR_PAIR(2));
                addch('>'); // выводим указатель
                if (mode == CHOOSE_CHAT) {
                    send_system_message("wrong call of choose_one_of_list_user");
                } else if (mode == NEW_CHAT) {
                    print_name_of_user(list_to_show[i]);
                }
                attroff(COLOR_PAIR(2));
            } else {
                addch(' ');
                if (mode == CHOOSE_CHAT) {
                    send_system_message("wrong call of choose_one_of_list_user");
                } else if (mode == NEW_CHAT) {
                    print_name_of_user(list_to_show[i]);
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
            delwin(win);
            return BACK;
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

void send_system_message(const std::string &message) {
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

// TODO как проверять ввод специальных символов, которые читаются как несколько? (никак)
bool is_sanitizes_input_not_empty(char *value) {
    size_t len = strlen(value);
    size_t cnt = 0;
    while (cnt < len) {
        if (!isgraph(value[cnt])) {
            for (size_t j = cnt; j < len - 1; ++j) {
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

std::vector<std::string> registration_forms(int pixel_diff,
                                            const std::string &button,
                                            const std::vector<std::string> &fields)
{
    std::vector<std::string> field_values;
    clear();
    start_color();
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    // Рисуем рамку вокруг формы
    WINDOW *form_win = newwin(6 + pixel_diff, 50, max_rows / 2 - 5, max_cols / 2 - BUFSIZE);
    box(form_win, 0, 0);
    refresh();
    wrefresh(form_win);
    // Выводим поля ввода и метки к ним
    for (int i = 0; i < fields.size(); ++i) {
        mvprintw((max_rows / 2) - 3 + i, (max_cols / 2) - 18, "%s: ", fields[i].c_str());
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
            send_system_message("You entered empty string in " + fields[i] + "!");
            return registration_forms(pixel_diff, button, fields);
        }
    }
    // Создаем кнопку
    WINDOW *button_win = newwin(3, 15, max_rows / 2 + pixel_diff, max_cols / 2 - 2);
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
    refresh();
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    // TODO вызов бд, получить чаты
    std::string chat_id = "";
    std::vector<std::string> chat_ids = db.getChatIdsByUsername(username);
    if (chat_ids.empty()) {
        return chat_id;
    }
    curs_set(0);
    keypad(stdscr, true);

    // индекс выбранного из всего списка, либо команды
    // TODO раскоменть, это печать всех чатов
    int idx = 0; // TODO удали
    // int idx = choose_one_of_list_chat(max_rows, max_cols, chat_ids, CHOOSE_CHAT);

    if (idx == ESC) {
        endwin();
        exit(0);
    } else if (idx == BACK) {
        endwin();
        index();
        return chat_id;
    }
    chat_id = chat_ids[idx];
    endwin();
    return chat_id;
}


std::string open_users_list(const std::string &client_username) {
    clear();
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    std::string chat_id = "";

    std::vector<User> users = connector.GetAllUsers();

    int idx = choose_one_of_list_user(max_rows, max_cols, users, NEW_CHAT);
    if (idx == ESC) {
        endwin();
        exit(0);
    } else if (idx == BACK) {
        refresh();
        index();
    } else {
        std::string chosen_username = users[idx].username;
        send_system_message("Chosen username is " + chosen_username + ". Name is " + users[idx].name);
        if (chosen_username == client_username) {
            send_system_message("Choose someone else, you cannot create a chat with yourself");
            open_users_list(client_username);
            return chat_id;
        }

        std::string chat_name = chosen_username + " CHAT";
        connector.CreateChat(client_username, chosen_username, chat_name);

        std::vector<Chat> all_chats = connector.GetAllChats(users[idx]);
        chat_id = all_chats.back().chat_id;
        if (all_chats.size() > 0) {
            send_system_message("Chat '" + chat_name + "' was created! (chat_id " + chat_id + ")");
        } else {
            send_system_message("Oops, something went wrong!");
        }
    }
    return chat_id;
}

// sign_up or login
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
    if (choice == BACK) {
        index();
        return;
    } else if (choice == SIGNUP) {
        // TODO signup возвращает {} всегда
        std::vector<std::string> field_values = signup();
        if (field_values.empty()) {
            endwin();
            exit(0);
        }
        username = field_values[1];

        send_system_message("Welcome, " + db.getName(username) + "!");
    } else if (choice == LOGIN) {
        std::vector<std::string> field_values = login();
        if (field_values.empty()) {
            endwin();
            exit(0);
        }
        username = field_values[0];
        // TODO отправить серверу, узнать имя
        // ProtocolPacket info = {OperationType::GET_USER_NAME, {username, "", ""}};
        // TODO получить имя от сервера

        send_system_message("Hello, " + db.getName(username) + "!");
    } else {
        endwin();
        exit(1);
    }
    home(username);
}

std::string show_input_field() {
    clear();
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    int field_y = max_rows - 2;
    int field_x = 0;
    int field_height = 1;
    int field_width = max_cols;
    WINDOW *field_win = newwin(field_height, field_width, field_y, field_x);
    box(field_win, 0, 0);
    wattron(field_win, A_BOLD);
    wattron(field_win, COLOR_PAIR(3));
    wrefresh(field_win);
    char buffer[MAXSIZE];
    echo();
    curs_set(1);
    mvwgetnstr(field_win, 0, 4, buffer, MAXSIZE);
    // пока не нажмём enter, мы ничего не ввели, но оно показывается
    noecho();
    curs_set(0);
    std::string message = buffer;
    werase(field_win);
    wrefresh(field_win);
    delwin(field_win);
    return message;
}

void display_message(Message& message) {
    addch(' ');
    attron(A_BOLD);
    printw("%s\n", message.sender_username.c_str());
    attroff(A_BOLD);
    printw("%s\n", message.content.c_str());
}

void display_all_messages(const std::string& chat_id) {
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    clear();
    refresh();
    const int max_num_rows = MAX_ROWS;
    std::vector<Message> list_to_show = db.getChatMessages(chat_id);

    int num_rows = std::min(max_num_rows, 2 * static_cast<int>(list_to_show.size()));
    int first_row = std::max(0, static_cast<int>(list_to_show.size()) - num_rows / 2);

    // Display all messages on screen
    for (int i = first_row; i < std::min(first_row + num_rows / 2, static_cast<int>(list_to_show.size())); ++i) {
        display_message(list_to_show[i]);
    }
    refresh();
}

// bool abool = false; // TODO delete;
// Message new_msg = {
//     .content = "HALO",
//     .sender_username = "Loki",
// };

void display_new_messages(const std::string& chat_id, std::mutex& m) {
    // while (true) {
    //     // TODO получаем какое-то сообщение от сервера
    //     if (abool) {
    //         m.lock();

    //         db.addMessage(chat_id, new_msg.sender_username, new_msg.content); // TODO избавиться от new_msg

    //         display_all_messages(chat_id);
    //         abool = false;

    //         m.unlock();
    //     }
        // TODO: add listening for new messages from server
    // }
}

void send_messages(const std::string& chat_id, const std::string& username, std::mutex& m) {
    while (true) {
        switch (getch()) {
            case KEY_ESC:
                endwin();
                exit(0);
            case KEY_BACK:
                home(username);
                break;
            case '\n':
            case KEY_ENTER:
                m.lock();

                std::string message;
                message = show_input_field();

                if (!message.empty()) {
                    Message new_message;
                    new_message.sender_username = db.getName(username);
                    new_message.chat_id = chat_id;
                    new_message.content = message;

                    //TODO send message to the server
                    db.addMessage(chat_id, username, message);
                    display_all_messages(chat_id); // TODO эта штука не будет скорее всего нужна, когда прикрутим сервер
                }

                m.unlock();

                break;
        }
    }
}

void start_chat(const std::string& chat_id, const std::string& username) {
    std::mutex m;

    display_all_messages(chat_id);

    std::thread display_thread(display_new_messages, std::ref(chat_id), std::ref(m));
    std::thread send_thread(send_messages, std::ref(chat_id), std::ref(username), std::ref(m));

    // for (size_t i = 0; i < 10; ++i) {
    //     std::this_thread::sleep_for(std::chrono::seconds(5));
    //     abool = true;
    // }

    display_thread.join();
    send_thread.join();
}

void home(const std::string &username) { // TODO важно!
    clear();
    std::vector<std::string> home_buttons = {"New chat", "Open chat", "Delete chat"};
    int choice = NEW + choose_system_call(home_buttons);

    std::string chat_id = "";
    if (choice == NEW + BACK) {
        index();
        return;
    } else if (choice == NEW) {
        chat_id = open_users_list(username);
    } else if (choice == OPEN || choice == DELETE) {
        chat_id = choose_chat(username);
    } else {
        endwin();
        exit(1);
    }
    if (chat_id.empty()) {
        send_system_message("You have no chats. Create NEW one");
        home(username);
        return;
    }
    if (choice == DELETE) {
        db.removeChat(chat_id);
        home(username);
        return;
    }

    start_chat(chat_id, username);
}


std::vector<std::string> login() {
    int pixel_diff = 0;
    std::string button = "Login";
    std::vector<std::string> fields = {"Username", "Password"};
    std::vector<std::string> field_values = registration_forms(pixel_diff, button, fields);

    const std::string username = field_values[0];
    // TODO вызов сервера. Есть ли такой пользователь?
    // ProtocolPacket info = {OperationType::GET_USER_NAME, {username, "", ""}};
    // TODO проверить статус
    if (db.getName(username).empty()) {
        send_system_message("This user does not exist");
        login();
    }
    // TODO вызов сервера. Получить пароль
    // ProtocolPacket info = {OperationType::GET_USER_PASSWORD_HASH, {username, "", ""}};
    // TODO получить имя от сервера

    if (db.getPasswordHash(username) != field_values[1]) {
        send_system_message("Wrong password. Try again");
        login();
    }
    return field_values;
}

std::vector<std::string> signup() {
    int pixel_diff = 4;
    std::string button = "Sign Up";
    std::vector<std::string> fields = {"Name", "Username", "Password"};
    std::vector<std::string> field_values = registration_forms(pixel_diff, button, fields);

    User new_user {
        .name = field_values[0],
        .username = field_values[1],
        .password_hash = field_values[2],
    };
    connector.AddUser(new_user);

    return field_values;
}

int main() {
    initscr();
    index();
    endwin();
}
