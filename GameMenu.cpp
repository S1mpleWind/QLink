#include "GameMenu.h"

GameMenu::GameMenu(QWidget* parent) : QWidget(parent) {
    setupUI();
    hide();
}

GameMenu::~GameMenu() {}

void GameMenu::setupUI() {
    // 半透明背景
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background: rgba(0,0,0,120);");

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setAlignment(Qt::AlignCenter);

    stacked = new QStackedWidget(this);
    root->addWidget(stacked, 0, Qt::AlignCenter);

    setupPauseMenu();
    setupEndMenu();


    connect(resumeButton,     &QPushButton::clicked, this, &GameMenu::resumeClicked);

    connect(restartButton,    &QPushButton::clicked, this, &GameMenu::restartClicked);
    connect(exitButton,       &QPushButton::clicked, this, &GameMenu::exitClicked);

    connect(endRestartButton, &QPushButton::clicked, this, &GameMenu::restartClicked);
    connect(endExitButton,    &QPushButton::clicked, this, &GameMenu::exitClicked);

    connect(writeButton,      &QPushButton::clicked, this, &GameMenu::fileClicked);
}

void GameMenu::setupPauseMenu()
{
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

    resumeButton  = new QPushButton("Resume",  pausePanel);
    restartButton = new QPushButton("Restart", pausePanel);
    exitButton    = new QPushButton("Exit",    pausePanel);
    writeButton   = new QPushButton("File",    pausePanel);

    pv->addWidget(pauseTitle);
    pv->addStretch();
    pv->addWidget(resumeButton);
    pv->addWidget(restartButton);
    pv->addWidget(exitButton);
    pv->addWidget(writeButton);

    stacked->addWidget(pausePanel);
}

void GameMenu::setupEndMenu()
{
    auto* endPanel = new QFrame(this);
    endPanel->setObjectName("menuPanel");
    endPanel->setFixedSize(380, 260);
    endPanel->setStyleSheet("#menuPanel { background: rgba(255,255,255,230); border-radius: 16px; }");

    auto* ev = new QVBoxLayout(endPanel);
    ev->setContentsMargins(24, 24, 24, 24);
    ev->setSpacing(12);

    endTitle = new QLabel("Game Over", endPanel);
    endTitle->setAlignment(Qt::AlignCenter);
    endTitle->setStyleSheet("font-size: 40px; font-weight: 600;");

    finalScoreLabel = new QLabel("Score: 0", endPanel);
    finalScoreLabel->setAlignment(Qt::AlignCenter);
    finalScoreLabel->setStyleSheet("font-size: 16px; color: rgba(255,255,255,230);");

    endRestartButton = new QPushButton("Restart", endPanel);
    endExitButton    = new QPushButton("Exit",    endPanel);

    ev->addWidget(endTitle);
    ev->addWidget(finalScoreLabel);
    ev->addStretch();
    ev->addWidget(endRestartButton);
    ev->addWidget(endExitButton);

    stacked->addWidget(endPanel);
}


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

void GameMenu::hideMenu() {
    hide();
}

void GameMenu::setFinalScore(int score)
{
    finalScoreLabel->setText(QString("Score: %1").arg(score));
    qDebug()<<"update final score"<<score;
}
