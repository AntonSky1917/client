#ifndef IMESSAGE_DATABASEMANAGER_H
#define IMESSAGE_DATABASEMANAGER_H

#include <iostream>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QDebug>
#include <QVector>
#include "ConnectionInfo.h"
#include "client/ContactListItem.h"

class DatabaseManager {
public:
    DatabaseManager();

    DatabaseManager(const QString &host, const QString &database_name, const QString &username, const QString &password,
                    qint32 port);

    DatabaseManager(QString &&host, QString &&database_name, QString &&username, QString &&password, qint32 port);

    ~DatabaseManager();

    DatabaseManager(const DatabaseManager &rhs) = delete;

    DatabaseManager &operator=(const DatabaseManager &rhs) = delete;

    void showVersion() const;

    [[nodiscard]] quint64 selectUserId(const QString &username, const QString &raw_password) const;

    void insertAuthLogAboutUser(quint64 user_id) const;

    [[nodiscard]] QVector<QWidget *> selectAllContactsForUser(quint64 user_id, QWidget* parent = nullptr) const;

    [[nodiscard]] QVector<quint64> selectAllContactsIdForUser(quint64 user_id) const;

    void insertMessage(quint64 sender_id, quint64 receiver_id, const QByteArray& message) const;

    [[nodiscard]] QVector<QMap<QString, QString>> selectMessagesForUsers(quint64 sender_id, quint64 receiver_id) const;

    void createNewUser(const QString& username, const QString& password) const;

private:
    QSqlDatabase db;
};


#endif //IMESSAGE_DATABASEMANAGER_H
