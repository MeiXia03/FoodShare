/****************************************************************************
** Meta object code from reading C++ file 'personalinfoview.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../view/personalinfoview.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'personalinfoview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSPersonalInfoViewENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSPersonalInfoViewENDCLASS = QtMocHelpers::stringData(
    "PersonalInfoView",
    "onChangeAvatarClicked",
    "",
    "onUpdateSignatureClicked",
    "onAddFriendClicked",
    "onLogoutClicked",
    "onFeedbackButtonClicked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSPersonalInfoViewENDCLASS_t {
    uint offsetsAndSizes[14];
    char stringdata0[17];
    char stringdata1[22];
    char stringdata2[1];
    char stringdata3[25];
    char stringdata4[19];
    char stringdata5[16];
    char stringdata6[24];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSPersonalInfoViewENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSPersonalInfoViewENDCLASS_t qt_meta_stringdata_CLASSPersonalInfoViewENDCLASS = {
    {
        QT_MOC_LITERAL(0, 16),  // "PersonalInfoView"
        QT_MOC_LITERAL(17, 21),  // "onChangeAvatarClicked"
        QT_MOC_LITERAL(39, 0),  // ""
        QT_MOC_LITERAL(40, 24),  // "onUpdateSignatureClicked"
        QT_MOC_LITERAL(65, 18),  // "onAddFriendClicked"
        QT_MOC_LITERAL(84, 15),  // "onLogoutClicked"
        QT_MOC_LITERAL(100, 23)   // "onFeedbackButtonClicked"
    },
    "PersonalInfoView",
    "onChangeAvatarClicked",
    "",
    "onUpdateSignatureClicked",
    "onAddFriendClicked",
    "onLogoutClicked",
    "onFeedbackButtonClicked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPersonalInfoViewENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   44,    2, 0x08,    1 /* Private */,
       3,    0,   45,    2, 0x08,    2 /* Private */,
       4,    0,   46,    2, 0x08,    3 /* Private */,
       5,    0,   47,    2, 0x08,    4 /* Private */,
       6,    0,   48,    2, 0x08,    5 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject PersonalInfoView::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSPersonalInfoViewENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPersonalInfoViewENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPersonalInfoViewENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PersonalInfoView, std::true_type>,
        // method 'onChangeAvatarClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onUpdateSignatureClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddFriendClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onLogoutClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFeedbackButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void PersonalInfoView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PersonalInfoView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onChangeAvatarClicked(); break;
        case 1: _t->onUpdateSignatureClicked(); break;
        case 2: _t->onAddFriendClicked(); break;
        case 3: _t->onLogoutClicked(); break;
        case 4: _t->onFeedbackButtonClicked(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *PersonalInfoView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PersonalInfoView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPersonalInfoViewENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PersonalInfoView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
