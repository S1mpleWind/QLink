#pragma once
#include <QWidget>



class GamePlayer : public QWidget {
    Q_OBJECT

public:
    // ===================== 构造与析构 =====================
    GamePlayer();      // 构造函数
    ~GamePlayer();     // 析构函数


    // ===================== 分数相关 =====================

    // 增加固定分数
    void addFixedScore();

    // 增加指定分数
    void addScore(int value);

    // 清空分数
    void clearScore();

    // 获取当前分数
    int getScore() { return score; }

    // 设置分数
    void setScore(int x) { score = x; }


    // ===================== 位置相关 =====================

    // 重置玩家位置
    void resetPlayer() { setPosition(QPoint(0, 0)); }

    // 获取当前位置
    QPoint getPosition() const;

    // 设置当前位置
    void setPosition(const QPoint& pos);


private:
    // ===================== 成员变量 =====================

    QPoint currPos;   // 当前坐标
    QPoint prevPos;   // 上一次坐标
    int score;        // 当前分数

};
