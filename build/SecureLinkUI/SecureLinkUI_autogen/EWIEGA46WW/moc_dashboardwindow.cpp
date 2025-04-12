/****************************************************************************
** Meta object code from reading C++ file 'dashboardwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.16)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../SecureLinkUI/dashboardwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dashboardwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.16. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DashboardWindow_t {
    QByteArrayData data[9];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DashboardWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DashboardWindow_t qt_meta_stringdata_DashboardWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "DashboardWindow"
QT_MOC_LITERAL(1, 16, 17), // "openSmartLockPage"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 14), // "openCameraPage"
QT_MOC_LITERAL(4, 50, 18), // "openGarageDoorPage"
QT_MOC_LITERAL(5, 69, 18), // "openSmartLightPage"
QT_MOC_LITERAL(6, 88, 18), // "updateServerStatus"
QT_MOC_LITERAL(7, 107, 7), // "message"
QT_MOC_LITERAL(8, 115, 5) // "color"

    },
    "DashboardWindow\0openSmartLockPage\0\0"
    "openCameraPage\0openGarageDoorPage\0"
    "openSmartLightPage\0updateServerStatus\0"
    "message\0color"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DashboardWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    0,   41,    2, 0x06 /* Public */,
       5,    0,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    8,

       0        // eod
};

void DashboardWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DashboardWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->openSmartLockPage(); break;
        case 1: _t->openCameraPage(); break;
        case 2: _t->openGarageDoorPage(); break;
        case 3: _t->openSmartLightPage(); break;
        case 4: _t->updateServerStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DashboardWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DashboardWindow::openSmartLockPage)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DashboardWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DashboardWindow::openCameraPage)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DashboardWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DashboardWindow::openGarageDoorPage)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DashboardWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DashboardWindow::openSmartLightPage)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DashboardWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_DashboardWindow.data,
    qt_meta_data_DashboardWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DashboardWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DashboardWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DashboardWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int DashboardWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DashboardWindow::openSmartLockPage()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DashboardWindow::openCameraPage()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DashboardWindow::openGarageDoorPage()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void DashboardWindow::openSmartLightPage()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
