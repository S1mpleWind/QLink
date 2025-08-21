#include <QVBoxLayout>
#include "GameScreen.h"
#include <QDebug>

GameScreen::GameScreen(QWidget* parent)
    : QWidget(parent)
    , gameMap(new GameMap(this)) // ← 这里改成 new 出来
    , gameLogic(new GameLogic(gameMap))
    , gameMenu(new GameMenu(this))
{

    srand((int)time(0));
    backgroundIndex = std::rand()%6;


    //preset
    gameLogic->pauseGame();



    //UI
    scoreLabel = new QLabel(this);
    scoreLabel->setText(QString("Score: %1").arg(displayScore));
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("font: bold 16px; color: white;"); // 可根据需要美化
    scoreLabel->setFixedHeight(40);

    timeLabel = new QLabel(this);
    timeLabel->setText(QString("Remaining time : %1").arg(remainingTime));
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("font: bold 16px; color: white;"); // 可根据需要美化
    timeLabel->setFixedHeight(40);

    QHBoxLayout* menuBar = new QHBoxLayout();
    menuBar->addWidget(scoreLabel);
    menuBar->addWidget(timeLabel);

    // 创建一个布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 可选：无边距




    mainLayout->addLayout(menuBar,0);  // 分数在顶部
    mainLayout->addWidget(gameMap,1);            // 把地图控件加入布局

    //gameLogic->updatePlayerPosition();

    setLayout(mainLayout); // 设置布局到 GameScreen 上


    // 初始隐藏菜单
    gameMenu->hide();
    gameMenu->raise();  // 保证覆盖在最上层
    gameMenu->resize(size());

    connect(gameMenu, &GameMenu::resumeClicked, this, &GameScreen::resumeGame);
    connect(gameMenu, &GameMenu::restartClicked, this, &GameScreen::restartGame);
    connect(gameMenu, &GameMenu::exitClicked, this, &GameScreen::exitGame);
    connect(gameMenu, &GameMenu::fileClicked,this,&GameScreen::saveGame);




    connect(gameLogic, &GameLogic::noMoreValidPairs,this,&GameScreen::pauseGame);

    connect(gameLogic,&GameLogic::updateScores,this,&GameScreen::updateScoreDisplay);
    connect(gameLogic,&GameLogic::updateTime,this,&GameScreen::updateTimeDisplay);


    connect(gameLogic,&GameLogic::timeIsUp,this,&GameScreen::onGameOver);
    connect(gameLogic,&GameLogic::noMoreValidPairs,this,&GameScreen::onGameOver);




}

GameScreen:: ~GameScreen()
{

}


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


void GameScreen::updateScoreDisplay(int s)
{
    displayScore = s ;
    scoreLabel->setText(QString("Score: %1").arg(displayScore));
}

void GameScreen::updateTimeDisplay(int t)
{
    remainingTime = t ;
    timeLabel->setText(QString("Remaining Time: %1").arg(remainingTime));
}


void GameScreen::keyPressEvent(QKeyEvent* event) {

    if (event->key() == Qt::Key_Escape) {
        pauseGame();
        return;
    }

    QPoint newPos = gameLogic->getPlayerPosition();
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        //qDebug()<<"moveup";
        newPos.ry() -= 1;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        //qDebug()<<"movedown";
        newPos.ry() += 1;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        //qDebug()<<"moveleft";
        newPos.rx() -= 1;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        //qDebug()<<"moveright";
        newPos.rx() += 1;
        break;
    }
    gameLogic->updatePlayerPosition(newPos);
    update(); // 触发重绘
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
    gameLogic->resetGame();   // 假设 GameLogic 有个 resetGame()
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
    // 从 player 拿到分数
    int finalScore = gameLogic->getPlayer()->getScore();

    setPause();

    // 传给 GameMenu
    gameMenu->setFinalScore(finalScore);

    // 显示结束菜单
    gameMenu->showEndMenu();
}
