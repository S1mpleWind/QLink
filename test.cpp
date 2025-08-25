#define QT_NO_DEBUG_OUTPUT
#include "test.h"



void Test::test_legalMap()
{
    GameLogic logic(nullptr);
    QPoint p1(1,1);
    QVERIFY(!logic.canLink(p1, p1, 1));
}

// 相同点不能连接
void Test::test_samePoint() {

    GameLogic logic(nullptr);
    QPoint p1(1,1);
    GameMap* map = logic.getMap();

    map->setBoxType(1,1,1);
    QVERIFY(!logic.canLink(p1, p1, 1));
}

// 类型不同不能连接
void Test::test_typeMismatch() {
    GameMap map;
    GameLogic logic(&map);
    QPoint p1(0,0), p2(1,0);
    map.setBoxType(0,0,1);
    map.setBoxType(1,0,2);
    QVERIFY(!logic.canLink(p1, p2, 1));
}

// 一个为空也不能连接
void Test::test_emptyBox() {
    GameMap map;
    GameLogic logic(&map);
    QPoint p1(0,0), p2(1,0);
    map.setBoxType(0,0,0);
    map.setBoxType(1,0,1);
    QVERIFY(!logic.canLink(p1, p2, 1));
}

// 直连成功
void Test::test_straightConnect() {
    GameLogic logic(nullptr);
    GameMap* map = logic.getMap();
    QPoint p1(0,0), p2(0,1);
    map->setBoxType(0,0,1);
    map->setBoxType(0,1,1);
    QVERIFY(logic.canLink(p1, p2, 1));
}


// 一折成功
void Test::test_oneTurnConnect() {

    GameLogic logic(nullptr);
    GameMap* testMap = logic.getMap();

    QVector<QVector<int>> fixedMap = {
        {0, 0, 0, 0},
        {0, 2, 1, 0},
        {0, 0, 2, 0},
        {0, 0, 0, 0}
    };
    int buf = 1 ;
    int col = 2 ;
    int row = 2 ;

    testMap->setColNum(col);
    testMap->setRowNum(row);
    testMap->setBufferNum(buf);
    testMap->setTestMap(fixedMap);


    QPoint p1(1,1), p2(2,2);
    QVERIFY(logic.canLink(p1, p2, 1));
}

// 二折成功
void Test::test_twoTurnConnect() {

    GameLogic logic(nullptr);
    GameMap* testMap = logic.getMap();


    QVector<QVector<int>> fixedMap = {
        {0, 0, 0, 0, 0},
        {0, 2, 1, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 2, 0, 1, 0},
        {0, 0, 0, 0, 0}
    };

    int buf = 1 ;
    int col = 3 ;
    int row = 3 ;


    testMap->setColNum(col);
    testMap->setRowNum(row);
    testMap->setBufferNum(buf);
    testMap->setTestMap(fixedMap);


    QPoint p1(1,1), p2(1,3);
    QVERIFY(logic.canLink(p1, p2, 1));
}

// 无法连接
void Test::test_cannotConnect() {
    GameLogic logic(nullptr);
    GameMap* testMap = logic.getMap();


    QVector<QVector<int>> fixedMap = {
        {0, 0, 0, 0, 0},
        {0, 2, 1, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 2, 1, 0},
        {0, 0, 0, 0, 0}
    };

    int buf = 1 ;
    int col = 3 ;
    int row = 3 ;


    testMap->setColNum(col);
    testMap->setRowNum(row);
    testMap->setBufferNum(buf);
    testMap->setTestMap(fixedMap);


    QPoint p1(1,1), p2(2,3);
    QVERIFY(!logic.canLink(p1, p2, 1));
}

// Qt 测试入口
QTEST_MAIN(Test)

