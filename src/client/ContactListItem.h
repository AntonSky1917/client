#ifndef IMESSAGE_CONTACTLISTITEM_H
#define IMESSAGE_CONTACTLISTITEM_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include "ContactsPage.h"

class ContactListItem : public QWidget {
Q_OBJECT;
public:
    explicit ContactListItem(quint64 id, const QString &username, const QString &first_name, const QString& last_name, QWidget *parent = nullptr);

    quint64 getUserId() const noexcept;

signals:

    void signalOpenChat();

private:
    QGridLayout *grid;
    quint64 user_id;
    QLabel *username;
    QLabel *first_name;
    QLabel *last_name;
    QPushButton *openChat;
    QLabel* full_name;


    void initWidgets();

    void initLayout();

    void initConnections();
};


#endif //IMESSAGE_CONTACTLISTITEM_H
