#include "GamePlayer.h"
#include <QDebug>
#include <QTimer>

GamePlayer::GamePlayer()
    :currPos(0,0)
    ,prevPos(0,0)
    ,score(0)

{

}




GamePlayer::~GamePlayer()
{
    //qDebug()<<"delete player";
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
    //emit scoreChanged(score);
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

