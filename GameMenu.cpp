#include "GameMenu.h"

GameMenu::GameMenu(QWidget* parent) : QWidget(parent) {
    setupUI();
    hide();
}

GameMenu::~GameMenu() {}


void GameMenu::setupUI() {

    // 启用”StyledBackground“绘制半透明背景
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background: rgba(0,0,0,120);");

    //创建垂直布局
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setAlignment(Qt::AlignCenter);

    //创建一个stackwidget用于存放两个菜单
    stacked = new QStackedWidget(this);
    root->addWidget(stacked, 0, Qt::AlignCenter);

    //构建子菜单
    setupPauseMenu();
    setupEndMenu();

    //连接按钮的信号与槽
    connect(resumeButton,     &QPushButton::clicked, this, &GameMenu::resumeClicked);

    connect(restartButton,    &QPushButton::clicked, this, &GameMenu::restartClicked);
    connect(exitButton,       &QPushButton::clicked, this, &GameMenu::exitClicked);

    connect(endRestartButton, &QPushButton::clicked, this, &GameMenu::restartClicked);
    connect(endExitButton,    &QPushButton::clicked, this, &GameMenu::exitClicked);

    connect(writeButton,      &QPushButton::clicked, this, &GameMenu::fileClicked);
}


//构建暂停菜单
void GameMenu::setupPauseMenu()
{
    //设置panel的格式
    auto* pausePanel = new QFrame(this);
    pausePanel->setObjectName("menuPanel");
    pausePanel->setFixedSize(380, 260);
    pausePanel->setStyleSheet("#menuPanel { background: rgba(255,255,255,230); border-radius: 16px; }");

    auto* pv = new QVBoxLayout(pausePanel);
    pv->setContentsMargins(24, 24, 24, 24);
    pv->setSpacing(12);

    pauseTitle = new QLabel("Paused", pausePanel);
    pauseTitle->setAlignment(Qt::AlignCenter);
    pauseTitle->setStyleSheet("font-size: 20px; font-weight: 600;");

    //暂停菜单中一共有一个“标签”，四个“按钮”
    resumeButton  = new QPushButton("Resume",  pausePanel);
    restartButton = new QPushButton("Restart", pausePanel);
    exitButton    = new QPushButton("Exit",    pausePanel);
    writeButton   = new QPushButton("File",    pausePanel);

    //把所有控件加进去 ，同时中间加一个留白
    pv->addWidget(pauseTitle);
    pv->addStretch();
    pv->addWidget(resumeButton);
    pv->addWidget(restartButton);
    pv->addWidget(exitButton);
    pv->addWidget(writeButton);

    stacked->addWidget(pausePanel);
}


//构建结算菜单
void GameMenu::setupEndMenu()
{
    //设置panel的格式
    auto* endPanel = new QFrame(this);
    endPanel->setObjectName("menuPanel");
    endPanel->setFixedSize(380, 260);
    endPanel->setStyleSheet("#menuPanel { background: rgba(255,255,255,230); border-radius: 16px; }");

    auto* ev = new QVBoxLayout(endPanel);
    ev->setContentsMargins(24, 24, 24, 24);
    ev->setSpacing(12);

    //结算菜单中一共有两个“标签”，两个“按钮”
    endTitle = new QLabel("Game Over", endPanel);
    endTitle->setAlignment(Qt::AlignCenter);
    endTitle->setStyleSheet("font-size: 40px; font-weight: 600;");

    finalScoreLabel = new QLabel("Score: 0", endPanel);
    finalScoreLabel->setAlignment(Qt::AlignCenter);
    finalScoreLabel->setStyleSheet("font-size: 16px; color: rgba(255,255,255,230);");

    endRestartButton = new QPushButton("Restart", endPanel);
    endExitButton    = new QPushButton("Exit",    endPanel);

    //把所有控件加进去 ，同时中间加一个留白
    ev->addWidget(endTitle);
    ev->addWidget(finalScoreLabel);
    ev->addStretch();
    ev->addWidget(endRestartButton);
    ev->addWidget(endExitButton);

    stacked->addWidget(endPanel);
}


//展示菜单
void GameMenu::showPauseMenu() {
    stacked->setCurrentIndex(0);
    show();
    raise();
}

void GameMenu::showEndMenu() {
    stacked->setCurrentIndex(1);
    show();
    raise();
}

//隐藏菜单
void GameMenu::hideMenu() {
    hide();
}

void GameMenu::setFinalScoreText(QString s)
{
    finalScoreLabel->setText(s);
    //qDebug()<<"update final score"<<score;
}
