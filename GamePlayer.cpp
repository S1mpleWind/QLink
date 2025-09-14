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
}


//获取位置的接口
QPoint GamePlayer::getPosition()const
{
    return currPos;
}


//设置位置的接口
void GamePlayer::setPosition(const QPoint& pos) {
    currPos = pos;
}


//两种加分形式的接口
void GamePlayer::addFixedScore()
{
    score+=10;
}

void GamePlayer::addScore(int s)
{
    score+=s;
}


//重置分数
void GamePlayer::clearScore()
{
    score = 0 ;
}

