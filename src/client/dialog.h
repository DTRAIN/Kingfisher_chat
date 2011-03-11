#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include "../ui_dialog.h"
namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT 
public:
    Ui::Dialog *ui;
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

};

#endif // DIALOG_H
