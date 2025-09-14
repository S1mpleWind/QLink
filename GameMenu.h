#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QFrame>


class GameMenu : public QWidget {
    Q_OBJECT

public:
    // ===================== 构造与析构 =====================
    explicit GameMenu(QWidget* parent = nullptr); // 构造函数
    ~GameMenu(); // 析构函数


    // ===================== 菜单显示与操作 =====================

    // 设置最终得分文本
    void setFinalScoreText(QString text);

    // 显示暂停菜单
    void showPauseMenu();

    // 显示结束菜单
    void showEndMenu();

    // 隐藏菜单
    void hideMenu();


signals:
    // ===================== 信号 =====================

    void resumeClicked();   // 继续游戏
    void restartClicked();  // 重新开始
    void exitClicked();     // 退出游戏
    void fileClicked();     // 文件操作


private:
    // ===================== UI 组件 =====================

    QStackedWidget* stacked;   // 用来切换 Pause / End

    // Pause 菜单相关控件
    QLabel* pauseTitle;
    QPushButton* resumeButton;
    QPushButton* restartButton;
    QPushButton* exitButton;
    QPushButton* writeButton;

    // End 菜单相关控件
    QLabel* endTitle;
    QLabel* finalScoreLabel;
    QPushButton* endRestartButton;
    QPushButton* endExitButton;


    // ===================== UI 构建 =====================

    // 总体UI初始化
    void setupUI();

    // 初始化暂停菜单
    void setupPauseMenu();

    // 初始化结束菜单
    void setupEndMenu();
};
