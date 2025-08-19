#pragma once
#include <QWidget>


class GamePlayer:public QWidget{

    Q_OBJECT

public:
    GamePlayer();
    ~GamePlayer();


    void initPlayer();

    /*
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
*/
    void addFixedScore();
    void addScore(int);
    void clearScore();
    int getScore(){return score;};

    void resetPlayer(){setPosition(QPoint(0,0));}


    QPoint getPosition() const;


    //try not to call this function or use it as initialization
    //because the prevpos cannot be transported
    void setPosition(const QPoint& pos);


signals:
    void positionChanged(QPoint pos);
    void scoreChanged(int s);


private:
    QPoint currPos;
    QPoint prevPos;

    int score;


};
