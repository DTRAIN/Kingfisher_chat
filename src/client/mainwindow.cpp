#include "mainwindow.h"
extern "C" {
#include "../network/network.h"
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
    connect(ui_->pushButton, SIGNAL(clicked()), this, SLOT(sendText()));
}

MainWindow::~MainWindow() {
    delete ui_;
}

QString MainWindow::getUserText() {
    QString text(ui_->textEdit->toPlainText());
    ui_->textEdit->clear();
    return text;
}

void MainWindow::addChatText(QString& text) {
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

QString MainWindow::recvText() {
    char buf[PACKETSIZE];
    recv_packet(sock_, buf);
    return QString(buf);
}

void MainWindow::initNetworking() {
    sock_ = create_sock();
}

void MainWindow::connectToServer(QString& servaddr) {
    sock_ = connect_client_sock(sock_, (char*)servaddr.toAscii().constData());
}

void MainWindow::getServInfo() {
    dialog_ = new Ui::Dialog();
    
}
