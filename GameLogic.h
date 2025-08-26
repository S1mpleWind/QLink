#pragma once
#include "GameMap.h"
#include "GamePlayer.h"
#include <Qwidget>
#include <QTimer>
#include <QVector>
#include <QPainter>



class GameLogic : public QWidget
{
    Q_OBJECT

public:
    GameLogic(QWidget * parent = nullptr);
    ~GameLogic();

//获取player信息的接口
    QPoint getPlayerPosition(int);
    GamePlayer* getPlayer(int);

    GameMap * getMap() {
        return gameMap;
    }
    //TODO : use index in multiplayer mode
    /*
    void movePlayerUp();
    void movePlayerDown();
    void movePlayerLeft();
*/
    void movePlayerRight();

    void updatePlayerPosition(int , QPoint);
    void updateEdgePts();

    void remainUnmatchedPairs();

    void resetGame();
    void resetGamePara();
    void resetGamePlayer();

    void pauseGame(){isPaused=true;};
    void resumeGame(){isPaused=false;};

    void setGameMode(int a);

    int getScore(int);

    int getMode(){return gameMode;}

    bool canLink(QPoint pt_1,QPoint pt_2 ,int index);


protected:    //judge a legal link


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

    bool delayClearSelect(int);






private:
    GameMap* gameMap;

    GamePlayer * gamePlayer;

    GamePlayer * gamePlayer2;

    void initPlayer(int);
    QPoint getRandomBufferCell();

    QVector<QPoint> validPath;

    QVector<QPoint> edgePts;
    std::unordered_map<int, std::vector<QPoint>> edgePtsByType;
    bool detectMode = false ;

    QTimer* gameTimer;

    int remainingTime;

    QTimer* propTimer;

    QTimer* hintTimer;
    bool hintActive = false ;


    //control logic
    bool isPaused;

    bool hintMode = false ;
    QVector<QPoint> hintPath;

    QVector<QPoint> hintPts;
    void showHint();

    int gameMode = 1 ;



public slots:
    void receiveScores(int);
    void countTime();

    void generateRandomProp();   // 随机生成道具
    void saveGame(const QString & filePath);
    void loadGame(const QString & filePath);

signals:
    void drawLineSignal();
    void deletePtSignal(QPoint,QPoint);
    void noMoreValidPairs();

    void updateScores(int,int);

    void updateTime(int);
    void timeIsUp();


};
