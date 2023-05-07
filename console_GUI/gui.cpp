#include <cstring>
#include <iostream>
#include <ncurses.h>
#include <random>
#include <string>
#include <vector>

enum {
    ESC = -2,
    BACK = -1,
    KEY_BACK = 127,
    KEY_ESC = 27,
};

// выбрать один из вариантов, вернется либо индекс выбранного, либо флаги возврата и выхода
int choose_one_of_list(int max_rows, int max_cols, std::vector<std::string> &list_to_show) {
    init_pair(2, COLOR_BLUE, COLOR_WHITE); // цвет выделения
    int num_rows = std::min(max_rows, static_cast<int>(list_to_show.size()));
    int choice = 0; //Выбор пользователя
    while (true) {
        clear();
        for (int i = choice; i < std::min(choice + num_rows, static_cast<int>(list_to_show.size())); i++) {
            if (i == choice) { // текущий совпадает с выбором пользователя
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
                return ESC;
            case KEY_BACK:
                return BACK; // вернуться, но пока не понимаю, как запоминать предыдущий этап
            case KEY_UP:
                if (choice) { // указатель вверх
                    --choice;
                }
                break;
            case KEY_DOWN:
                if (choice < list_to_show.size() - 1) { // указатель вниз
                    ++choice;
                }
                break;
            case '\n':
                return choice; // какой индекс из списка контактов выбрал
        }
    }
}

std::vector<std::string> generate_contact_names() {
    std::vector<std::string> contacts;
    std::vector<std::string> first_names = {"Alice", "Bob", "Charlie", "David", "Emily", "Frank", "Grace", "Hannah", "Isaac", "Jack", "Kate", "Luke", "Megan", "Nathan", "Olivia", "Peter", "Quinn", "Rachel", "Sarah", "Tom", "Ursula", "Victoria", "Wendy", "Xander", "Yvette", "Zachary"};
    std::vector<std::string> last_names = {"Adams", "Brown", "Clark", "Davis", "Evans", "Franklin", "Garcia", "Hernandez", "Irwin", "Jackson", "Kim", "Lee", "Martin", "Nguyen", "Owens", "Patel", "Quinn", "Rodriguez", "Smith", "Taylor", "Upton", "Vargas", "Walker", "Xu", "Young", "Zhang"};
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> first_name_dist(0, first_names.size() - 1);
    std::uniform_int_distribution<int> last_name_dist(0, last_names.size() - 1);
    for (int i = 0; i < 100; ++i) { // почему 100?
        std::string full_name = first_names[first_name_dist(rng)] + " " + last_names[last_name_dist(rng)];
        contacts.push_back(full_name);
    }
    return contacts;
}

int main(int argc, const char *argv[]) {
    auto contacts = generate_contact_names();

    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE); // Цвет окна
    curs_set(0);
    keypad(stdscr, true);
    int max_rows;
    int max_cols;
    getmaxyx(stdscr, max_rows, max_cols);

    int return_value = choose_one_of_list(max_rows, max_cols, contacts);
    if (return_value == ESC) {
        std::cout << "ESCAPE" << std::endl;
    } else if (return_value == BACK) {
        clear();
        mvwprintw(stdscr, max_rows / 2, (max_cols - strlen("Backspace pressed")) / 2, "%s", "Backspace pressed");
        refresh();
        getch();
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
    return 0;
}