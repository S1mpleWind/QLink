#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class EndMenu : public QWidget {
    Q_OBJECT

public:
    explicit EndMenu(QWidget* parent = nullptr);
    ~EndMenu();

    void showPauseMenu();
    void showGameOver();
    void hideMenu();

signals:
    void resumeClicked();      // 继续游戏
    void restartClicked();     // 重新开始
    void exitClicked();        // 退出游戏

private:
    QLabel* titleLabel;        // 菜单标题（暂停/游戏结束）
    QPushButton* resumeButton;
    QPushButton* restartButton;
    QPushButton* exitButton;
    QVBoxLayout* layout;

    void setupUI();
};
