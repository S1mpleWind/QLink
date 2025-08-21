#include "GamePlayer.h"
#include <QDebug>
#include <QTimer>

GamePlayer::GamePlayer()
    :currPos(0,0)
    ,prevPos(0,0)
    ,score(0)

{
    qDebug()<<"gameplayer created:"<<currPos;
    //emit positionChanged(position);
    //emit 在完全构造好之前无法调用

    QTimer::singleShot(0, this, [this]{
        emit positionChanged(currPos);
    });
}




GamePlayer::~GamePlayer()
{
    qDebug()<<"delete player";
}

QPoint GamePlayer::getPosition()const
{
    return currPos;
}


void GamePlayer::setPosition(const QPoint& pos) {
    currPos = pos;
}

void GamePlayer::addFixedScore()
{
    score+=10;
    emit scoreChanged(score);
}

void GamePlayer::addScore(int s)
{
    score+=s;
    emit scoreChanged(score);
}

void GamePlayer::clearScore()
{
    score = 0 ;
}



/*
void GamePlayer::moveUp() {
    qDebug()<<"player-up called";
    prevPos=currPos;
    currPos.setY(currPos.y() - 1);
    qDebug()<<"position changed:"<<currPos;
    emit positionChanged(currPos);

}

void GamePlayer::moveDown() {
    prevPos=currPos;
    currPos.setY(currPos.y() + 1);
    qDebug()<<"position changed:"<<currPos;
    emit positionChanged(currPos);
}

void GamePlayer::moveLeft() {
    prevPos=currPos;
    currPos.setX(currPos.x() - 1);
    qDebug()<<"position changed:"<<currPos;
    emit positionChanged(currPos);
}

void GamePlayer::moveRight() {
    prevPos=currPos;
    currPos.setX(currPos.x() + 1);
    qDebug()<<"position changed:"<<currPos;
    emit positionChanged(currPos);
}
*/
