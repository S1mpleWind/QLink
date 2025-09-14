#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include "StartScreen.h"
#include "GameScreen.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // ===================== 构造与析构 =====================
    MainWindow(QWidget *parent = nullptr); // 构造函数
    ~MainWindow();                        // 析构函数


    // ===================== 界面切换与初始化 =====================

    // 显示开始界面
    void showStartScreen();

    // 显示游戏界面
    void showGameScreen();

    // 初始化主窗口
    void initMainWindow();


private:
    // ===================== 成员变量 =====================

    Ui::MainWindow *ui;           // UI指针
    QStackedWidget *stackWidget;  // 界面堆栈

    StartScreen *startScreen;     // 开始界面
    GameScreen *gameScreen;       // 游戏界面


private slots:
    // ===================== 槽函数 =====================

    // 加载游戏
    void loadGame();

    // 打开帮助文档
    void openHelpDoc();

};

