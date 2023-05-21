# SocketMessenger

## Необходимые библиотеки

`apt-get install libncurses5-dev libncursesw5-dev`

`sudo apt-get install libboost-all-dev`

> Компиляция сервера
 
`g++ -std=c++17 server/server_new.cpp db_manager/db_manager.cpp protocol/data_protocol.cpp -o server_bin -lboost_serialization`


> Компиляция клиента (GUI)

`g++ -std=c++17 console_gui/gui.cpp db_manager/db_manager.cpp protocol/data_protocol.cpp -o gui_bin -lboost_serialization -lboost_system -lboost_thread -lboost_chrono -lncurses`

---

## GUI

### Нажатия клавиш

`Escape` - выход из интерфейса.

`Backspace` - возврат к приветственному меню.

`Enter` - подтвердить действие: ввод или выбор опции.

Навигация с помощью стрелок `up` и `down`.

### Правила ввода данных при регистрации

До тех пор, пока поля авторизации/регистрации не будут заполнены (то есть пока выбор не дойдет до кнопки подтверждения `Login` или `Sign up`), нельзя пользоваться командами – все введенное будет учтено в заполнении полей.

Ограничение по количеству символов для каждого поля - **20**. Не отображающиеся графически (`!isgraph()`) символы будут проигнорированы при занесении в базу данных, но учтены при учете ограничения по длине. Ввод расчитан только на латинские символы.

### Ввод сообщений в чат

Нажмите `Enter` для того, чтобы получить доступ к вводу сообщения. В процессе ввода можно стирать символы и они не отразятся в сообщении, для этого используйте `Backspace`. Для отправки введенного сообщения нажмите `Enter`. Если поле ввода было отправленно пустым, то сообщение отправлено не будет, вы перейдете обратно к режиму чтения чата.
Из режима чтения можно вернуться обратно к выбору операций над чатами или закрыть программу.

---

## Цель проекта

Создать мессенджер на сокетах с возможностью открытия и закрытия чатов, просмотра истории, а также отправки сообщений между пользователями.
## Технические особенности

Проект реализован на языке С++ с дополнительным использованием библиотек boost и ncurses.

## Актуальность и описание мотивации

Мессенджеры стали неотъемлемой частью нашей повседневной жизни и являются важным средством общения для людей во всем мире.
Разработка мессенджера на сокетах может помочь в создании инструмента для обмена сообщениями и информацией между пользователями,
что может быть полезно для командной работы в рамках проектов или для обмена личной информацией между людьми.
Создание собственного месенджера дает возможность персонализировать его под цели общения и не зависеть от сторонних серверов, например,
в работе над общим проектом.

Можно выделить следующие преимущества мессенджеров на сокетах:

- Безопасность:
  - сокеты предоставляют высокий уровень безопасности при передаче данных, так как они используют шифрование и аутентификацию.
- Быстрая и удобная коммуникация
- Надежность:
  - сокеты используют TCP-протокол передачи данных,
который гарантирует доставку сообщения и контроль целостности данных
- Эффективность:
  - мессенджеры на сокетах используют минимальное количество ресурсов системы, что позволяет сохранять энергию
устройства и уменьшать использование интернет-трафика
- Гибкость:
  - подобные мессенджеры могут быть использованы на различных устройствах и операционных системах, так как они основаны на
стандарте TCP/IP, который поддерживается всеми современными устройствами и ОС


Данный проект может быть полезен для организаций или сообществ, которые желают иметь свою собственную систему обмена сообщениями для внутреннего
использования, где необходима контролируемая передача данных и обеспечение безопасности. Это также может быть полезным для тех, кто заботится о конфиденциальности
своей переписки, так как данные не будут храниться на сторонних серверах и будут шифроваться при передаче. Также подобный мессенджер может быть использован как чат-рулетка для знакомств.

Проект может быть использован для общения в условиях низкоскоростных интернет-соединений, где передача текстовых сообщений через сокеты может быть более
эффективной, чем использование тяжеловесных мессенджеров с избыточным функционалом.

## Структура проекта

- Структура проекта представлена (и будет дополняться) на [данной доске Miro](https://miro.com/welcomeonboard/Nkt5dmlNSFczRWo5em9lSVh0anpSaFJGc3VUSmNTRkt0YnRwckFjdnJWOXhmMTNUZk9tZ0lHWHJXdEYzcjhScHwzMDc0NDU3MzYxNDk0MDA1NDkzfDI=?share_link_id=894624831781)

## Список литературы

- [Кроссплатформенный многопоточный TCP/IP сервер на C++](https://habr.com/ru/post/503432)

- [Асинхронный ввод-вывод средствами POSIX](https://ps-group.github.io/os/nonblocking_io_posix)

- [Серия статей по Ncurses](https://code-live.ru/post/cpp-ncurses-hello-world/)

--- 

## Авторы

- [Владислав Анохов](https://t.me/pelanglene)
- [Комила Курбанова](https://t.me/kvrmalin)
- [Шевердов Никита](https://t.me/pchelka_zh)
