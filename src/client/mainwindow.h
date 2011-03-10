#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QString>
#include "../ui_mainwindow.h"
#include "../ui_dialog.h"
extern "C" {
#include "../network/network.h"
}

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    QString getUserText();
    void addChatText(QString& text);
    QString createTextPacket(const QString& data, const QString& username);

public slots:
    void sendText();
    QString recvText();
    void initNetworking();
    void connectToServer(QString& servaddr);
    void getServInfo();

private:
    QString username_;
    int sock_;
    Ui::MainWindow *ui_;
    Ui::Dialog *dialog_;
};

#endif // MAIN_H
