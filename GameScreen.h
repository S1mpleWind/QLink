#pragma once
#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QKeyEvent>
#include "GameMap.h"
#include "GameLogic.h"
#include "GameMenu.h"
class GameScreen : public QWidget
{
    Q_OBJECT

public:
    GameScreen(QWidget* parent = nullptr);
    ~GameScreen();

    void setPause();
    void recoverGame();


protected:
    void paintEvent(QPaintEvent* event) override ;
    void keyPressEvent(QKeyEvent* event) override ;
    void resizeEvent(QResizeEvent* event) override;
private slots:
    void pauseGame();
    void resumeGame();
    void restartGame();
    void exitGame();
    void onGameOver();
    void saveGame();

public slots:
    void loadGame();
    void setMode(int);

    //void updateScoreDisplay(int s , int index);



private:
    //GameMap* gameMap;
    GameLogic* gameLogic;
    GameMenu* gameMenu;

    QLabel* scoreLabel1;
    QLabel* scoreLabel2;
    int displayScore = 0 ;

    QLabel* timeLabel;
    int remainingTime = 120 ;



    int backgroundIndex ;

    QVector<QString> backgroundUrl = {
        ":/img/images/background_img/background_1.png",
        ":/img/images/background_img/background_2.png",
        ":/img/images/background_img/background_3.png",
        ":/img/images/background_img/background_4.png",
        ":/img/images/background_img/background_5.png",
        ":/img/images/background_img/background_6.png"
};
private slots:
    void updateScoreDisplay(int,int);
    void updateTimeDisplay(int);

signals:
    void goBackToStartScreen();

};
