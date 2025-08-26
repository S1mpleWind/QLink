#include "GameLogic.h"
#include "GamePreset.h"
#include <QTimer>
#include <QDebug>
#include <QRandomGenerator>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>

GameLogic::GameLogic(QWidget* parent)
    :gameMap(new GameMap())
    ,remainingTime(GAME_TIME)
    ,gameTimer(new QTimer(this))
    ,propTimer(new QTimer(this))

{

    //connect(gameMap,&GameMap::,this,&GameLogic::setNormalMode);
    connect(gameTimer,&QTimer::timeout,this,&GameLogic::countTime);
    gameTimer->start(1000);


    connect(propTimer,&QTimer::timeout,this,&GameLogic::generateRandomProp);
    propTimer->start(PROP_GENERATE_TIME);


    connect(gameMap,&GameMap::checkCanLink,this,&GameLogic::canLink);
    //bool drawline = connect(this,&GameLogic::drawLineSignal,gameMap,&GameMap::update);

    //gamePlayer = new GamePlayer();
    initPlayer(1);
    initPlayer(2);

    if (gameMode == 1) {
        QPoint p2 = gamePlayer2->getPosition();
        gameMap->setBoxType(p2.x(), p2.y(), 0);  // 清掉P2位置
    }

    //initialize player position
    updatePlayerPosition(1,gamePlayer->getPosition());

    connect(gamePlayer,&GamePlayer::scoreChanged,this,&GameLogic::receiveScores);

    connect(gameMap,&GameMap::flashPosition,this,&GameLogic::updatePlayerPosition);


    //initial for hint
    updateEdgePts();

    hintTimer = new QTimer(this);
    hintTimer->setSingleShot(true);
    connect(hintTimer, &QTimer::timeout, this, [=]() {
        gameMap->disablePaintHint();
        gameMap->update();
        hintActive = false;
    });
}


void GameLogic :: setGameMode(int a){
    gameMode = a;
    if (a==2)
    {
        gameMap->multiModeOn();
        QPoint p2 = gamePlayer2->getPosition();
        gameMap->setBoxType(p2.x(), p2.y(), -2);
        gameMap->update();
    }

    else gameMap->multimodeOff();
}

GameLogic::~GameLogic()
{
    delete gamePlayer;
    delete gamePlayer2;
}

int GameLogic::getScore(int index) {
    if (index == 1) return gamePlayer->getScore();
    if (index == 2) return gamePlayer2->getScore();
    return 0;
}

void GameLogic::saveGame(const QString &filePath) {
    QJsonObject root;
    root["mode"] = gameMode;  // 1=单人, 2=双人

    // 存时间
    root["remainingTime"] = remainingTime;

    // 1. 存地图
    QJsonArray mapArray;
    int rowCount = gameMap->getRowNum() + 2*gameMap->getBufferNum();
    int colCount = gameMap->getColNum() + 2*gameMap->getBufferNum();

    for (int i = 0; i < rowCount; ++i) {
        QJsonArray row;
        for (int j = 0; j < colCount; ++j) {
            row.append(gameMap->getBoxType(j, i));
        }
        mapArray.append(row);
    }
    root["map"] = mapArray;

    // 2. 存玩家1
    QJsonObject player1Obj;
    player1Obj["x"] = gamePlayer->getPosition().x();
    player1Obj["y"] = gamePlayer->getPosition().y();
    player1Obj["score"] = gamePlayer->getScore();
    root["player1"] = player1Obj;

    // 3. 存玩家2（仅双人模式）
    if (gameMode == 2 && gamePlayer2) {
        QJsonObject player2Obj;
        player2Obj["x"] = gamePlayer2->getPosition().x();
        player2Obj["y"] = gamePlayer2->getPosition().y();
        player2Obj["score"] = gamePlayer2->getScore();
        root["player2"] = player2Obj;
    }

    // 4. 写入文件
    QJsonDocument doc(root);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void GameLogic::loadGame(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();

    gameMode = root["mode"].toInt(1); // 默认单人

    remainingTime = root["remainingTime"].toInt(60); // 默认60秒
    emit updateTime(remainingTime);

    // 1.recover map
    QJsonArray mapArray = root["map"].toArray();
    for (int i = 0; i < mapArray.size(); ++i) {
        QJsonArray row = mapArray[i].toArray();
        for (int j = 0; j < row.size(); ++j) {
            gameMap->setBoxType(j, i, row[j].toInt());
        }
    }

    // 2.recover player
    QJsonObject player1Obj = root["player1"].toObject();
    QPoint pos1(player1Obj["x"].toInt(), player1Obj["y"].toInt());
    gamePlayer->setPosition(pos1);
    gamePlayer->setScore(player1Obj["score"].toInt());
    emit updateScores(gamePlayer->getScore(),1);

    if (gameMode == 2 && root.contains("player2")) {
        QJsonObject player2Obj = root["player2"].toObject();
        QPoint pos2(player2Obj["x"].toInt(), player2Obj["y"].toInt());
        gamePlayer2->setPosition(pos2);
        gamePlayer2->setScore(player2Obj["score"].toInt());
        emit updateScores(gamePlayer2->getScore(),2);
    }

    //repaint
    gameMap->update();
}



void GameLogic::generateRandomProp()
{
    if(isPaused) return ;

    //TODO
    if(gameMode == 2) return;

    QPoint pos = getRandomBufferCell();

    int t = QRandomGenerator::global()->bounded(-8, -4);

    gameMap->setBoxType(pos.x(),pos.y(),t);
    gameMap->update();
}


void GameLogic::initPlayer(int index)
{
    if(index==1)
    {
        gamePlayer = new GamePlayer();
        QPoint initPos = getRandomBufferCell();
        gamePlayer->setPosition(initPos);
        int x = initPos.x(), y = initPos.y();
        gameMap->setBoxType(x,y,-1);
    }

    else if (index == 2)
    {
        gamePlayer2 = new GamePlayer();
        QPoint initPos = getRandomBufferCell();
        gamePlayer2->setPosition(initPos);
        int x = initPos.x(), y = initPos.y();
        gameMap->setBoxType(x,y,-2);
    }
}

GamePlayer* GameLogic::getPlayer(int index)
{
    return index==0?gamePlayer:gamePlayer2;
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

    emit updateTime(remainingTime);

    if(remainingTime == 0)
    {
        emit timeIsUp();
    }
}


void GameLogic:: receiveScores(int s)
{

    emit updateScores(s,1);
}

bool GameLogic :: delayClearSelect(int index)
{
    QTimer::singleShot(500, gameMap, [index,this]() {
        gameMap->clearSelected(index);  // 500ms 后清空
    });
    return false;
}



bool GameLogic::canLink(QPoint pt1, QPoint pt2 , int index) {
    if (!gameMap) {
        qDebug() << "GameMap not created successfully!";
        return false;
    }

    //Basic conditions
    if (pt1 == pt2)
    {
        return delayClearSelect(index);
    }

    int type1 = gameMap->getBoxType(pt1.x(), pt1.y());
    int type2 = gameMap->getBoxType(pt2.x(), pt2.y());
    if (type1 == 0 || type2 == 0 || type1 != type2)
    {
        return delayClearSelect(index);
    }


    validPath.clear();

    if (canConnectStraight(pt1, pt2) ||
        canConnectOneTurn(pt1, pt2) ||
        canConnectTwoTurn(pt1, pt2)) {

        //In detectmode , these functions will not be triggered to paint path
        if(!detectMode)
        {
            gameMap->enablePaintPath();
            gameMap->setLinkPath(validPath);

            if(index == 1)
            {
                gamePlayer->addFixedScore();
                emit updateScores(gamePlayer->getScore(),1);
                }
            else {
                gamePlayer2->addFixedScore();
                emit updateScores(gamePlayer2->getScore(),2);
                }

            //draw map
            gameMap->update();


            QTimer::singleShot(500, this, [=]() {
                remove(pt1, pt2);
                gameMap->update();
            });

            QTimer::singleShot(500, gameMap, [this,index]() {
                gameMap->clearSelected(index);  // 500ms 后清空
            });

            QTimer::singleShot(800 ,this,[=](){
                gameMap->disablePaintPath();
                validPath.clear();
                gameMap->update();
            });
        }
        return true;
    }

    return delayClearSelect(index);
}



// Basic conditions
bool GameLogic::isEmptyTile(QPoint pt) {
    int x = pt.x() , y=pt.y();
    return gameMap->getBoxType(x,y) <= 0;
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

    if (!hintPts.empty() &&
        ((pt_1 == hintPts[0] || pt_2 == hintPts[1]) ||
         (pt_1 == hintPts[1] || pt_2 == hintPts[0]))) {
        hintPts.clear();
        hintPath.clear();
        gameMap->disablePaintHint();
        if (hintActive) showHint();
    }

    updateEdgePts();
    remainUnmatchedPairs();

}






//control player
QPoint GameLogic::getPlayerPosition(int i )
{
    return i == 1 ? gamePlayer->getPosition() : gamePlayer2->getPosition();
}



void GameLogic::updatePlayerPosition(int index, QPoint newPos)
{
    if (isPaused) return;

    int col = newPos.x();
    int row = newPos.y();

    // if it hit boundary , stay still
    if (row < 0 || row >= gameMap->getRowNum() + 2 * gameMap->getBufferNum() ||
        col < 0 || col >= gameMap->getColNum() + 2 * gameMap->getBufferNum()) {
        qDebug() << "Invalid move: out of bounds";
        return;
    }

    int t = gameMap->getBoxType(col, row);
\
    if (t > 0) { // hit pair box
        if(index==1)
        {
            gameMap->addSelected(newPos);
            return;
        }
        else
        {
            gameMap->addSelected2(newPos);
            return;
        }
    }

    // prop logic (only active in single mode)
    if (t < -2 && gameMode == 1) {
        switch (t) {
        case PROP_ADD_ONE:
            remainingTime += ADD_TIME;
            emit updateTime(remainingTime);
            break;
        case PROP_FLASH:
            gameMap->flashModeON();
            QTimer::singleShot(FLASH_TIME, gameMap, &GameMap::flashModeOff);
            break;
        case PROP_HINT:
            hintTimer->start(HINT_TIME);
            hintActive = true ;
            showHint();
            break;
        case PROP_SHUFFLE:
            gameMap->shuffleMap();
            updateEdgePts();
            showHint();
            break;
        default:
            qDebug() << "error when hitting props";
            break;
        }
    }

    // === 区分不同玩家 ===
    if (index == 1) {
        QPoint prevPos = gamePlayer->getPosition();  // 玩家1旧位置
        gamePlayer->setPosition(newPos);

        gameMap->setBoxType(prevPos.x(), prevPos.y(), 0);   // 清空旧位置
        gameMap->setBoxType(newPos.x(), newPos.y(), -1);    // 玩家1标记
    }

    else if (index == 2 && gameMode == 2) {
        QPoint prevPos = gamePlayer2->getPosition(); // 玩家2旧位置
        gamePlayer2->setPosition(newPos);

        gameMap->setBoxType(prevPos.x(), prevPos.y(), 0);   // 清空旧位置
        gameMap->setBoxType(newPos.x(), newPos.y(), -2);    // 玩家2标记
    }

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
                    edgePtsByType[type].push_back(pt);
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
        if (pts.size() < 2) continue; // no enough boxs left , skip
        // not trigger the remove function
        for (int i = 0; i < pts.size(); ++i) {
            for (int j = i + 1; j < pts.size(); ++j) {
                // not trigger the remove function
                if (canLink(pts[i], pts[j],1)) {
                    //In Detect mode , we record the possible path for the hint mode to call
                    hintPath.clear();
                    hintPath = validPath;

                    hintPts.clear();
                    hintPts.push_back(pts[i]);
                    hintPts.push_back(pts[j]);

                    detectMode = false ;
                    return; // 找到可消除的一对
                }
            }
        }
    }

    qDebug()<<"no more valid pairs";
    emit noMoreValidPairs();
    //throw it to game screen
}


void GameLogic::showHint() {
    remainUnmatchedPairs();
    if (hintPts.empty()) {
        qDebug() << "No hint available";
        return;
    }

    gameMap->enablePaintHint();
    gameMap->setHintPath(hintPath);
    gameMap->update();

    hintActive = true;
}




void GameLogic::resetGamePara()
{
    //TODO:
    gameMap->resetMap();
    remainingTime = GAME_TIME ;
    //gamePlayer->resetPlayer();
    //return;
    gameMap->clearSelected(1);
    gameMap->clearSelected(2);

    gamePlayer->clearScore();
    emit updateScores(gamePlayer->getScore(), 1);
    gamePlayer2->clearScore();
    emit updateScores(gamePlayer2->getScore(), 2);
}

void GameLogic::resetGamePlayer()
{
    gamePlayer->resetPlayer();
    gamePlayer2->resetPlayer();
}

void GameLogic::resetGame()
{
    resetGamePara();
    resetGamePlayer();
    initPlayer(1);
    initPlayer(2);
    resumeGame();
}
