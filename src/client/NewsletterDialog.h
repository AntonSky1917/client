#ifndef IMESSAGE_NEWSLETTERDIALOG_H
#define IMESSAGE_NEWSLETTERDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include "../DatabaseManager.h"

class NewsletterDialog  : public QDialog {
Q_OBJECT;
public:
    explicit NewsletterDialog(QTcpSocket* socket, quint64 sender, QWidget* parent = nullptr);
    [[nodiscard]] QString getMessage() const;
private slots:
    void slotSendToAllContacts();
private:
    void initWidgets();
    void initLayout();
    void initConnections();

    quint64 sender_id;
    QTcpSocket *socket;
    QGridLayout* grid;
    QPushButton* send;
    QLineEdit* message_to_send;
    QLabel* label_for_message;
};


#endif //IMESSAGE_NEWSLETTERDIALOG_H
