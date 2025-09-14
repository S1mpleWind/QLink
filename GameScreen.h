#pragma once
#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QKeyEvent>
#include "GameMap.h"
#include "GameLogic.h"
#include "GameMenu.h"

class GameScreen : public QWidget
{
    Q_OBJECT

public:
    // ===================== 构造与析构 =====================
    GameScreen(QWidget* parent = nullptr); // 构造函数
    ~GameScreen(); // 析构函数


    // ===================== 游戏控制接口 =====================

    // 设置为暂停状态
    void setPause();

    // 恢复游戏
    void recoverGame();


protected:
    // ===================== 事件处理 =====================

    // 绘图事件
    void paintEvent(QPaintEvent* event) override;

    // 键盘按键事件
    void keyPressEvent(QKeyEvent* event) override;

    // 窗口大小变化事件
    void resizeEvent(QResizeEvent* event) override;


private slots:
    // ===================== 槽函数（私有） =====================

    void pauseGame();         // 暂停游戏
    void resumeGame();        // 恢复游戏
    void restartGame();       // 重新开始
    void exitGame();          // 退出游戏
    void onGameOver();        // 游戏结束
    void saveGame();          // 保存游戏

    void updateScoreDisplay(int, int); // 更新分数显示
    void updateTimeDisplay(int);       // 更新时间显示


public slots:
    // ===================== 槽函数（公有） =====================

    void loadGame();          // 加载游戏
    void setMode(int);        // 设置游戏模式


private:
    // ===================== 成员变量 =====================

    GameLogic* gameLogic;     // 游戏逻辑对象
    GameMenu* gameMenu;       // 菜单对象

    QLabel* scoreLabel1;      // 玩家1分数标签
    QLabel* scoreLabel2;      // 玩家2分数标签
    int displayScore = 0;     // 当前显示分数

    QLabel* timeLabel;        // 时间标签
    int remainingTime = 120;  // 剩余时间

    int backgroundIndex;      // 当前背景索引

    QVector<QString> backgroundUrl = {
        ":/img/images/background_img/background_1.png",
        ":/img/images/background_img/background_2.png",
        ":/img/images/background_img/background_3.png",
        ":/img/images/background_img/background_4.png",
        ":/img/images/background_img/background_5.png",
        ":/img/images/background_img/background_6.png"
    };

    // 初始化标签
    void setupLabel();


signals:
    // ===================== 信号 =====================

    void goBackToStartScreen(); // 返回开始界面

};
