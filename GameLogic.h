#pragma once
#include "GameMap.h"
#include "GamePlayer.h"
#include <Qwidget>
#include <QTimer>
#include <QVector>
#include <QPainter>

#define gameTime 10

class GameLogic : public QWidget
{
    Q_OBJECT

public:
    GameLogic(GameMap* map = nullptr);
    ~GameLogic();

//获取player信息的接口
    QPoint getPlayerPosition();
    GamePlayer* getPlayer();
    //TODO : use index in multiplayer mode
    /*
    void movePlayerUp();
    void movePlayerDown();
    void movePlayerLeft();
*/
    void movePlayerRight();

    void updatePlayerPosition(QPoint);
    void updateEdgePts();

    void remainUnmatchedPairs();

    void resetGame();
    void resetGamePara();
    void resetGamePlayer();

    void pauseGame(){isPaused=true;};
    void resumeGame(){isPaused=false;};


protected:    //judge a legal link
    bool canLink(QPoint pt_1,QPoint pt_2);

    bool isEmptyTile(QPoint);
    bool isClearRow(int row , int col_1 , int col_2);
    bool isClearCol(int col , int row_1 , int row_2);

    //“——"shape
    bool canConnectStraight(QPoint pt_1, QPoint pt_2);
    //"L"shape
    bool canConnectOneTurn(QPoint pt_1, QPoint pt_2);
    //"|__|"shape
    bool canConnectTwoTurn(QPoint pt_1, QPoint pt_2);


    void remove(QPoint pt_1,QPoint pt_2);

    //void drawLinkPath(QPainter&,QPoint,QPoint);

    bool delayClearSelect();






private:
    GameMap* gameMap;

    GamePlayer * gamePlayer;
    void initPlayer();
    QPoint getRandomBufferCell();

    QVector<QPoint> validPath;

    QVector<QPoint> edgePts;
    std::unordered_map<int, std::vector<QPoint>> edgePtsByType;
    bool detectMode = false ;

    QTimer* gameTimer;
    int remainingTime;


    //control logic
    bool isPaused;



public slots:
    void setNormalMode();
    void receiveScores(int);
    void countTime();

signals:
    void drawLineSignal();
    void deletePtSignal(QPoint,QPoint);
    void noMoreValidPairs();
    void updateScores(int);
    void updateTime(int);
    void timeIsUp();


};
