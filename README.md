# client

Chat written on Qt6 with PostgreSQL16

## installing PostgreSQL drivers for QT

### Apple m2 processor

Database drivers are located here (considering that you've installed qt via homebrew): `/opt/homebrew/Cellar/qt/<version>/share/qt/plugins/sqldrivers`

So you can install QPSQL drivers executing this command

```shell
brew install qt-postgresql
```
## !!!
## ВАЖНО! На данный момент на клиенте нет реализации возможности добавления контактов
##  таким образом, для того что-бы протестировать код вам необходимо создать структуру базы данных, в соответствии с этим кодом:

## ------------------------------------------------

CREATE TABLE public.user (
    id bigserial not null,
    username varchar(32) not null,
    first_name varchar(100) not null,
    last_name varchar(100) not null,
    password varchar(100) not null
);

ALTER TABLE public.user ADD PRIMARY KEY (id);
ALTER TABLE public.user ADD CONSTRAINT unique_username UNIQUE (username);

INSERT INTO public.user (username, first_name, last_name, password) VALUES ('steve_jobs', 'steve', 'jobs', '123');
INSERT INTO public.user (username, first_name, last_name, password) VALUES ('ilon_number1', 'ilon', 'mask', '123');
INSERT INTO public.user (username, first_name, last_name, password) VALUES ('toni', 'anton', 'noname', '123');

CREATE TABLE public.message (
    id bigserial not null primary key,
    sender_id bigint references public.user(id),
    receiver_id bigint references public.user(id),
    content text not null,
    send_datetime timestamp not null
);

ALTER TABLE public.message ADD CONSTRAINT auto_send_datetime UNIQUE (send_datetime);
ALTER TABLE public.message ALTER COLUMN send_datetime set default current_timestamp;

CREATE TABLE public.auth_log (
    id bigserial not null primary key,
    user_id bigint references public.user(id),
    auth_datetime timestamp not null
);

ALTER TABLE public.auth_log ALTER COLUMN auth_datetime set default current_timestamp;

CREATE TABLE public.contacts (
    id bigserial not null primary key,
    user_id bigint references public.user(id) not null,
    contact_id bigint references public.user(id) not null,
    becomes_friend_datetime timestamp not null default current_timestamp
);

INSERT INTO public.contacts (user_id, contact_id) VALUES (1, 3);

SELECT (id, username, first_name, last_name) FROM public.user WHERE id in (SELECT (contact_id) FROM public.contacts WHERE user_id = 1);

## ------------------------------------------------

## !!!
## Далее при запуске программы нужно будет указывать: хост/юзер/пароль/название бд
 Пример: % ./cmake-build-debug/client --host localhost --user 123 --password 123 --database_name client_123 
 # Они должны подставится в файл конфига ConnectionInfo.h автоматически c портом 5432, или измените порт.

# сервер и клиент сидят на порте 6000