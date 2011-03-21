#include "mainwindow.h"
#include "dialog.h"
#include <QApplication>
#include <QDialogButtonBox>
#include <QString>
#include <pthread.h>
extern "C" {
#include "../network/network.h"
}
/* CONSTRUCTOR */
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui_(new Ui::MainWindow) {
    sock_ = 0;
    ui_->setupUi(this);
    connect(ui_->pushButton, SIGNAL(clicked()), this, SLOT(sendText()));
    connect(ui_->actionConnect, SIGNAL(triggered()), this, SLOT(openDlg()));
    connect(ui_->actionQuit_2, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
    connect(ui_->actionClose, SIGNAL(triggered()), this, SLOT(disconnectFromServer()));
}

/* DESTRUCTOR */
MainWindow::~MainWindow() {
    closeConnection(sock_);
    delete MainWindow::ui_;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: getUserText
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: QString MainWindow::getUserText(void)

  --
  -- RETURNS: the user input string.
  --
  -- NOTES:
  -- returns the user input text.
  ----------------------------------------------------------------------------------------------------------------------*/
QString MainWindow::getUserText() {
    QString text(ui_->textEdit->toPlainText());
    ui_->textEdit->clear();
    return text;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: addChatText
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void MainWindow::addChatText(QString text)
--                        - text -- the string to be appended.
  --
  -- RETURNS: void.
  --
  -- NOTES:
  -- appends a string to the main chat window.
  ----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::addChatText(QString text) {
    ui_->plainTextEdit->appendPlainText(text);
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: createTextPacket
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: QString MainWindow::createTextPacket(const QString& data, const QString& username)
  --                                - data     -- the message data
  --                                - username -- the username to be appended to the message data.
  -- RETURNS: a formatted packet.
  --
  -- NOTES:
  -- creates a formatted packet, by combining the user message and user name.
  ----------------------------------------------------------------------------------------------------------------------*/
QString MainWindow::createTextPacket(const QString& data, const QString& username) {
    QString packet;
    packet.append(username);
    packet.append(": ");
    packet.append(data);
    return packet;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: sendText
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void MainWindow::sendText(void)
  --
  -- RETURNS: void.
  --
  -- NOTES:
  -- gets the user text, formats it into a packet, then sends the packet to the server.
  ----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::sendText() {
    QString text(getUserText());
    QString packet(createTextPacket(text, this->username_));
    send_packet(sock_, (char*)packet.toAscii().constData());
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: connectToServer
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void MainWindow::connectToServer(QString& servaddr)
  --                        - servaddr -- the address of the server to connect to.
  --
  -- RETURNS: void.
  --
  -- NOTES:
  -- connects to the server and starts the listen thread.
  ----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::connectToServer(QString& servaddr) {
    if(sock_ != 0) {
        return;
    }
    sock_ = create_sock();
    sock_ = connect_client_sock(sock_, (char*)servaddr.toAscii().constData());
    if(pthread_create(&tid_, NULL, readThread, this) != 0 ) {
	    serv_err(THREAD_ERR, (char*)"pthread_create");
    }
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: disconnectFromServer
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void MainWindow::disconnectFromServer(void)
  --
  -- RETURNS: void.
  --
  -- NOTES:
  -- disconnects from the server and kills the listen thread.
  ----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::disconnectFromServer() {
    if(sock_ == 0) {
        return;
    }
    pthread_cancel(tid_);
    closeConnection(sock_);
    sock_ = 0;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: openDlg
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void MainWindow::openDlg(void)
  --
  -- RETURNS: void.
  --
  -- NOTES:
  -- opens a dialog to get the server address and connects various slots and signals.
  ----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::openDlg() {
    dialog_ = new Dialog();
    dialog_->open();
    connect(this, SIGNAL(connectServer(QString&)), this, SLOT(connectToServer(QString&)));
    connect(dialog_->ui->buttonBox, SIGNAL(accepted()), this, SLOT(getServInfo()));
    connect(dialog_->ui->buttonBox, SIGNAL(rejected()), this, SLOT(closeDlg()));
    connect(this, SIGNAL(display(QString)), this, SLOT(addChatText(QString)));
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: closeDlg
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void MainWindow::closeDlg(void)
  --
  -- RETURNS: void.
  --
  -- NOTES:
  -- closes a dialog to get the server address and disconnects various slots and signals.
  ----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::closeDlg() {
    disconnect(this, SIGNAL(connectServer(QString&)), this, SLOT(connectToServer(QString&)));
    disconnect(dialog_->ui->buttonBox, SIGNAL(accepted()), this, SLOT(getServInfo()));
    disconnect(dialog_->ui->buttonBox, SIGNAL(rejected()), this, SLOT(closeDlg()));
    disconnect(this, SIGNAL(display(QString)), this, SLOT(addChatText(QString)));
    delete dialog_;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: getServInfo
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void MainWindow::getServInfo(void)
  --
  -- RETURNS: void.
  --
  -- NOTES:
  -- gets the server info from the connection dialog.
  ----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::getServInfo() {

    QString servaddr(dialog_->ui->lineEdit->text());
    QString username(dialog_->ui->lineEdit_2->text());

    username_ = username;
    servaddr_ = servaddr;

    emit connectServer(servaddr);
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: readThread
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void* readThread(void* arg)
  --
  -- RETURNS: void.
  --
  -- NOTES:
  -- reads messages from the server, and appends them to the main chat window.
  ----------------------------------------------------------------------------------------------------------------------*/
void* readThread(void* arg) {
    MainWindow* mw = (MainWindow*) arg;

    while(1) {
	    char buf[PACKETSIZE];
	    recv_packet(mw->getSock(), buf);
        emit(mw->displayBuf(buf));
    }
    return arg;
}

