/****************************************************************************
** Meta object code from reading C++ file 'GameLogic.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../GameLogic.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GameLogic.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
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
struct qt_meta_tag_ZN9GameLogicE_t {};
} // unnamed namespace

template <> constexpr inline auto GameLogic::qt_create_metaobjectdata<qt_meta_tag_ZN9GameLogicE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GameLogic",
        "drawLineSignal",
        "",
        "deletePtSignal",
        "noMoreValidPairs",
        "updateScores",
        "updateTime",
        "timeIsUp",
        "setNormalMode",
        "receiveScores",
        "countTime",
        "generateRandomProp",
        "saveGame",
        "filePath",
        "loadGame"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'drawLineSignal'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'deletePtSignal'
        QtMocHelpers::SignalData<void(QPoint, QPoint)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QPoint, 2 }, { QMetaType::QPoint, 2 },
        }}),
        // Signal 'noMoreValidPairs'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'updateScores'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Signal 'updateTime'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Signal 'timeIsUp'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setNormalMode'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'receiveScores'
        QtMocHelpers::SlotData<void(int)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'countTime'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'generateRandomProp'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'saveGame'
        QtMocHelpers::SlotData<void(const QString &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Slot 'loadGame'
        QtMocHelpers::SlotData<void(const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GameLogic, qt_meta_tag_ZN9GameLogicE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GameLogic::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9GameLogicE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9GameLogicE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9GameLogicE_t>.metaTypes,
    nullptr
} };

void GameLogic::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GameLogic *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->drawLineSignal(); break;
        case 1: _t->deletePtSignal((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[2]))); break;
        case 2: _t->noMoreValidPairs(); break;
        case 3: _t->updateScores((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->updateTime((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->timeIsUp(); break;
        case 6: _t->setNormalMode(); break;
        case 7: _t->receiveScores((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->countTime(); break;
        case 9: _t->generateRandomProp(); break;
        case 10: _t->saveGame((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->loadGame((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GameLogic::*)()>(_a, &GameLogic::drawLineSignal, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameLogic::*)(QPoint , QPoint )>(_a, &GameLogic::deletePtSignal, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameLogic::*)()>(_a, &GameLogic::noMoreValidPairs, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameLogic::*)(int )>(_a, &GameLogic::updateScores, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameLogic::*)(int )>(_a, &GameLogic::updateTime, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameLogic::*)()>(_a, &GameLogic::timeIsUp, 5))
            return;
    }
}

const QMetaObject *GameLogic::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameLogic::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9GameLogicE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GameLogic::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void GameLogic::drawLineSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GameLogic::deletePtSignal(QPoint _t1, QPoint _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void GameLogic::noMoreValidPairs()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void GameLogic::updateScores(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void GameLogic::updateTime(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void GameLogic::timeIsUp()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
