#ifndef IMESSAGE_REGISTRATIONDIALOG_H
#define IMESSAGE_REGISTRATIONDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "../DatabaseManager.h"

class RegistrationDialog : public QDialog {
Q_OBJECT;
public:
    explicit RegistrationDialog(QWidget* parent =nullptr);
private slots:
    void slotRegister();
private:

    void initWidgets();
    void initLayout();
    void initConnections();

    QGridLayout *grid;

    QLabel* label_for_password;
    QLineEdit* password;

    QLabel* label_for_username;
    QLineEdit* username;

    QPushButton* registration;
};


#endif //IMESSAGE_REGISTRATIONDIALOG_H
