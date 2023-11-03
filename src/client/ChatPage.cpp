#include "ChatPage.h"

// реализуем класс интерфейс станицы чата
ChatPage::ChatPage(quint64 sender_id, quint64 receiver_id, QWidget *parent) :
        QWidget(parent),
        user_id(sender_id),
        receiver_id(receiver_id),
        grid(new QGridLayout()),
        message(new QLineEdit()),
        listWidget(new QListWidget()),
        send(new QPushButton()),
        back_to_contacts(new QPushButton()) {
    initWidgets();
    initLayout();
    initConnections();
}

void ChatPage::initWidgets() {
    message->setPlaceholderText("Type some message...");

    send->setText("Send");
    back_to_contacts->setText("Back");

    // load history
    DatabaseManager databaseManager;
    auto messages = databaseManager.selectMessagesForUsers(user_id, receiver_id);
    for (const auto& message : messages) {
        auto sender_id = message["sender_id"].toULongLong();
        if (user_id == sender_id) {
            listWidget->addItem("me: " + message["message"]);
        } else {
            listWidget->addItem(message["sender_id"] + ": " + message["message"]);
        }
    }
}

void ChatPage::initLayout() {
    setLayout(grid);

    grid->addWidget(back_to_contacts, 0, 0, 1, 1);
    grid->addWidget(listWidget, 1, 0, 5, 5);
    grid->addWidget(message, 6, 0, 1, 4);
    grid->addWidget(send, 6, 4, 1, 1);
}

void ChatPage::initConnections() {
    auto main_window = dynamic_cast<MainWindow*>(parent());
    connect(back_to_contacts, &QPushButton::clicked, main_window, &MainWindow::slotRemoveChatWidget);
    connect(send, &QPushButton::clicked, main_window, &MainWindow::slotSendToServer);
}

QByteArray ChatPage::getMessage() const {
    return message->text().toUtf8();
}

void ChatPage::setUserId(quint64 id) {
    user_id = id;
}

void ChatPage::setReceiverId(quint64 id) {
    receiver_id = id;
}

quint64 ChatPage::getSenderId() const noexcept {
    return user_id;
}

quint64 ChatPage::getReceiverId() const noexcept {
    return receiver_id;
}

void ChatPage::addMessage(const QString &sender_name, const QString &message) {
    qDebug() << "ChatPage.h: adding message";
    listWidget->addItem(sender_name + ": " + message);
    listWidget->update();
}

void ChatPage::clearInput() {
    message->clear();
}
