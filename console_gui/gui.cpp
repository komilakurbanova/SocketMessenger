#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <random>
#include <ctype.h>

enum {
    ESC = -2,
    BACK = -1,
    KEY_BACK = 127,
    KEY_ESC = 27,
    LOGIN = 0,
    SIGNUP = 1,
    NEW = 2,
    OPEN = 3,
    DELETE = 4,
};

void index();

// выбор одного варианта из списка, вернет либо индекс выбранного, либо флаги возврата и выхода
int choose_one_of_list(int max_rows, int max_cols, const std::vector<std::string> &list_to_show) {
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
                printw("%s\n", list_to_show[i].c_str());
                attroff(COLOR_PAIR(2));
            } else {
                addch(' ');
                printw("%s\n", list_to_show[i].c_str());
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
                return choice; // какой индекс из списка контактов выбрал
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
            break; // какой индекс из списка контактов выбрал
        }
    }
    delwin(win);
    return choice;
}

std::vector<std::string> signup();

std::vector<std::string> login();

void home(const std::string &username);

bool is_sanitizes_input_not_empty(
        char *value) { // TODO как проверять ввод специальных символов, которые читаются как несколько?
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
    WINDOW *form_win = newwin(6 + diff, 50, max_rows / 2 - 5, max_cols / 2 - 20);
    box(form_win, 0, 0);
    refresh();
    wrefresh(form_win);
    // Выводим поля ввода и метки к ним
    for (int i = 0; i < fields.size(); ++i) {
        mvprintw(max_rows / 2 - 3 + i, max_cols / 2 - 18, "%s: ", fields[i].c_str());
        refresh();
        // Создаем окно для поля ввода
        WINDOW *field_win = newwin(1, 20, max_rows / 2 - 3 + i, max_cols / 2 + 2);
        box(field_win, 0, 0);
        wattron(field_win, A_BOLD);
        wattron(field_win, COLOR_PAIR(3));
        wrefresh(field_win);
        // Получаем от пользователя данные и сохраняем в массив символов
        char value[20]; // ограничение длины фиксировано, можно изменить
        wmove(field_win, 0, 1);
        echo();
        wgetstr(field_win, value);
        noecho();
        if (is_sanitizes_input_not_empty(value)) {
            // Строка содержит хотя бы один печатный символ, строка уже sanitized
            field_values.push_back(value);
        } else {
            // Строка пуста
            clear();
            refresh();
            int max_rows, max_cols;
            getmaxyx(stdscr, max_rows, max_cols);
            std::string greeting =
                    "You entered empty string in " + fields[i] + "!"; // TODO подтягивать из бд name по username
            attron(A_BOLD);
            mvprintw(10, max_cols / 2 - greeting.size() / 2, "%s\n\n", greeting.c_str());
            attroff(A_BOLD);
            refresh();
            sleep(2);
            return registration_forms(4, "Sign Up", {"Name", "Username", "Password"});
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

std::vector<std::string> generate_contact_names() {
    // генерация случайного списка контактов (заглушка)
    std::vector<std::string> contacts;
    std::vector<std::string> first_names = {"Alice", "Bob", "Charlie", "David", "Emily", "Frank", "Grace", "Hannah",
                                            "Isaac", "Jack", "Kate", "Luke", "Megan", "Nathan", "Olivia", "Peter",
                                            "Quinn", "Rachel", "Sarah", "Tom", "Ursula", "Victoria", "Wendy", "Xander",
                                            "Yvette", "Zachary"};
    std::vector<std::string> last_names = {"Adams", "Brown", "Clark", "Davis", "Evans", "Franklin", "Garcia",
                                           "Hernandez", "Irwin", "Jackson", "Kim", "Lee", "Martin", "Nguyen", "Owens",
                                           "Patel", "Quinn", "Rodriguez", "Smith", "Taylor", "Upton", "Vargas",
                                           "Walker", "Xu", "Young", "Zhang"};
    std::random_device rd; // инициализация генератора случайных чисел
    std::mt19937 rng(rd()); // используем Mersenne Twister 19937 как генератор
    std::uniform_int_distribution<int> first_name_dist(0, first_names.size() - 1);
    std::uniform_int_distribution<int> last_name_dist(0, last_names.size() - 1);
    for (int i = 0; i < 30; ++i) {
        std::string full_name = first_names[first_name_dist(rng)] + " " + last_names[last_name_dist(rng)];
        contacts.push_back(full_name);
    }
    return contacts;
}

void choose_chat(const std::string &username) { // TODO что лучше принимать? Какой-то класс пользователя мб
    clear();
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    // TODO вызов бд, получить чаты

    auto contacts = generate_contact_names();

    curs_set(0);
    keypad(stdscr, true);
    int return_value = choose_one_of_list(max_rows, max_cols, contacts);
    if (return_value == BACK) {
        refresh();
        index();
    } else {
        clear();
        printw("%s\n", std::to_string(return_value).c_str());
        attron(A_BOLD);
        printw("%s\n", contacts[return_value].c_str());
        attroff(A_BOLD);
        refresh();
        getch();
    }
    endwin();
    // TODO возвращать id чата из бд!!!
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
        // TODO вызов бд и добавление пользователя, тут отловить наличие кого-то с таким юзернеймом
//        for (auto e: field_values) {
//            std::cout << e << std::endl;
//        }

    } else if (choice == LOGIN) {
        auto field_values = login();
        if (field_values.empty()) {
            endwin();
            exit(0);
        }
        username = field_values[0];
        // TODO вызов бд и проверка на корректность входа
    } else {
        exit(1);
    }
    clear();
    refresh();
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    std::string greeting = "Welcome, " + username + "!"; // TODO подтягивать из бд name по username
    attron(A_BOLD);
    mvprintw(10, max_cols / 2 - greeting.size() / 2, "%s\n\n", greeting.c_str());
    attroff(A_BOLD);
    refresh();
    sleep(2);
    home(username);
}

void home(const std::string &username) {
    clear();
    std::vector<std::string> buttons = {"New chat", "Open chat", "Delete chat"};
    int choice = NEW + choose_system_call(buttons);

    int chat_id; // TODO или строка
    if (choice == NEW) {
        // chat_id = new chat id
    } else if (choice == OPEN || choice == DELETE) {
        choose_chat(username); // chat_id = ...
    } else {
        exit(1);
    }
    if (choice == DELETE) {
        // удалить из бд
        home(username);
    } else {
        // вызвать из бд все сообщения в хронологическом порядке
        // TODO обработчик и вывод сообщений на экран
        return;
    }

    // TODO вызов бд и добавление пользователя, тут отловить наличие кого-то с таким юзернеймом
    // TODO тут мне нужно получить список имеющихся чатов (пусть пока это все зареганные), но пока рандомно нагенерю
}

std::vector<std::string> login() {
    return registration_forms(0, "Login", {"Username", "Password"});
}

std::vector<std::string> signup() {
    return registration_forms(4, "Sign Up", {"Name", "Username", "Password"});
}

int main() {
    // Инициализация ncurses и экрана
    initscr();
    //choose_chat("Some-strange-named-person");
    index();

    endwin();
    return 0;
}
