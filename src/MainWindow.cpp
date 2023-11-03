#include "MainWindow.h"

// инициализируем объект MainWindow и другие объекты
MainWindow::MainWindow(quint32 TCPPort, const QString &windowTitle, QWidget *parent) : QMainWindow(parent),
                                                                                       TCPPort(TCPPort),
                                                                                       widget(new QWidget()),
                                                                                       grid(new QGridLayout()),
                                                                                       webSocket(new QTcpSocket()),
                                                                                       contactsPage(new ContactsPage()),
                                                                                       stackedWidget(
                                                                                               new QStackedWidget()),
                                                                                       authorizePage(
                                                                                               new AuthorizePage()),
                                                                                       dialog(nullptr) {
    setWindowTitle(windowTitle);
    connect(authorizePage, &AuthorizePage::signalOpenMainWindow, this, &MainWindow::slotOpenMainWindow);
    authorizePage->show();
}

// функция устанавливает соединения между различными сигналами и слотами в приложении,
// чтобы обеспечить правильную обработку событий, такие как чтение данных с сокета, открытие чата и открытие диалога для рассылки сообщений
void MainWindow::initConnections() {
    connect(webSocket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(contactsPage, &ContactsPage::signalContactButtonClicked, this, &MainWindow::slotOpenChat);
    connect(contactsPage, &ContactsPage::signalSendToAllContacts, this, &MainWindow::slotOpenNewsletterDialog);
}

// функция инициализирует макет приложения
void MainWindow::initLayout() {
    grid->addWidget(stackedWidget);
}

// функция инициализирует виджеты приложения
void MainWindow::initWidgets() {
    DatabaseManager databaseManager;
    // init grid
    setCentralWidget(widget);
    widget->setLayout(grid);

    // init stack
    stackedWidget->addWidget(contactsPage);

    // init websocket
    webSocket->connectToHost(QHostAddress::LocalHost, TCPPort);
    if (webSocket->isOpen()) {
        qDebug() << "webSocket Opened";
    } else {
        qDebug() << "webSocket Not opened";
    }

    // fill contacts list
    auto contacts = databaseManager.selectAllContactsForUser(user_id, contactsPage);
    for (auto &contact: contacts) {
        contactsPage->addItemContact(contact);
    }
}

// функция отправляет данные на сервер в формате JSON
// данные включают отправителя, получателя и текст сообщения
void MainWindow::slotSendToServer() {
    auto chat = dynamic_cast<ChatPage *>(stackedWidget->currentWidget());
    auto text = chat->getMessage();
    if (text.size() == 0)
        return;


    QDataStream clientStream(webSocket);

    clientStream.setVersion(QDataStream::Qt_6_2);

    QJsonObject message;
    message["type"] = QStringLiteral("message");
    message["text"] = text.toStdString().c_str();
    message["sender_id"] = QString::number(chat->getSenderId());
    message["receiver_id"] = QString::number(chat->getReceiverId());

    clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
}

// функция отвечает за открытие чата
void MainWindow::slotOpenChat(quint64 receiver_id) {
    auto chat_page = new ChatPage(user_id, receiver_id, this);
    chat_page->setUserId(user_id);
    chat_page->setReceiverId(receiver_id);
    stackedWidget->addWidget(chat_page);
    stackedWidget->setCurrentIndex(stackedWidget->count() - 1);
    qDebug() << "Chat added to stack successfully";
}

// функция отвечает за удаление виджета чата из stackedWidget
void MainWindow::slotRemoveChatWidget() {
    stackedWidget->removeWidget(stackedWidget->currentWidget());
    stackedWidget->setCurrentIndex(stackedWidget->count() - 1);
}

// функция вызывается при открытии главного окна после авторизации и выполняет несколько действий,
// такие как установка инициализации макета, виджетов и соединений
void MainWindow::slotOpenMainWindow() {
    user_id = authorizePage->getUserId();
    contactsPage->setUserId(user_id);
    initLayout();
    initWidgets();
    initConnections();
    authorizePage->close();
    show();
}

// функция отвечает за чтение данных с сокета и обработку JSON-данных, полученных от сервера
void MainWindow::readSocket() {
    QByteArray jsonData; // создаем пустой массив байтов для хранения данных
    QDataStream socketStream(webSocket); // создаем объект для сетевого взаимодействия
    socketStream.setVersion(QDataStream::Qt_6_2); // устанавливаем версию протокола для потока данных

    for (;;) {
        // запускаем цикл
        socketStream.startTransaction(); // Начинается транзакция в socketStream
        // это позволяет откатиться к предыдущему состоянию потока данных, если произойдет ошибка чтения данных
        socketStream >> jsonData; // попытка чтения данных из сокета и запись их в массив байтов jsonData.
        if (socketStream.commitTransaction()) { // если чтение данных успешно, то идем дальше...
            QJsonParseError parseError; // создаем объект QJsonParseError для отслеживания ошибок парсинга JSON
            // пытаемся создать объект QJsonDocument из данных jsonData
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError) { // проверяем что при парсинге JSON не возникло ошибок
                if (jsonDoc.isObject()) // проверяем что jsonDoc является объектом JSON (валидным JSON-объектом)
                    jsonReceived(jsonDoc.object()); // если JSON валиден, вызывается функция jsonReceived,
                    // передавая в нее объект JSON для дальнейшей обработки.
            }
            // если при парсинге были ошибки переходим к следующей ветке кода
        } else {
            // в случае ошибки при чтении данных, цикл завершится, и программа будет ожидать новых данных.
            break;
        }
    }
}

// функция обрабатывает JSON-данные полученные от сервера - отвечает за обработку входящих сообщений
void MainWindow::jsonReceived(const QJsonObject &docObj) {
    DatabaseManager databaseManager; // создаем объект для взаимодействия с БД
    const QJsonValue typeVal = docObj.value(QLatin1String("type")); // извлекаем и сохраняем тип сообщения

    // проверяем является ли typeVal пустым или не является строкой
    // если это так, функция завершается, так как ожидается, что сообщение должно иметь тип "message"
    if (typeVal.isNull() || !typeVal.isString())
        return;

    if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0) {

        // извлекаем значения
        const QJsonValue textVal = docObj.value(QLatin1String("text"));
        const QJsonValue sender_id = docObj.value(QLatin1String("sender_id"));
        const QJsonValue receiver_id = docObj.value(QLatin1String("receiver_id"));

        // проверки чтобы убедиться, что textVal, sender_id и receiver_id являются допустимыми значениями: они не пусты и являются строками
        if (textVal.isNull() || !textVal.isString()) {
            return;
        }
        if (sender_id.isNull() || !sender_id.isString()) {
            return;
        }
        if (receiver_id.isNull() || !receiver_id.isString()) {
            return;
        }

        qDebug() << "Received from client:" << textVal << " " << sender_id << " "
                 << receiver_id;

        // пробуем получить текущий активный чат (если таковой существует) из stackedWidget
        auto chat = dynamic_cast<ChatPage *>(stackedWidget->currentWidget());
        if (chat != nullptr) { // проверяем был ли найдет чат
            auto s_id = sender_id.toString().toULongLong(); // преобразуем что-бы получить числовой идентификатор отправителя
            if (chat->getSenderId() == s_id) { // проверяем, является ли идентификатор отправителя сообщения идентификатором отправителя активного чата
                chat->addMessage("me", textVal.toString()); // добавляем сообщение в чат
                // делаем вставку в БД
                databaseManager.insertMessage(chat->getSenderId(), chat->getReceiverId(), chat->getMessage());
            }
             if (chat->getReceiverId() == s_id && chat->getSenderId() == receiver_id.toString().toULongLong()) {
                // добавляем сообщение в активный чат с меткой и текстом, полученными из сообщения
                chat->addMessage(receiver_id.toString(), textVal.toString());
            }
        } else {
            // если активный чат не соответствует отправителю и получателю сообщения, данные о сообщении записываются в базу данных через databaseManager
            databaseManager.insertMessage(sender_id.toString().toULongLong(), receiver_id.toString().toULongLong(), textVal.toString().toUtf8());
        }
    }
}

// функция открывает диалоговое окно для отправки рассылки сообщений
void MainWindow::slotOpenNewsletterDialog() {
    auto w = NewsletterDialog(webSocket, user_id, this);
    w.exec();
}