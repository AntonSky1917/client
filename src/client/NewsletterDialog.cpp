#include "NewsletterDialog.h"

// реализуем класс интерфейс для отправки сообщений рассылки
// этот класс создает и настраивает интерфейс диалогового окна для отправки сообщений рассылки,
// позволяет вводить текст сообщения и отправлять его всем контактам указанного пользователя через сокет
NewsletterDialog::NewsletterDialog(QTcpSocket* socket, quint64 sender, QWidget *parent) :
        QDialog(parent),
        sender_id(sender),
        socket(socket),
        grid(new QGridLayout()),
        send(new QPushButton()),
        message_to_send(new QLineEdit()),
        label_for_message(new QLabel()) {
    initWidgets();
    initLayout();
    initConnections();
}

void NewsletterDialog::initWidgets() {
    send->setText("send");
    message_to_send->setPlaceholderText("Type message here...");
    label_for_message->setText("Type message that will be send to all users");
}

void NewsletterDialog::initLayout() {
    setLayout(grid);

    grid->addWidget(label_for_message, 0, 0, 1, 3);
    grid->addWidget(message_to_send, 1, 0, 1, 4);
    grid->addWidget(send, 2, 3, 1, 1);
}

void NewsletterDialog::initConnections() {
    connect(send, &QPushButton::clicked, this, &NewsletterDialog::slotSendToAllContacts);
}

QString NewsletterDialog::getMessage() const {
    return message_to_send->text();
}

void NewsletterDialog::slotSendToAllContacts() {
    auto text = message_to_send->text();
    if (text.size() == 0)
        return;

    DatabaseManager databaseManager;

    auto contacts_id = databaseManager.selectAllContactsIdForUser(sender_id);

    QDataStream clientStream(socket);

    clientStream.setVersion(QDataStream::Qt_6_2);

    for (auto& id : contacts_id) {
        QJsonObject message;
        message["type"] = QStringLiteral("message");
        message["text"] = text.toStdString().c_str();
        message["sender_id"] = QString::number(sender_id);
        message["receiver_id"] = QString::number(id);

        clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
    }
    message_to_send->clear();
}
