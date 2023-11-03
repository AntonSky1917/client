#include "AuthorizePage.h"

// реализуем класс интерфейс станицы авторизации
AuthorizePage::AuthorizePage(QWidget *parent) :
        QWidget(parent),
        grid(new QGridLayout()),
        auth(new QPushButton()),
        reg(new QPushButton()),
        message_error(new QLabel()),
        username_label(new QLabel()),
        username_input(new QLineEdit()),
        password_label(new QLabel()),
        password_input(new QLineEdit()) {
    setWindowTitle("Authorization");
    initWidgets();
    initLayout();
    initConnections();
}

void AuthorizePage::initWidgets() {
    auth->setText("Authorize");
    auth->setDefault(true);
    auth->setAutoDefault(true);
    reg->setText("Registration");

    message_error->setText("Incorrect data. Please try again.");
    message_error->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    message_error->setStyleSheet("color: red;");

    username_label->setText("Username");
    username_input->setPlaceholderText("Type your username...");
    username_input->setStyleSheet("padding: 10px 12px; border-radius: 8px;");

    password_label->setText("Password");
    password_input->setPlaceholderText("Type your password...");
    password_input->setStyleSheet("padding: 10px 12px; border-radius: 8px;");
    password_input->setEchoMode(QLineEdit::Password);

    setFocus();
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void AuthorizePage::initLayout() {
    setLayout(grid);

    grid->addWidget(username_label, 1, 0, 1, 2);
    grid->addWidget(username_input, 1, 2, 1, 2);
    grid->addWidget(password_label, 2, 0, 1, 2);
    grid->addWidget(password_input, 2, 2, 1, 2);
    grid->addWidget(auth, 3, 3, 1, 1);
    grid->addWidget(reg, 3, 2, 1, 1);
}

void AuthorizePage::initConnections() {
    connect(auth, &QPushButton::clicked, this, &AuthorizePage::slotCheckPassword);
    connect(reg, &QPushButton::clicked, this, &AuthorizePage::slotOpenRegisterWindow);
}

void AuthorizePage::slotCheckPassword() {
    DatabaseManager manager;
    auto user_id = manager.selectUserId(username_input->text(), password_input->text());
    qDebug() << "Selected user_id: " << user_id;
    if (user_id) {
        manager.insertAuthLogAboutUser(user_id);
        this->user_id = user_id;
        emit signalOpenMainWindow();
    } else {

        grid->addWidget(message_error, 0, 0, 1, 4);
    }
}

quint64 AuthorizePage::getUserId() {
    return user_id;
}

void AuthorizePage::slotOpenRegisterWindow() {
    auto w = RegistrationDialog(this);
    w.exec();
}
