#ifndef IMESSAGE_AUTHORIZEPAGE_H
#define IMESSAGE_AUTHORIZEPAGE_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "../DatabaseManager.h"
#include "RegistrationDialog.h"

class AuthorizePage : public QWidget {
Q_OBJECT;
public:
    explicit AuthorizePage(QWidget *parent = nullptr);

    quint64 getUserId();
signals:
    void signalOpenMainWindow();
public slots:
    void slotCheckPassword();
void slotOpenRegisterWindow();
private:
    void initWidgets();
    void initLayout();
    void initConnections();

    quint64  user_id;

    QGridLayout* grid;
    QPushButton* auth;
    QPushButton* reg;

    QLabel* message_error;

    QLabel* username_label;
    QLineEdit* username_input;

    QLabel* password_label;
    QLineEdit* password_input;
};


#endif //IMESSAGE_AUTHORIZEPAGE_H
