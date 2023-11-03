#include "RegistrationDialog.h"

// реализуем класс интерфейс для ввода имени пользователя и пароля для регистрации
// этот класс создает и настраивает интерфейс диалогового окна для регистрации новых пользователей,
// позволяет пользователям вводить имя пользователя и пароль, а также регистрироваться в приложении
RegistrationDialog::RegistrationDialog(QWidget *parent) :
        QDialog(parent),
        grid(new QGridLayout()),
        label_for_password(new QLabel()),
        password(new QLineEdit()),
        label_for_username(new QLabel()),
        username(new QLineEdit()),
        registration(new QPushButton()) {
    initWidgets();
    initLayout();
    initConnections();
}

void RegistrationDialog::initWidgets() {
    registration->setText("Registration");
    registration->setDefault(true);
    registration->setAutoDefault(true);

    label_for_username->setText("Username");
    username->setPlaceholderText("Type your username...");
    username->setStyleSheet("padding: 10px 12px; border-radius: 8px;");

    label_for_password->setText("Password");
    password->setPlaceholderText("Type your password...");
    password->setStyleSheet("padding: 10px 12px; border-radius: 8px;");
    password->setEchoMode(QLineEdit::Password);

    setFocus();
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void RegistrationDialog::initLayout() {
    setLayout(grid);

    grid->addWidget(label_for_username, 0, 0, 1, 1);
    grid->addWidget(username, 0, 1, 1, 1);
    grid->addWidget(label_for_password, 1, 0, 1, 1);
    grid->addWidget(password, 1, 1, 1, 1);

    grid->addWidget(registration, 2, 1, 1, 1);
}

void RegistrationDialog::initConnections() {
    connect(registration, &QPushButton::clicked, this, &RegistrationDialog::slotRegister);
}

void RegistrationDialog::slotRegister() {
    if (username->text().isEmpty() || password->text().isEmpty()) {
        return;
    }
    DatabaseManager databaseManager;
    databaseManager.createNewUser(username->text(), password->text());
    close();
}
