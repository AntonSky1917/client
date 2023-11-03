#include "ContactListItem.h"

// реализуем класс который создает интерфейс элемента списка контактов
// этот класс создает и настраивает интерфейс элемента списка контактов,
// позволяет пользователю открывать чат с выбранным контактом и предоставляет метод для получения идентификатора пользователя, связанного с этим элементом
ContactListItem::ContactListItem(quint64 id, const QString &username, const QString &first_name, const QString& last_name, QWidget *parent) :
        QWidget(parent),
        grid(new QGridLayout()),
        user_id(id),
        username(new QLabel(username)),
        first_name(new QLabel(first_name)),
        last_name(new QLabel(last_name)),
        openChat(new QPushButton()),
        full_name(new QLabel()){
    initWidgets();
    initLayout();
    initConnections();
}

void ContactListItem::initWidgets() {
    openChat->setText("Message");
    openChat->setObjectName(QString::number(user_id));

    full_name->setText(first_name->text() + " " + last_name->text());
}

void ContactListItem::initLayout() {
    setLayout(grid);

    grid->addWidget(username, 0, 0, 1, 1);
    grid->addWidget(full_name, 0, 1, 1, 3);
    grid->addWidget(openChat, 0, 4, 1, 1);
}

void ContactListItem::initConnections() {
    auto contact_list = dynamic_cast<ContactsPage*>(parent());
    connect(openChat, &QPushButton::clicked, contact_list, &ContactsPage::slotEmitOpenChat);
}

quint64 ContactListItem::getUserId() const noexcept {
    return user_id;
}
