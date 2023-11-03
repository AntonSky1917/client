cmake_minimum_required(VERSION 3.24)
project(client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        WebSockets
        Sql
        REQUIRED)

add_executable(client main.cpp src/MainWindow.cpp src/MainWindow.h src/client/ContactsPage.cpp src/client/ContactsPage.h src/client/ContactListItem.cpp src/client/ContactListItem.h src/client/ChatPage.cpp src/client/ChatPage.h src/client/AuthorizePage.cpp src/client/AuthorizePage.h src/DatabaseManager.cpp src/DatabaseManager.h src/client/NewsletterDialog.cpp src/client/NewsletterDialog.h src/client/RegistrationDialog.cpp src/client/RegistrationDialog.h src/ConnectionInfo.h)
target_link_libraries(iMessage
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::WebSockets
        Qt::Sql
        )

