/****************************************************************************
** Meta object code from reading C++ file 'conditiontab.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../tabs/conditiontab.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'conditiontab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConditionTab_t {
    QByteArrayData data[11];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConditionTab_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConditionTab_t qt_meta_stringdata_ConditionTab = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ConditionTab"
QT_MOC_LITERAL(1, 13, 16), // "requestCondition"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 24), // "removeCurrentConditionAt"
QT_MOC_LITERAL(4, 56, 1), // "i"
QT_MOC_LITERAL(5, 58, 22), // "appendCurrentCondition"
QT_MOC_LITERAL(6, 81, 12), // "conditionObj"
QT_MOC_LITERAL(7, 94, 14), // "initConditions"
QT_MOC_LITERAL(8, 109, 12), // "conditionArr"
QT_MOC_LITERAL(9, 122, 14), // "onStoreChanged"
QT_MOC_LITERAL(10, 137, 7) // "diffObj"

    },
    "ConditionTab\0requestCondition\0\0"
    "removeCurrentConditionAt\0i\0"
    "appendCurrentCondition\0conditionObj\0"
    "initConditions\0conditionArr\0onStoreChanged\0"
    "diffObj"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConditionTab[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,
       5,    1,   43,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   46,    2, 0x0a /* Public */,
       9,    1,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::QJsonObject,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::QJsonArray,    8,
    QMetaType::Void, QMetaType::QJsonObject,   10,

       0        // eod
};

void ConditionTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConditionTab *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestCondition(); break;
        case 1: _t->removeCurrentConditionAt((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->appendCurrentCondition((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 3: _t->initConditions((*reinterpret_cast< QJsonArray(*)>(_a[1]))); break;
        case 4: _t->onStoreChanged((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ConditionTab::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConditionTab::requestCondition)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ConditionTab::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConditionTab::removeCurrentConditionAt)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ConditionTab::*)(QJsonObject );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConditionTab::appendCurrentCondition)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConditionTab::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ConditionTab.data,
    qt_meta_data_ConditionTab,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConditionTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConditionTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConditionTab.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ConditionTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void ConditionTab::requestCondition()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ConditionTab::removeCurrentConditionAt(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ConditionTab::appendCurrentCondition(QJsonObject _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
