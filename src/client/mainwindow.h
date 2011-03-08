#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QString>
#include "../ui_mainwindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(QWidget *parent, QString& group,
                        QString& user, int sock = 0);
    QString getUserText();
    void setChatText();
    ~MainWindow();

public slots:
    void sendText();
    //QString recvText();

private:
    QString groupname_;
    QString username_;
    int sock_;
    Ui::MainWindow *ui_;
};

#endif // MAIN_H