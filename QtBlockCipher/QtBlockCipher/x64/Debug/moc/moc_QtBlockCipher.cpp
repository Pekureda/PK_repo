/****************************************************************************
** Meta object code from reading C++ file 'QtBlockCipher.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../QtBlockCipher.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtBlockCipher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtBlockCipher_t {
    const uint offsetsAndSize[30];
    char stringdata0[356];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_QtBlockCipher_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_QtBlockCipher_t qt_meta_stringdata_QtBlockCipher = {
    {
QT_MOC_LITERAL(0, 13), // "QtBlockCipher"
QT_MOC_LITERAL(14, 20), // "on_RunButton_clicked"
QT_MOC_LITERAL(35, 0), // ""
QT_MOC_LITERAL(36, 23), // "on_actionExit_triggered"
QT_MOC_LITERAL(60, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(84, 27), // "on_KeyTextField_textChanged"
QT_MOC_LITERAL(112, 25), // "on_actionSaveAs_triggered"
QT_MOC_LITERAL(138, 30), // "on_actionSave_key_as_triggered"
QT_MOC_LITERAL(169, 27), // "on_actionOpen_key_triggered"
QT_MOC_LITERAL(197, 24), // "on_actionAbout_triggered"
QT_MOC_LITERAL(222, 38), // "on_AESModeComboBox_currentInd..."
QT_MOC_LITERAL(261, 5), // "index"
QT_MOC_LITERAL(267, 35), // "on_DivideIntoBlocksCheckBox_t..."
QT_MOC_LITERAL(303, 7), // "checked"
QT_MOC_LITERAL(311, 44) // "on_InitializationVectorTextFi..."

    },
    "QtBlockCipher\0on_RunButton_clicked\0\0"
    "on_actionExit_triggered\0on_actionOpen_triggered\0"
    "on_KeyTextField_textChanged\0"
    "on_actionSaveAs_triggered\0"
    "on_actionSave_key_as_triggered\0"
    "on_actionOpen_key_triggered\0"
    "on_actionAbout_triggered\0"
    "on_AESModeComboBox_currentIndexChanged\0"
    "index\0on_DivideIntoBlocksCheckBox_toggled\0"
    "checked\0on_InitializationVectorTextField_textChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtBlockCipher[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x08,    1 /* Private */,
       3,    0,   81,    2, 0x08,    2 /* Private */,
       4,    0,   82,    2, 0x08,    3 /* Private */,
       5,    0,   83,    2, 0x08,    4 /* Private */,
       6,    0,   84,    2, 0x08,    5 /* Private */,
       7,    0,   85,    2, 0x08,    6 /* Private */,
       8,    0,   86,    2, 0x08,    7 /* Private */,
       9,    0,   87,    2, 0x08,    8 /* Private */,
      10,    1,   88,    2, 0x08,    9 /* Private */,
      12,    1,   91,    2, 0x08,   11 /* Private */,
      14,    0,   94,    2, 0x08,   13 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void,

       0        // eod
};

void QtBlockCipher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QtBlockCipher *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_RunButton_clicked(); break;
        case 1: _t->on_actionExit_triggered(); break;
        case 2: _t->on_actionOpen_triggered(); break;
        case 3: _t->on_KeyTextField_textChanged(); break;
        case 4: _t->on_actionSaveAs_triggered(); break;
        case 5: _t->on_actionSave_key_as_triggered(); break;
        case 6: _t->on_actionOpen_key_triggered(); break;
        case 7: _t->on_actionAbout_triggered(); break;
        case 8: _t->on_AESModeComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_DivideIntoBlocksCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_InitializationVectorTextField_textChanged(); break;
        default: ;
        }
    }
}

const QMetaObject QtBlockCipher::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_QtBlockCipher.offsetsAndSize,
    qt_meta_data_QtBlockCipher,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QtBlockCipher_t
, QtPrivate::TypeAndForceComplete<QtBlockCipher, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *QtBlockCipher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtBlockCipher::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtBlockCipher.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int QtBlockCipher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
