#ifndef IMESSAGE_MAINWINDOW_H
#define IMESSAGE_MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QWebSocket>
#include <QUrl>
#include <QList>
#include <QPushButton>
#include <QStackedWidget>
#include <QSignalMapper>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include "ConnectionInfo.h"
#include "client/ContactsPage.h"
#include "client/ChatPage.h"
#include "client/AuthorizePage.h"
#include "client/NewsletterDialog.h"

class MainWindow : public QMainWindow {
Q_OBJECT;
public:
    explicit MainWindow(quint32 TCPPort, const QString &windowTitle = "iMessenger", QWidget *parent = nullptr);

signals:

    void signalOpenChat();

public slots:

    void slotRemoveChatWidget();

    void slotSendToServer();


private slots:

    void slotOpenChat(quint64 receiver_id);

void slotOpenNewsletterDialog();

void readSocket();

void slotOpenMainWindow();

private:
    quint32 TCPPort;
    QWidget *widget;
    QGridLayout *grid;
    QTcpSocket *webSocket;
    ContactsPage *contactsPage;
    QStackedWidget *stackedWidget;
    AuthorizePage *authorizePage;
    quint64 user_id;
    NewsletterDialog* dialog;

    void initWidgets();

    void initLayout();

    void initConnections();

    void jsonReceived(const QJsonObject& docObj);
};


#endif //IMESSAGE_MAINWINDOW_H
