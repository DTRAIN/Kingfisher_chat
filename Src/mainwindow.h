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
    QString getUserText();
    void setChatText();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
    void SendText();
    QString recvText();
};

#endif // MAIN_H
