#pragma once
#include <QtTest/QtTest>

#include "GameLogic.h"
#include "GameMap.h"
#include "GamePreset.h"



class Test : public QObject
{
    Q_OBJECT

private slots:
    // ===================== 单元测试用例 =====================

    // 测试地图是否合法
    void test_legalMap();

    // 测试同一点判定
    void test_samePoint();

    // 测试类型不匹配
    void test_typeMismatch();

    // 测试空格判定
    void test_emptyBox();

    // 测试直线连通
    void test_straightConnect();

    // 测试一次转折连通
    void test_oneTurnConnect();

    // 测试两次转折连通
    void test_twoTurnConnect();

    // 测试不可连通情况
    void test_cannotConnect();
};
