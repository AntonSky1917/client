#ifndef IMESSAGE_CONTACTSPAGE_H
#define IMESSAGE_CONTACTSPAGE_H

#include <QWidget>
#include <QGridLayout>
#include <QListWidget>
#include <QVector>
#include <QPair>
#include <QString>
#include "ContactListItem.h"

class ContactsPage : public QWidget {
Q_OBJECT;
public:
    explicit ContactsPage(QWidget *parent = nullptr);

    void setUserId(quint64 id) noexcept;

    void addItemContact(QWidget* widget);

signals:

    void signalContactButtonClicked(quint64 receiver_id);

    void signalSendToAllContacts();

public slots:
    void slotEmitOpenChat(quint64);

    void slotSendToAllContacts();

private:
    QGridLayout *grid;
    QListWidget *contactList;
    quint64 user_id;
    QPushButton* send_to_all_contacts;

    void initWidgets();

    void initLayout();

    void initConnections();
};


#endif //IMESSAGE_CONTACTSPAGE_H
