#include "mainwindow.h"
#include <QApplication>
/*------------------------------------------------------------------------------------------------------------------
--  SOURCE FILE: main.c - A simple chat client. This client is part of a simple chat echo client / server application.
--                        
--  PROGRAM: client
--
--  FUNCTIONS:
--  ***************CLIENT*********************
--  int main(int argc, char** argv)
--  QString getUserText()
--  QString createTextPacket(const QString&, const QString&)
--  void displayBuf(QString)
--  int getSock()
--  void connectServer(QString&)
--  void display(QString)
--  void sendText()
--  void connectToServer(QString&)
--  void disconnectFromServer()
--  void openDlg()
--  void closeDlg()
--  void getServInfo()
--  void addChatText(QString)
--  ******************************************
--  ****************NETWORK*******************
--  int create_sock(void);
--  int connect_client_sock(int, char*)
--  int send_packet(int, char*)
--  int recv_packet(int, char*)
--  void closeConnection(int)
--  ******************************************
--  ****************ERRORS********************
--  void serv_err(int, char*)
--  ******************************************       
--
--
--  DATE: March 20, 2011
--
--  REVISIONS: (Date and Description)
--
--  DESIGNER: Duncan Donaldson
--
--  PROGRAMMER: Duncan Donaldson
--
--  NOTES: This program is a simple chat client. It allows the user to connect to an echo server, then send a message
--         to the server. Upon receiving a message the server will echo it back to all clients. The client uses a C
--         backend created with the Berkeley Sockets API, combined with a QT front end, to create the full chat client
--         experience.
--
----------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow* mw = new MainWindow;

    mw->show();
    return app.exec();

}
