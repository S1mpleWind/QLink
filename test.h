#pragma once
#include <QtTest/QtTest>

#include "GameLogic.h"
#include "GameMap.h"
#include "GamePreset.h"


class Test : public QObject
{
    Q_OBJECT

private slots:
    void test_legalMap();
    void test_samePoint();
    void test_typeMismatch();
    void test_emptyBox();
    void test_straightConnect();
    void test_oneTurnConnect();
    void test_twoTurnConnect();
    void test_cannotConnect();
};
