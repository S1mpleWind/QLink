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
    explicit GameMenu(QWidget* parent = nullptr);
    ~GameMenu();

    void setFinalScore(int);

    void showPauseMenu();   // 显示暂停菜单
    void showEndMenu();     // 显示结束菜单
    void hideMenu();        // 隐藏

signals:
    void resumeClicked();   // 继续游戏
    void restartClicked();  // 重新开始
    void exitClicked();     // 退出游戏

private:
    QStackedWidget* stacked;   // 用来切换 Pause / End

    // Pause 菜单
    QLabel* pauseTitle;
    QPushButton* resumeButton;
    QPushButton* restartButton;
    QPushButton* exitButton;

    // End 菜单
    QLabel* endTitle;
    QLabel* finalScoreLabel;
    QPushButton* endRestartButton;
    QPushButton* endExitButton;

    void setupUI();
    void setupPauseMenu();
    void setupEndMenu();
};
