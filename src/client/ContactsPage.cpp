#include "ContactsPage.h"

// реализуем класс интерфейс страницы контактов
// этот класс код создает и настраивает интерфейс страницы контактов,
// позволяет пользователю выбирать контакты и отправлять сообщения,
// а также предоставляет методы для добавления контактов в список и установки идентификатора текущего пользователя
ContactsPage::ContactsPage(QWidget *parent) :
        QWidget(parent), grid(new QGridLayout()), contactList(new QListWidget()), send_to_all_contacts(new QPushButton()) {
    initWidgets();
    initLayout();
    initConnections();
}

void ContactsPage::initWidgets() {
    send_to_all_contacts->setText("Send to all");
}

void ContactsPage::initLayout() {
    setLayout(grid);
    grid->addWidget(send_to_all_contacts, 0, 0, 1, 1);
    grid->addWidget(contactList, 1, 0, 5, 5);
}

void ContactsPage::initConnections() {
    connect(send_to_all_contacts, &QPushButton::clicked, this, &ContactsPage::slotSendToAllContacts);
}

void ContactsPage::slotEmitOpenChat(quint64 receiver_id) {
    auto* btn = dynamic_cast<QPushButton*>(sender());
    emit signalContactButtonClicked(btn->objectName().toULongLong());
}

void ContactsPage::setUserId(quint64 id) noexcept {
    user_id = id;
}

void ContactsPage::addItemContact(QWidget *widget) {
    auto *list_item = dynamic_cast<ContactListItem *>(widget);
    auto *item = new QListWidgetItem();
    item->setSizeHint(list_item->sizeHint());
    contactList->addItem(item);
    contactList->setItemWidget(item, list_item);
}

void ContactsPage::slotSendToAllContacts() {
    emit signalSendToAllContacts();
}
