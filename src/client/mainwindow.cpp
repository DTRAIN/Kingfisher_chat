#include "mainwindow.h"
#include "dialog.h"
#include <QApplication>
#include <QDialogButtonBox>
#include <QString>
#include <pthread.h>
extern "C" {
#include "../network/network.h"
}
int MainWindow::sid_;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
    connect(ui_->pushButton, SIGNAL(clicked()), this, SLOT(sendText()));
    connect(ui_->actionConnect, SIGNAL(triggered()), this, SLOT(openDlg()));
    connect(ui_->actionQuit_2, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
}

MainWindow::~MainWindow() {
    delete MainWindow::ui_;
}

QString MainWindow::getUserText() {
    QString text(ui_->textEdit->toPlainText());
    ui_->textEdit->clear();
    return text;
}

void MainWindow::addChatText(QString text) {
    ui_->plainTextEdit->appendPlainText(text);
}

QString MainWindow::createTextPacket(const QString& data, const QString& username) {
    QString packet;
    packet.append(username);
    packet.append(": ");
    packet.append(data);
    return packet;
}

void MainWindow::sendText() {
    QString text(getUserText());
    QString packet(createTextPacket(text, this->username_));
    send_packet(sock_, (char*)packet.toAscii().constData());
}

void MainWindow::connectToServer(QString& servaddr) {
    pthread_t tid;
    sock_ = create_sock();
    sock_ = connect_client_sock(sock_, (char*)servaddr.toAscii().constData());
    if(pthread_create(&tid, NULL, readThread, this) != 0 ) {
	    serv_err(THREAD_ERR, (char*)"pthread_create");
    }
}

void MainWindow::openDlg() {
    dialog_ = new Dialog();
    dialog_->open();
    connect(this, SIGNAL(connectServer(QString&)), this, SLOT(connectToServer(QString&)));
    connect(dialog_->ui->buttonBox, SIGNAL(accepted()), this, SLOT(getServInfo()));
    connect(dialog_->ui->buttonBox, SIGNAL(rejected()), this, SLOT(closeDlg()));
    connect(this, SIGNAL(display(QString)), this, SLOT(addChatText(QString)));
}

void MainWindow::closeDlg() {
    delete dialog_;
}

void MainWindow::getServInfo() {

    QString servaddr(dialog_->ui->lineEdit->text());
    QString username(dialog_->ui->lineEdit_2->text());

    username_ = username;
    servaddr_ = servaddr;

    emit connectServer(servaddr);

}

void* readThread(void* arg) {
    MainWindow* mw = (MainWindow*) arg;

    while(1) {
	    char buf[PACKETSIZE];
	    if(recv_packet(mw->getSock(), buf) != 0) {
	        emit(mw->displayBuf(buf));
	    }
    }
    return arg;
}

