/****************************************************************************
** Meta object code from reading C++ file 'dropbutton.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Xavier/dropbutton.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dropbutton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DropButton_t {
    QByteArrayData data[7];
    char stringdata[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DropButton_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DropButton_t qt_meta_stringdata_DropButton = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DropButton"
QT_MOC_LITERAL(1, 11, 7), // "changed"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 16), // "const QMimeData*"
QT_MOC_LITERAL(4, 37, 8), // "mimeData"
QT_MOC_LITERAL(5, 46, 7), // "dropped"
QT_MOC_LITERAL(6, 54, 5) // "clear"

    },
    "DropButton\0changed\0\0const QMimeData*\0"
    "mimeData\0dropped\0clear"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DropButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       1,    0,   42,    2, 0x26 /* Public | MethodCloned */,
       5,    1,   43,    2, 0x06 /* Public */,
       5,    0,   46,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void DropButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DropButton *_t = static_cast<DropButton *>(_o);
        switch (_id) {
        case 0: _t->changed((*reinterpret_cast< const QMimeData*(*)>(_a[1]))); break;
        case 1: _t->changed(); break;
        case 2: _t->dropped((*reinterpret_cast< const QMimeData*(*)>(_a[1]))); break;
        case 3: _t->dropped(); break;
        case 4: _t->clear(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DropButton::*_t)(const QMimeData * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DropButton::changed)) {
                *result = 0;
            }
        }
        {
            typedef void (DropButton::*_t)(const QMimeData * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DropButton::dropped)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject DropButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_DropButton.data,
      qt_meta_data_DropButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DropButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DropButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DropButton.stringdata))
        return static_cast<void*>(const_cast< DropButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int DropButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
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
void DropButton::changed(const QMimeData * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void DropButton::dropped(const QMimeData * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
