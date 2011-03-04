#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QString>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(QWidget *parent, QString& group,
                        QString& user, int sock);
    QString getUserText();
    void setChatText();
    ~MainWindow();

public slots:
    void sendText();
    QString recvText();

private:
    Ui::MainWindow *ui_;
    QString groupname_;
    QString username_;
    int sock_;
};

#endif // MAIN_H
