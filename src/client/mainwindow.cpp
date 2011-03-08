#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
}

MainWindow::MainWindow(QWidget *parent, QString& group, QString& user, int sock):
  QMainWindow(parent), groupname_(group), username_(user), sock_(sock), ui_(new Ui::MainWindow) {

    ui_->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::sendText() {
    QString text(this->getUserText());
    // createTextPacket(text, this->username_, this->groupname_);
    // networkSend(text.toAscii());
}

QString MainWindow::getUserText() {
    //return textEdit.toPlainText();
    return QString();
}