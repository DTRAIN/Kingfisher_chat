/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sun Mar 20 10:49:53 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/client/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   12,   11,   11, 0x05,
      50,   45,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      67,   11,   11,   11, 0x0a,
      78,   12,   11,   11, 0x0a,
     104,   11,   11,   11, 0x0a,
     127,   11,   11,   11, 0x0a,
     137,   11,   11,   11, 0x0a,
     148,   11,   11,   11, 0x0a,
     162,   45,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0servaddr\0connectServer(QString&)\0"
    "text\0display(QString)\0sendText()\0"
    "connectToServer(QString&)\0"
    "disconnectFromServer()\0openDlg()\0"
    "closeDlg()\0getServInfo()\0addChatText(QString)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connectServer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: display((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: sendText(); break;
        case 3: connectToServer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: disconnectFromServer(); break;
        case 5: openDlg(); break;
        case 6: closeDlg(); break;
        case 7: getServInfo(); break;
        case 8: addChatText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::connectServer(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::display(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
