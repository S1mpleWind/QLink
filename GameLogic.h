#pragma once
#include "GameMap.h"
#include "GamePlayer.h"
#include <Qwidget>
#include <QTimer>
#include <QVector>
#include <QPainter>




class GameLogic : public QWidget
{
    Q_OBJECT

public:
    // 构造与析构
    GameLogic(QWidget * parent = nullptr); // 构造函数
    ~GameLogic(); // 析构函数


    // ===================== 玩家相关接口 =====================

    // 获取指定玩家当前位置
    QPoint getPlayerPosition(int playerIndex);

    // 获取指定玩家对象指针
    GamePlayer* getPlayer(int playerIndex);

    // 获取地图对象
    GameMap * getMap() { return gameMap; }

    // 玩家向右移动
    void movePlayerRight();

    // 更新玩家位置
    void updatePlayerPosition(int playerIndex, QPoint pos);

    // 更新边界点集合
    void updateEdgePts();



    // ===================== 游戏状态与控制 =====================

    // 统计剩余未配对的方块对数
    void remainUnmatchedPairs();

    // 重置游戏（包括地图、玩家等）
    void resetGame();

    // 重置游戏参数
    void resetGamePara();

    // 重置玩家状态
    void resetGamePlayer();

    // 暂停游戏
    void pauseGame() { isPaused = true; }

    // 恢复游戏
    void resumeGame() { isPaused = false; }

    // 设置游戏模式
    void setGameMode(int mode);

    // 获取指定玩家分数
    int getScore(int playerIndex);

    // 获取当前游戏模式
    int getMode() { return gameMode; }


    
    // ===================== 地图与连线判定 =====================

    // 判断两点是否可连通（主接口）
    bool canLink(QPoint pt_1, QPoint pt_2, int index);

protected:
    // 判断某点是否为空格
    bool isEmptyTile(QPoint pt);

    // 判断同一行两个点之间是否无障碍
    bool isClearRow(int row, int col_1, int col_2);

    // 判断同一列两个点之间是否无障碍
    bool isClearCol(int col, int row_1, int row_2);

    // “——”型直线连通
    bool canConnectStraight(QPoint pt_1, QPoint pt_2);

    // “L”型一次转折连通
    bool canConnectOneTurn(QPoint pt_1, QPoint pt_2);

    // “|__|”型两次转折连通
    bool canConnectTwoTurn(QPoint pt_1, QPoint pt_2);

    // 移除两点对应的方块
    void remove(QPoint pt_1, QPoint pt_2);

    // 延迟清除选中状态
    bool delayClearSelect(int playerIndex);


private:
    // ===================== 成员变量 =====================

    GameMap* gameMap;                // 地图对象
    GamePlayer* gamePlayer;          // 玩家1对象
    GamePlayer* gamePlayer2;         // 玩家2对象

    // 初始化玩家
    void initPlayer(int playerCount);

    // 获取随机缓冲区单元格
    QPoint getRandomBufferCell();

    QVector<QPoint> validPath;       // 当前有效路径
    QVector<QPoint> edgePts;         // 边界点集合
    std::unordered_map<int, std::vector<QPoint>> edgePtsByType; // 按类型分类的边界点
    bool detectMode = false;         // 检测模式

    QTimer* gameTimer;               // 游戏主计时器
    int remainingTime;               // 剩余时间
    QTimer* propTimer;               // 道具计时器
    QTimer* hintTimer;               // 提示计时器
    bool hintActive = false;         // 是否处于提示激活状态

    // 控制逻辑
    bool isPaused;                   // 是否暂停
    bool hintMode = false;           // 是否处于提示模式
    QVector<QPoint> hintPath;        // 提示路径
    QVector<QPoint> hintPts;         // 提示点集合
    void showHint();                 // 显示提示
    int gameMode = 1;                // 当前游戏模式


public slots:
    // ===================== 槽函数 =====================

    // 计时器槽函数：倒计时
    void countTime();

    // 随机生成道具
    void generateRandomProp();

    // 保存游戏进度到文件
    void saveGame(const QString& filePath);

    // 从文件加载游戏进度
    void loadGame(const QString& filePath);


signals:
    // ===================== 信号 =====================

    // 触发绘制连线
    void drawLineSignal();

    // 触发删除两个点
    void deletePtSignal(QPoint, QPoint);

    // 没有更多可消除对时发出
    void noMoreValidPairs();

    // 更新分数
    void updateScores(int, int);

    // 更新时间
    void updateTime(int);

    // 时间到
    void timeIsUp();

};
