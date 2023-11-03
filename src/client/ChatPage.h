#ifndef IMESSAGE_CHATPAGE_H
#define IMESSAGE_CHATPAGE_H

#include <QGridLayout>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QSignalMapper>
#include "../MainWindow.h"

class ChatPage : public QWidget {
Q_OBJECT;
public:
    explicit ChatPage(quint64 sender_id, quint64 receiver_id, QWidget* parent = nullptr);

    QByteArray getMessage() const;

    void setUserId(quint64 id);

    void setReceiverId(quint64 id);

    void clearInput();

    quint64 getSenderId() const noexcept;
    quint64 getReceiverId() const noexcept;

    void addMessage(const QString& sender_name, const QString& message);
private:
    quint64 user_id;
    quint64 receiver_id;
    QGridLayout* grid;
    QLineEdit* message;
    QListWidget* listWidget;
    QPushButton* send;
    QPushButton* back_to_contacts;


    void initWidgets();

    void initLayout();

    void initConnections();
};


#endif //IMESSAGE_CHATPAGE_H
