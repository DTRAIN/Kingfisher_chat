#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QString>
#include "../ui_mainwindow.h"
#include "dialog.h"
extern "C" {
#include "../network/network.h"
}

namespace Ui {
    class MainWindow;
}

void* readThread(void*);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    QString getUserText();
    QString createTextPacket(const QString& data, const QString& username);
    void displayBuf(QString buf) {
        emit(display(buf));
    }
    int getSock() {
	    return sock_;
    }
    bool getLog() {
        return log_;
    }
    int getLogFD() {
        return logfd_;
    }

signals:
    void connectServer(QString& servaddr);
    void display(QString text);
public slots:
    void sendText();
    void connectToServer(QString& servaddr);
    void disconnectFromServer();
    void openDlg();
    void closeDlg();
    void getServInfo();
    void addChatText(QString text);
private:
    QString username_;
    QString servaddr_;
    bool log_;
    int logfd_;
    int sock_;
    Ui::MainWindow *ui_;
    Dialog *dialog_;
    pthread_t tid_;
};

#endif // MAIN_H
