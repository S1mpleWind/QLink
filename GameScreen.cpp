#include <QVBoxLayout>
#include "GameScreen.h"
#include <QDebug>

GameScreen::GameScreen(QWidget* parent)
    : QWidget(parent)
    , gameLogic(new GameLogic(this))
    , gameMenu(new GameMenu(this))
{

    srand((int)time(0));
    backgroundIndex = std::rand()%6;


    //默认为暂停，游戏开始时自动恢复
    gameLogic->pauseGame();


    //初始化label并假如到菜单栏的水平布局中
    setupLabel();

    QHBoxLayout* menuBar = new QHBoxLayout();
    menuBar->addWidget(scoreLabel1);
    menuBar->addWidget(timeLabel);


    // 创建主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 可选：无边距


    mainLayout->addLayout(menuBar,0);  // 分数在顶部
    mainLayout->addWidget(gameLogic->getMap(),1);            // 把地图控件加入布局



    setLayout(mainLayout);
    // 设置布局到 GameScreen 上


    // 初始隐藏菜单
    gameMenu->hide();
    gameMenu->raise();  // 保证覆盖在最上层
    gameMenu->resize(size());



//按钮的信号与槽的连接
    connect(gameMenu, &GameMenu::resumeClicked, this, &GameScreen::resumeGame);
    connect(gameMenu, &GameMenu::restartClicked, this, &GameScreen::restartGame);
    connect(gameMenu, &GameMenu::exitClicked, this, &GameScreen::exitGame);
    connect(gameMenu, &GameMenu::fileClicked,this,&GameScreen::saveGame);


//更新菜单栏的连接
    connect(gameLogic,&GameLogic::updateTime,this,&GameScreen::updateTimeDisplay);
    connect(gameLogic,&GameLogic::updateScores,this,&GameScreen::updateScoreDisplay);


//时间结束或没有剩余可以连接的方块时，结束游戏
    connect(gameLogic,&GameLogic::timeIsUp,this,&GameScreen::onGameOver);
    connect(gameLogic,&GameLogic::noMoreValidPairs,this,&GameScreen::onGameOver);


}


void GameScreen::setupLabel()
{
    //UI
    scoreLabel1 = new QLabel(this);
    scoreLabel1->setText(QString("Score: %1").arg(displayScore));
    scoreLabel1->setAlignment(Qt::AlignCenter);
    scoreLabel1->setStyleSheet("font: bold 16px; color: white;"); // 可根据需要美化
    scoreLabel1->setFixedHeight(40);

    scoreLabel2 = new QLabel(this);
    scoreLabel2->setText(QString("Score: %1").arg(displayScore));
    scoreLabel2->setAlignment(Qt::AlignCenter);
    scoreLabel2->setStyleSheet("font: bold 16px; color: white;"); // 可根据需要美化
    scoreLabel2->setFixedHeight(40);

    timeLabel = new QLabel(this);
    timeLabel->setText(QString("Remaining time : %1").arg(remainingTime));
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("font: bold 16px; color: white;"); // 可根据需要美化
    timeLabel->setFixedHeight(40);
}


GameScreen:: ~GameScreen()
{

}


//更改游戏状态
void GameScreen::setPause()
{
    gameLogic->pauseGame();
}

void GameScreen::recoverGame()
{
    gameLogic->resumeGame();
}



// 保证随窗口变化而覆盖全屏
void GameScreen::resizeEvent(QResizeEvent* e) {
    QWidget::resizeEvent(e);
    if (gameMenu) gameMenu->setGeometry(rect());
}


//更新menubar的分数和时间显示
void GameScreen::updateScoreDisplay(int s, int index)
{
    if (index == 1) {
        scoreLabel1->setText(QString("P1: %1").arg(s));
    } else if (index == 2) {
        scoreLabel2->setText(QString("P2: %1").arg(s));
    }
}

void GameScreen::updateTimeDisplay(int t)
{
    remainingTime = t ;
    timeLabel->setText(QString("Remaining Time: %1").arg(remainingTime));
}


// 处理按键事件
void GameScreen::keyPressEvent(QKeyEvent* event) {

    if (event->key() == Qt::Key_Escape) {
        pauseGame();
        return;
    }

    QPoint pos1 = gameLogic->getPlayerPosition(1);
    QPoint pos2 = gameLogic->getPlayerPosition(2);



    switch (event->key()) {
    // 玩家1 (WASD)
    case Qt::Key_W: pos1.ry() -= 1; break;
    case Qt::Key_S: pos1.ry() += 1; break;
    case Qt::Key_A: pos1.rx() -= 1; break;
    case Qt::Key_D: pos1.rx() += 1; break;

        // 玩家2 (↑↓←→)
    case Qt::Key_I: pos2.ry() -= 1 ;  break;
    case Qt::Key_K: pos2.ry() += 1; break;
    case Qt::Key_J: pos2.rx() -= 1; break;
    case Qt::Key_L: pos2.rx() += 1; break;
    }

    gameLogic->updatePlayerPosition(1, pos1);

    gameLogic->updatePlayerPosition(2, pos2);

    //触发重新绘制
    update();
}


void GameScreen::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QPixmap background(backgroundUrl[backgroundIndex]);

    // 缩放图片，保持原始纵横比，并保证覆盖整个窗口
    QPixmap scaledBackground = background.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    // 计算偏移量以使图片居中显示
    QPoint centerOffset((scaledBackground.width() - width()) / 2,
                        (scaledBackground.height() - height()) / 2);

    painter.drawPixmap(-centerOffset, scaledBackground);
}

// --- 菜单相关 ---
void GameScreen::pauseGame() {
    //gameMenu->showPauseMenu();
    gameMenu->showPauseMenu();
    gameMenu->raise();
    gameLogic->pauseGame();
}

void GameScreen::resumeGame() {
    gameMenu->hide();
    gameLogic->resumeGame();
}

void GameScreen::restartGame() {
    gameMenu->hide();
    gameLogic->resetGame();
    update();
}

void GameScreen::exitGame() {
    gameLogic->resetGame();
    gameMenu->hide();
    close(); // 直接关闭整个窗口
    emit goBackToStartScreen();
}


void GameScreen::saveGame()
{
    gameLogic->saveGame("save.json");
}


void GameScreen::loadGame()
{
    gameLogic->loadGame("save.json");
}


void GameScreen::onGameOver() {
    int score1 = gameLogic->getScore(1);
    int score2 = gameLogic->getScore(2);

    QString result;
    if (gameLogic->getMode() == 1) {
        result = QString("Final Score: %1").arg(score1);
    } else {
        if (score1 > score2)
            result = QString("P1 Wins! (%1 vs %2)").arg(score1).arg(score2);
        else if (score2 > score1)
            result = QString("P2 Wins! (%1 vs %2)").arg(score2).arg(score1);
        else
            result = QString("Draw! (%1 vs %2)").arg(score1).arg(score2);
    }

    setPause();
    gameMenu->setFinalScoreText(result);
    gameMenu->showEndMenu();
}


void GameScreen::setMode(int a)
{
    gameLogic->setGameMode(a);
}



