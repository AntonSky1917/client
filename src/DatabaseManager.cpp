#include "DatabaseManager.h"

// реализуем класс менеджер базы данных для взаимодействия с базой данных PostgreSQL
DatabaseManager::DatabaseManager(const QString &host, const QString &database_name, const QString &username,
                                 const QString &password, qint32 port) : db(QSqlDatabase::addDatabase("QPSQL")) {
    db.setHostName(host);
    db.setDatabaseName(database_name);
    db.setUserName(username);
    db.setPassword(password);
    db.setPort(port);
    try {
        if (db.open()) {
            qDebug() << "DatabaseManager: Successfully connected to database " << database_name;
        } else {
            throw std::runtime_error(db.lastError().text().toStdString());
        }
    } catch (std::exception &exception) {
        qCritical() << "DatabaseManager: " << db.lastError();
    }
}

// rvalue-ссылки на параметры для оптимизации и обеспечения правильного перемещения данных
DatabaseManager::DatabaseManager(QString &&host, QString &&database_name, QString &&username, QString &&password,
                                 qint32 port) : db(QSqlDatabase::addDatabase("QPSQL")) {
    db.setHostName(host);
    db.setDatabaseName(database_name);
    db.setUserName(username);
    db.setPassword(password);
    db.setPort(port);
    if (db.open()) {
        qDebug() << "DatabaseManager: Successfully connected to database " << database_name << " (move ctor)";
    } else {
        throw std::runtime_error(db.lastError().text().toStdString());
    }
}

// для настройки соединения с базой данных
DatabaseManager::DatabaseManager() : db(QSqlDatabase::addDatabase("QPSQL")) {
    db.setHostName(DatabaseInfo::host);
    db.setDatabaseName(DatabaseInfo::database_name);
    db.setUserName(DatabaseInfo::user);
    db.setPassword(DatabaseInfo::password);
    db.setPort(DatabaseInfo::port);
    if (db.open()) {
        qDebug() << "DatabaseManager: Successfully connected to database " << DatabaseInfo::database_name;
    } else {
        throw std::runtime_error(db.lastError().text().toStdString());
    }
}

// извлекает и выводит версию PostgreSQL
void DatabaseManager::showVersion() const {
    auto query = db.exec("SELECT version();");
    query.next();
    if (query.isValid()) {
        qDebug() << "DatabaseManager: " << query.value(0).toString();
    } else {
        qWarning() << "DatabaseManager: can not fetch version of postgresql";
    }
}

// запрос для поиска идентификатора пользователя по его имени и паролю
quint64 DatabaseManager::selectUserId(const QString &username, const QString &raw_password) const {
    QSqlQuery query;
    query.prepare("SELECT (id) FROM public.user WHERE username=:username AND password=:password");
    query.bindValue(":username", username);
    query.bindValue(":password", raw_password);
    try {
        query.exec();
    } catch (std::exception &exception) {
        qCritical() << "DatabaseManager: can not exec query: " << query.lastError();
        return 0;
    }
    query.next();
    if (query.isValid()) {
        return query.value(0).toULongLong();
    } else {
        return 0;
    }
}

// деструктор проверяет, открыто ли соединение с базой данных, и, если да, закрывает его
DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();
    }
}

// для вставки записи в таблицу "public.auth_log" с указанным идентификатором пользователя (user_id)
void DatabaseManager::insertAuthLogAboutUser(quint64 user_id) const {
    QSqlQuery query;
    query.prepare("INSERT INTO public.auth_log (user_id) VALUES (:user_id)");
    query.bindValue(":user_id", user_id);
    if (query.exec()) {
        qDebug() << "DatabaseManager: inserted row into auth_log successfully (user_id: " << user_id << ")";
    } else {
        qCritical() << "DatabaseManager: inserted failed: " << query.lastError();
    }
}

// запросы для выборки контактов пользователя (контакты могут быть как отправителями, так и получателями сообщений)
// результат запросов используется для создания вектора виджетов ContactListItem, представляющих контакты пользователя
QVector<QWidget *> DatabaseManager::selectAllContactsForUser(quint64 user_id, QWidget *parent) const {
    QVector<QWidget *> res;
    QSqlQuery query;
    query.prepare(
            "SELECT id, username, first_name, last_name FROM public.user WHERE id in (SELECT (contact_id) FROM public.contacts WHERE user_id=:id);");
    query.bindValue(":id", user_id);
    query.exec();
    while (query.next()) {
        res.push_back(new ContactListItem(query.value(0).toULongLong(), query.value(1).toByteArray(),
                                          query.value(2).toString(), query.value(3).toString(), parent));
    }


    query.prepare(
            "SELECT id, username, first_name, last_name FROM public.user WHERE id in (SELECT (user_id) FROM public.contacts WHERE contact_id = :id);");
    query.bindValue(":id", user_id);
    query.exec();
    while (query.next()) {
        res.push_back(
                new ContactListItem(query.value(0).toULongLong(), query.value(1).toString(), query.value(2).toString(),
                                    query.value(3).toString(), parent));
    }
    return res;
}

// вставка сообщения с указанным отправителем (sender_id), получателем (receiver_id) и содержанием сообщения (message)
void DatabaseManager::insertMessage(quint64 sender_id, quint64 receiver_id, const QByteArray &message) const {
    QSqlQuery query;
    query.prepare(
            "INSERT INTO public.message (sender_id, receiver_id, content) VALUES (:sender_id, :receiver_id, :content)");
    query.bindValue(":sender_id", sender_id);
    query.bindValue(":receiver_id", receiver_id);
    query.bindValue(":content", message.toStdString().c_str());
    if (query.exec()) {
        qDebug() << "DatabaseManager: inserted row into message successfully (sender: " << sender_id << ", receiver: "
                 << receiver_id << ")";
    } else {
        qCritical() << "DatabaseManager: inserted failed: " << query.lastError();
    }
}

// выборка сообщений между двумя пользователями (sender_id и receiver_id)
// результаты запроса возвращаются в виде вектора словарей (QMap), представляющих сообщения
QVector<QMap<QString, QString>> DatabaseManager::selectMessagesForUsers(quint64 sender_id, quint64 receiver_id) const {
    QVector<QMap<QString, QString>> res;
    QSqlQuery query;
    query.prepare(
            "SELECT sender_id, receiver_id, content, send_datetime FROM public.message WHERE sender_id=:sender_id AND receiver_id=:receiver_id OR receiver_id=:sender_id AND sender_id=:receiver_id ORDER BY send_datetime ASC;");
    query.bindValue(":sender_id", sender_id);
    query.bindValue(":receiver_id", receiver_id);
    query.exec();
    while (query.next()) {
        QMap<QString, QString> item{
                {"timestamp", query.value(3).toString()},
                {"message", query.value(2).toString()},
                {"receiver_id", query.value(1).toString()},
                {"sender_id", query.value(0).toString()}
        };
        res.push_back(item);
    }
    return res;
}

// выборка идентификаторов контактов пользователя, как отправителей, так и получателей сообщений
// результаты запросов возвращаются в виде вектора идентификаторов (quint64)
QVector<quint64> DatabaseManager::selectAllContactsIdForUser(quint64 user_id) const {
    QVector<quint64> res;
    QSqlQuery query;
    query.prepare(
            "SELECT id, username, first_name, last_name FROM public.user WHERE id in (SELECT (contact_id) FROM public.contacts WHERE user_id=:id);");
    query.bindValue(":id", user_id);
    query.exec();
    while (query.next()) {
        res.push_back(query.value(0).toULongLong());
    }


    query.prepare(
            "SELECT id, username, first_name, last_name FROM public.user WHERE id in (SELECT (user_id) FROM public.contacts WHERE contact_id = :id);");
    query.bindValue(":id", user_id);
    query.exec();
    while (query.next()) {
        res.push_back(query.value(0).toULongLong());
    }
    return res;
}

// создание нового пользователя в БД
void DatabaseManager::createNewUser(const QString &username, const QString &password) const {
    QSqlQuery query;
    query.prepare(
            "INSERT INTO public.user (username, password, first_name, last_name) VALUES (:username, :password, '', '')");
    query.bindValue(":username", username.toStdString().c_str());
    query.bindValue(":password", password.toStdString().c_str());
    if (query.exec()) {
        qDebug() << "DatabaseManager: created new user successfully";
    } else {
        qCritical() << "DatabaseManager: inserted failed: " << query.lastError();
    }
}
