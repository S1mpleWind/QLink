#include "GameLogic.h"
#include <QTimer>
#include <QDebug>
GameLogic::GameLogic(GameMap * map)
    :gameMap(map)
    ,remainingTime(gameTime)
    ,gameTimer(new QTimer(this))

{
    //connect(gameMap,&GameMap::checkCanLink,this,&GameLogic::setNormalMode);
    connect(gameTimer,&QTimer::timeout,this,&GameLogic::countTime);
    gameTimer->start(1000);

    bool checklink = connect(gameMap,&GameMap::checkCanLink,this,&GameLogic::canLink);
    //bool drawline = connect(this,&GameLogic::drawLineSignal,gameMap,&GameMap::update);
    qDebug() << "[DEBUG] connect result:" << checklink;
    gamePlayer = new GamePlayer();

    //initialize player position
    updatePlayerPosition(gamePlayer->getPosition());

    //bool updatePlayer = connect(gamePlayer,&GamePlayer::positionChanged,gameMap,&GameMap::updatePlayerPosition);

    connect(gamePlayer,&GamePlayer::scoreChanged,this,&GameLogic::receiveScores);


}


GameLogic::~GameLogic()
{
    delete gamePlayer;
}


void GameLogic::initPlayer()
{

}

GamePlayer* GameLogic::getPlayer()
{
    return gamePlayer;
}

QPoint GameLogic::getRandomBufferCell()
{
    std::vector<QPoint> candidates;

    int rows = gameMap->getRowNum();
    int cols = gameMap->getColNum();
    int buffer = gameMap->getBufferNum();

    // 遍历 buffer 区（四周一圈）
    for (int x = 0; x < cols + 2*buffer ; ++x) {
        for (int y = 0; y < rows + 2*buffer ; ++y) {
            if (x < buffer || x >= cols + buffer ||
                y < buffer || y >= rows + buffer) {
                if (gameMap->getBoxType(x, y) == 0) {
                    candidates.push_back(QPoint(x, y));
                }
            }
        }
    }

    if (candidates.empty()) {
        qDebug()<<"error when finding suitable position for player";
        return QPoint(-1, -1); // 没有可用位置
    }

    int idx = std::rand() % candidates.size();
    return candidates[idx];
}


void GameLogic::countTime()
{
    if(isPaused)return;

    remainingTime-=1;
    //qDebug()<<"remaining time"<<remainingTime;
    emit updateTime(remainingTime);

    if(remainingTime == 0)
    {
        emit timeIsUp();
    }
}


void GameLogic:: receiveScores(int s)
{
    emit updateScores(s);
}

bool GameLogic :: delayClearSelect()
{
    QTimer::singleShot(500, gameMap, [this]() {
        gameMap->clearSelected();  // 500ms 后清空
    });
    return false;
}


//Most Important Function here

bool GameLogic::canLink(QPoint pt1, QPoint pt2) {
    if (!gameMap) {
        qDebug() << "GameMap not created successfully!";
        return false;
    }

    //Basic conditions
    if (pt1 == pt2)
    {
        // QTimer::singleShot(500, gameMap, [this]() {
        //     gameMap->clearSelected();  // 500ms 后清空
        // });
        // return false;
        return delayClearSelect();
    }

    int type1 = gameMap->getBoxType(pt1.x(), pt1.y());
    int type2 = gameMap->getBoxType(pt2.x(), pt2.y());
    if (type1 == 0 || type2 == 0 || type1 != type2)
    {
        return delayClearSelect();
    }



    // 清空路径，避免旧路径残留
    validPath.clear();

    if (canConnectStraight(pt1, pt2) ||
        canConnectOneTurn(pt1, pt2) ||
        canConnectTwoTurn(pt1, pt2)) {

        //In detectmode , these functions will not be triggered
        if(!detectMode)
        {
            gameMap->enablePaintPath();
            gameMap->setLinkPath(validPath);


            gamePlayer->addFixedScore();

            //draw map
            gameMap->update();


            QTimer::singleShot(500, this, [=]() {
                remove(pt1, pt2);
                gameMap->update();
            });

            QTimer::singleShot(500, gameMap, [this]() {
                gameMap->clearSelected();  // 500ms 后清空
            });

            QTimer::singleShot(800 ,this,[=](){
                gameMap->disablePaintPath();
                validPath.clear();
                gameMap->update();
            });
        }

        return true;
    }

    return delayClearSelect();
}








// Basic conditionsint
bool GameLogic::isEmptyTile(QPoint pt) {
    int x = pt.x() , y=pt.y();
    return gameMap->getBoxType(x,y) <= 0;

    //ATTENTION : the 2-D array has a different direction compared with the coordinates
}

bool GameLogic::isClearRow(int row, int col1, int col2) {
    // these are  y , x1 , x2
    if (col1 > col2) std::swap(col1, col2);
    for (int c = col1 + 1; c < col2; ++c) {
        if (gameMap->getBoxType(c,row) > 0) return false;
    }
    return true;
}

bool GameLogic::isClearCol(int col, int row1, int row2) {
    //these are x , y1 , y2
    if (row1 > row2) std::swap(row1, row2);
    for (int c = row1 + 1; c < row2; ++c) {
        if (gameMap->getBoxType(col,c) > 0) return false;
    }
    return true;
}


bool GameLogic::canConnectStraight(QPoint p1, QPoint p2) {
    if (p1.x() == p2.x() && isClearCol(p1.x(), p1.y(), p2.y())) {
        validPath.clear();
        validPath << p1 << p2;
        return true;
    }
    if (p1.y() == p2.y() && isClearRow(p1.y(), p1.x(), p2.x())) {
        validPath.clear();
        validPath << p1 << p2;
        return true;
    }
    return false;
}

bool GameLogic::canConnectOneTurn(QPoint p1, QPoint p2) {
    QPoint corner1(p1.x(), p2.y());
    QPoint corner2(p2.x(), p1.y());

    if (isEmptyTile(corner1) &&
        canConnectStraight(p1, corner1) &&
        canConnectStraight(corner1, p2)) {
        validPath.clear();
        validPath << p1 << corner1 << p2;
        return true;
    }

    if (isEmptyTile(corner2) &&
        canConnectStraight(p1, corner2) &&
        canConnectStraight(corner2, p2)) {
        validPath.clear();
        validPath << p1 << corner2 << p2;
        return true;
    }

    return false;
}

bool GameLogic::canConnectTwoTurn(QPoint p1, QPoint p2) {
    int w = gameMap->getColNum();
    int h = gameMap->getRowNum();
    int b = gameMap->getBufferNum();

    for (int x = b-1; x < w + b + 1; ++x) {
        QPoint pt1(x, p1.y());
        QPoint pt2(x, p2.y());
        if (isEmptyTile(pt1) && isEmptyTile(pt2) &&
            canConnectStraight(p1, pt1) &&
            canConnectStraight(pt1, pt2) &&
            canConnectStraight(pt2, p2)) {
            validPath.clear();
            validPath << p1 << pt1 << pt2 << p2;
            return true;
        }
    }

    for (int y = b-1; y < h + b + 1; ++y) {
        QPoint pt1(p1.x(), y);
        QPoint pt2(p2.x(), y);
        if (isEmptyTile(pt1) && isEmptyTile(pt2) &&
            canConnectStraight(p1, pt1) &&
            canConnectStraight(pt1, pt2) &&
            canConnectStraight(pt2, p2)) {
            validPath.clear();
            validPath << p1 << pt1 << pt2 << p2;
            return true;
        }
    }

    return false;
}



void GameLogic::remove(QPoint pt_1 , QPoint pt_2)
{
    int x1 = pt_1.x(), y1 = pt_1.y();
    int x2 = pt_2.x(), y2 = pt_2.y();

    gameMap->setBoxType(x1,y1,0);
    gameMap->setBoxType(x2,y2,0);

    gameMap->update();
    //repaint the screen

    updateEdgePts();
    remainUnmatchedPairs();

}






//control player
QPoint GameLogic::getPlayerPosition()
{
    return gamePlayer->getPosition();
}




void GameLogic::updatePlayerPosition(QPoint newPos)
{
    if(isPaused)return;

    int col = newPos.x();
    int row = newPos.y();

    QPoint prevPos = gamePlayer ->getPosition();

    // 判断是否越界
    if (row < 0 || row >= gameMap->getRowNum()+2*gameMap->getBufferNum() ||
        col < 0 || col >= gameMap->getColNum()+2*gameMap->getBufferNum()) {
        qDebug() << "Invalid move: out of bounds";
        return;

        //TODO: wraparound function here
    }

    // 判断是否撞到方块
    if (gameMap->getBoxType(col, row) > 0) {
        //qDebug() << "Invalid move: hit a block";
        gameMap->addSelected(newPos);
        return;
    }

    // 更新玩家数据
    gamePlayer->setPosition(newPos);

    // 通知地图更新
    gameMap->setBoxType(prevPos.x(), prevPos.y(), 0);   // 清空旧位置
    gameMap->setBoxType(newPos.x(), newPos.y(), -1);  // 设置新位置
    gameMap->update(); // 触发重绘
}


void GameLogic::updateEdgePts() {
    edgePts.clear();
    edgePtsByType.clear();  // 同步清空哈希表

    int rows = gameMap->getRowNum();
    int cols = gameMap->getColNum();
    int buf  = gameMap->getBufferNum();

    for (int i = buf; i < rows + buf; ++i) {
        for (int j = buf; j < cols + buf; ++j) {
            int type = gameMap->getBoxType(j, i);
            if (type > 0) {  // 有方块
                // 判断是否有空格可接触
                if (gameMap->getBoxType(j+1, i) == 0 ||
                    gameMap->getBoxType(j-1, i) == 0 ||
                    gameMap->getBoxType(j, i+1) == 0 ||
                    gameMap->getBoxType(j, i-1) == 0) {
                    QPoint pt(j, i);
                    edgePts.push_back(pt);
                    edgePtsByType[type].push_back(pt);  // 同时分类
                }
            }
        }
    }
}



void GameLogic:: remainUnmatchedPairs()
{
    detectMode = true;
    gameMap->disablePaintPath();

    for (auto &[type, pts] : edgePtsByType) {
        if (pts.size() < 2) continue; // 不够配对，跳过



        // not trigger the remove function

        for (int i = 0; i < pts.size(); ++i) {
            for (int j = i + 1; j < pts.size(); ++j) {
                //detectMode = true;
                //gameMap->enablePaintPath();
                // not trigger the remove function
                if (canLink(pts[i], pts[j])) {

                    //this step will couse extra "clr pts"


                    detectMode = false ;
                    qDebug()<<"remain unmatched pairs";
                    //gameMap->enablePaintPath();
                return; // 找到可消除的一对
                }
            }
        }
    }

    qDebug()<<"no more valid pairs";
    emit noMoreValidPairs();
    //throw it to game screen
}




void GameLogic::setNormalMode()
{
    gameMap->enablePaintPath();
}
// boll GameLogic :: isBool （）


void GameLogic::resetGamePara()
{
    //TODO:
    gameMap->resetMap();
    remainingTime = gameTime ;
    //gamePlayer->resetPlayer();
    //return;
}

void GameLogic::resetGamePlayer()
{
    gamePlayer->resetPlayer();
    updatePlayerPosition(QPoint(0,0));
    //TODO: logic here
}

void GameLogic::resetGame()
{
    resetGamePara();
    resetGamePlayer();
    resumeGame();
}
