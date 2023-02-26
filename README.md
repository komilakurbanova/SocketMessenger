# SocketMessenger

## Цель проекта
Создать мессенджер на сокетах с возможностью открытия и закрытия чато, просмотра истории, а также отправки сообщений между пользователями. (В базовой конфигурации) 

## Технические особенности
Проект будет реализован на языке С++ с использованием PostgreSQL в качестве базы данных для хранения информации о пользователях и их чатах.

## Актуальность и описание мотивации
Мессенджеры стали неотъемлемой частью нашей повседневной жизни и являются важным средством общения для людей во всем мире. 
Разработка мессенджера на сокетах может помочь в создании инструмента для обмена сообщениями и информацией между пользователями, 
что может быть полезно для командной работы в рамках проектов или для обмена личной информацией между людьми. 
Создание собственного месенджера дает возможность персонализировать его под цели общения и не зависеть от сторонних серверов, например, 
в работе над общим проектом.

Можно выделить следующие преимущества мессенджеров на сокетах:
* Безопасность:
  * сокеты предоставляют высокий уровень безопасности при передаче данных, так как они используют шифрование и аутентификацию.
* Быстрая и удобная коммуникация
* Надежность:
   * сокеты используют TCP-протокол передачи данных, 
который гарантирует доставку сообщения и контроль целостности данных
* Эффективность:
  * мессенджеры на сокетах используют минимальное количество ресурсов системы, что позволяет сохранять энергию 
устройства и уменьшать использование интернет-трафика
* Гибкость:
  * подобные мессенджеры могут быть использованы на различных устройствах и операционных системах, так как они основаны на 
стандарте TCP/IP, который поддерживается всеми современными устройствами и ОС

Использование PostgreSQL в качестве базы данных позволяет хранить информацию о пользователях, чатах и сообщениях, обеспечивая быстрый и надежный доступ к данным. 
PostgreSQL обладает высоким уровнем защиты данных, что делает его подходящим для использования в качестве базы данных для мессенджера.

Данный проект может быть полезен для организаций или сообществ, которые желают иметь свою собственную систему обмена сообщениями для внутреннего 
использования, где необходима контролируемая передача данных и обеспечение безопасности. Это также может быть полезным для тех, кто заботится о конфиденциальности 
своей переписки, так как данные не будут храниться на сторонних серверах и будут шифроваться при передаче.

А также проект может быть использован для общения в условиях низкоскоростных интернет-соединений, где передача текстовых сообщений через сокеты может быть более 
эффективной, чем использование тяжеловесных мессенджеров с избыточным функционалом.

## [Структура проекта](https://miro.com/app/board/uXjVPtQUFw8=/?share_link_id=280083142639)

## Список литературы
[Кроссплатформенный многопоточный TCP/IP сервер на C++](https://habr.com/ru/post/503432)

[Асинхронный ввод-вывод средствами POSIX](https://ps-group.github.io/os/nonblocking_io_posix)
