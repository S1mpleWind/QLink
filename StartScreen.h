#pragma once
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QLayout>
#include <QPushButton>


class StartScreen : public QWidget
{
    Q_OBJECT

public:
    // ===================== 构造与析构 =====================
    explicit StartScreen(QWidget* parent = nullptr); // 构造函数
    ~StartScreen();                                 // 析构函数


    // ===================== 按钮获取接口 =====================

    // 获取帮助按钮
    QPushButton* getHelperButton();

    // 获取开始按钮
    QPushButton* getStartButton();

    // 获取模式切换按钮
    QPushButton* getModeButton();

    // 获取读取存档按钮
    QPushButton* getReadButton();

    // 获取退出按钮
    QPushButton* getQuitButton();


protected:
    // ===================== 事件处理 =====================

    // 绘图事件
    void paintEvent(QPaintEvent* event) override;

    // 窗口大小变化事件
    void resizeEvent(QResizeEvent* event) override;


private:
    // ===================== UI 组件 =====================

    QHBoxLayout* mainLayout;      // 主布局
    QWidget* buttonContainer;     // 按钮容器
    QVBoxLayout* menuLayout;      // 菜单布局

    QPushButton helperButton;     // 帮助按钮
    QPushButton startButton;      // 开始按钮
    QPushButton modeButton;       // 模式切换按钮
    QPushButton readButton;       // 读取存档按钮
    QPushButton quitButton;       // 退出按钮

    // 调整按钮大小
    void adjustButtonSizes();

    int gameMode;                // 当前游戏模式


private slots:
    // ===================== 槽函数 =====================

    // 切换游戏模式
    void changeMode();


signals:
    // ===================== 信号 =====================

    // 切换模式信号
    void switchmode(int);
};
