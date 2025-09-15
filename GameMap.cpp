#include "GameMap.h"
#include "GamePreset.h"
#include <qpainter.h>
#include <QMouseEvent>
#include <QDebug>
#include <QRandomGenerator>
#include <algorithm>   // for std::shuffle

/*
    GameMap类主要负责各种的绘制事件
    这里的函数也主要是绘制相关的函数
*/


GameMap::GameMap (QWidget* parent)
{
    srand((int)time(0));

    setBufferNum(BUFFER_NUM);
    setColNum(COL_NUM);
    setRowNum(ROW_NUM);

    initMap();
}

GameMap:: ~GameMap()
{

}


void GameMap::initMap()
{

    // 初始化二维数组大小
    mapType.resize(totalRows);  // 先设置行数
    for (int i = 0; i < totalRows; ++i) {
        mapType[i].resize(totalCols);  // 每一行设置列数
    }

    // 用循环初始化数组
    for (int i = 0; i <= row + 2 * buffer - 1 ; ++i)
        for (int j = 0; j <= column + 2 * buffer - 1; ++j) {
            mapType[i][j] = 0;
        }

    // 用随机数填入地图中的方块区域
    for (int i = buffer; i <= row + buffer-1 ; ++i)
        for (int j = buffer ; j <= column + buffer - 1; ++j) {
            mapType[i][j] = ((i+j+std::rand())) % boxType + 1;
        }
}


void GameMap::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);

    //设置视口比例，防止地图变形
    int heightSide, widthSide;
    if (((double)(width())/(double)(height())) > ((double)(totalCols) / (double)(totalRows))) {
        heightSide = height();
        widthSide = totalCols * heightSide / totalRows;
    } else {
        widthSide = width();
        heightSide = totalRows * widthSide / totalCols;
    }
    painter.setViewport((width()-widthSide)/2,(height()-heightSide)/2,widthSide,heightSide);


    //设置painter的坐标，方便画图
    double widSpace, heiSpace;
    heiSpace = row / std::min(row, column);
    widSpace = column / std::min(column, row);
    painter.setWindow(-widSpace, -heiSpace, totalCols+2*widSpace, totalRows+2*heiSpace);


    //绘制地图
    drawMap(&painter);

    //绘制选中边框特效
    highlightSelectedPt(&painter);

    //绘制连接的路径
    drawLinkPath(&painter);

    //绘制提示路径
    drawHintPath(&painter);

}


void GameMap::drawLinkPath(QPainter* painter) const
{

    if (!linkPath.isEmpty()&&paintPath) {
        //qDebug()<<"painting path";
        painter->setPen(QPen(Qt::red, 0.1));
        auto center = [](QPoint pt) -> QPointF {
            return QPointF(pt.x() + 0.5, pt.y() + 0.5);
        };
        for (int i = 0; i + 1 < linkPath.size(); ++i) {
            painter->drawLine(center(linkPath[i]), center(linkPath[i + 1]));
        }
        //disablePaintPath();
    }
}


void GameMap::drawHintPath(QPainter* painter) const
{
    if (!hintPath.isEmpty()&&paintHintPath) {
        //qDebug()<<"painting hint path";
        painter->setPen(QPen(Qt::red, 0.1));
        auto center = [](QPoint pt) -> QPointF {
            return QPointF(pt.x() + 0.5, pt.y() + 0.5);
        };
        for (int i = 0; i + 1 < hintPath.size(); ++i) {
            painter->drawLine(center(hintPath[i]), center(hintPath[i + 1]));
        }
        //disablePaintPath();
    }
}

void GameMap::drawMap(QPainter *painter) const
{
    painter->setPen(Qt::NoPen);

    //挨个格子绘制地图
    for (int i = 0; i <= row + 2 * buffer - 1 ; ++i)
        for (int j = 0; j <= column + 2*buffer - 1; ++j) {

            //“0”代表空格
            if (mapType[i][j] == 0)
            {
                drawBufferBox(painter , j , i);
            }

            //“-2”是玩家2
            else if (mapType[i][j] == -2)
            {
                //处在多人模式中触发绘制，否则当作空格处理
                if (multimode)
                {
                    drawPlayer2(painter,j,i);
                }

                else {
                    drawBufferBox(painter , j , i);
                }
            }


            //“-1”代表玩家1
            else if (mapType[i][j] == -1)
            {
                drawPlayer(painter,j,i);
            }


            //大于0的都是要消除的格子
            else if(mapType[i][j]>0){
                drawPairBox(painter,j,i);
            }


            //其他的负数都是道具格子
            else
            {
                drawProp(painter,j,i,mapType[i][j]);
            }
        }
}


//绘制高亮格子的注释
void GameMap::highlightSelectedPt(QPainter* painter) const
{
    //没有选中的格子，退出
    if (selectedPts.empty()&&selectedPts2.empty())
    {
        return;
    }

    //设置笔刷格式
    painter->setPen(QPen(Qt::yellow, 0.1));
    painter->setBrush(Qt::NoBrush);

    for (const QPoint& pt : selectedPts) {
        painter->drawRect(QRectF(pt.x(), pt.y(), 1, 1));  // 给选中方块画黄色边框
    }


    //多人模式下，使用另一种颜色绘制选中的方块，避免混淆
    if(multimode)
    {
        painter->setPen(QPen(Qt::green, 0.1));
        painter->setBrush(Qt::NoBrush);

        for (const QPoint& pt : selectedPts2) {
            painter->drawRect(QRectF(pt.x(), pt.y(), 1, 1));  // 给选中方块画绿色边框
        }
    }

}


//绘制空白格子
void GameMap::drawBufferBox(QPainter* painter, int j , int i) const
{
    QPixmap boxImg(boxImgUrl[0]);
    painter->drawPixmap(j,i,1,1,boxImg);

    //画边框
    QPen gridPen(QColor(10,10,10,100));
    gridPen.setWidthF(0.01);
    painter->setPen(gridPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(j, i, 1, 1);

}


//绘制要消除的格子
void GameMap ::drawPairBox(QPainter* painter,int j,int i) const
{
    int type = mapType[i][j];

    const qreal cornerRadius = 0.2;  // 圆角半径
    const qreal shadowOffset = 0.02; // 阴影偏移

    // 1. 绘制背景渐变（浅灰黄 -> 暗米色）
    QLinearGradient gradient(j, i, j + 1, i + 1);
    gradient.setColorAt(0.0, QColor(245, 240, 220));  // 浅色
    gradient.setColorAt(1.0, QColor(230, 225, 200));  // 深色
    painter->setBrush(gradient);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(QRectF(j, i, 1, 1), cornerRadius, cornerRadius);

    // -------------------------------
    // 2. 绘制 box 图像（覆盖在背景上）
    if (type > 0 && type < boxImgUrl.size()) {
        QPixmap boxImg(boxImgUrl[type]);
        painter->drawPixmap(j, i, 1, 1, boxImg);
    } else if (type < 0) {
        qDebug() << "error when creating map";
    }

    // -------------------------------
    // 3. 加立体边框（模拟光源来自左上）
    // 左上亮线
    QPen lightPen(QColor(255, 255, 255, 120));
    lightPen.setWidthF(0.04);
    painter->setPen(lightPen);
    painter->drawLine(QPointF(j, i), QPointF(j + 1, i));     // top
    painter->drawLine(QPointF(j, i), QPointF(j, i + 1));     // left

    // 右下暗线
    QPen darkPen(QColor(20, 20, 20, 210));
    darkPen.setWidthF(0.04);
    painter->setPen(darkPen);
    painter->drawLine(QPointF(j + 1, i), QPointF(j + 1, i + 1)); // right
    painter->drawLine(QPointF(j, i + 1), QPointF(j + 1, i + 1)); // bottom

    // -------------------------------
    // 4. 外部柔和阴影（可选，增加浮起感）
    QPen shadowPen(QColor(0, 0, 0, 40));
    shadowPen.setWidthF(0.03);
    painter->setPen(shadowPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(QRectF(j + shadowOffset, i + shadowOffset, 1, 1),
                             cornerRadius, cornerRadius);
}



//绘制两个玩家的函数
void GameMap::drawPlayer(QPainter* painter ,int x,int y) const
{
    painter->setBrush(Qt::blue);
    painter->drawRect(x,y,1,1);
    //qDebug()<<"drawing player";
}


void GameMap::drawPlayer2(QPainter* painter ,int x,int y) const
{
    painter->setBrush(Qt::red);
    painter->drawRect(x,y,1,1);
    //qDebug()<<"drawing player";
}


//绘制道具格子
void GameMap::drawProp(QPainter * painter,int x,int y , int t) const
{
    //根据道具类型，选择对应的图像进行绘制
    switch(t)
    {
        case PROP_ADD_ONE:
        {
            drawBufferBox(painter,x,y);
            QPixmap propImg(propImgUrl[0]);
            painter->drawPixmap(x, y, 1, 1, propImg);
            break;
        }
        case PROP_FLASH:
        {
            drawBufferBox(painter,x,y);
            QPixmap propImg(propImgUrl[1]);
            painter->drawPixmap(x, y, 1, 1, propImg);
            break;
        }
        case PROP_SHUFFLE:
        {
            drawBufferBox(painter,x,y);
            QPixmap propImg(propImgUrl[2]);
            painter->drawPixmap(x, y, 1, 1, propImg);
            break;
        }
        case PROP_HINT:
        {
            drawBufferBox(painter,x,y);
            QPixmap propImg(propImgUrl[3]);
            painter->drawPixmap(x, y, 1, 1, propImg);
            break;
            break;
        }
        default: break;
    }
}




//添加玩家一的选中方格
void GameMap::addSelected(QPoint pt)
{
    //qDebug()<<"the selected pt is"<<pt;
    if(selectedPts.length()<=1) selectedPts.push_back(pt);
    update();
    //更新绘制

    if (selectedPts.length()==2)
    {
        emit checkCanLink(selectedPts[0],selectedPts[1],1);
    }
   //qDebug()<<selectedPts.length();

}


//添加玩家2额选中方格
void GameMap::addSelected2(QPoint pt)
{
    //qDebug()<<"the selected pt is"<<pt;
    selectedPts2.push_back(pt);
    update();

    if (selectedPts2.length()==2)
    {
        emit checkCanLink(selectedPts2[0],selectedPts2[1],2);
    }
}






//在flash mode下，使用鼠标可以移动玩家
void GameMap::mousePressEvent(QMouseEvent* event) {
    if(!flashMode) return;

    // 1. 与 paintEvent 相同的 viewport 计算
    int heightSide, widthSide;
    if (((double)(width()) / (double)(height())) > ((double)(totalCols) / (double)(totalRows))) {
        heightSide = height();
        widthSide = totalCols * heightSide / totalRows;
    } else {
        widthSide = width();
        heightSide = totalRows * widthSide / totalCols;
    }

    int vx = (width() - widthSide) / 2;
    int vy = (height() - heightSide) / 2;

    // 2. 与 paintEvent 相同的 widSpace/heiSpace
    double widSpace = column / (double)std::min(row, column);
    double heiSpace = row / (double)std::min(row, column);

    // 3. 把屏幕坐标映射到 window 坐标（逻辑坐标）
    //   先映射到 viewport 内比例，再映射到 window 逻辑坐标
    double windowX = (event->position().x() - vx) * ( (totalCols + 2 * widSpace) / (double)widthSide ) - widSpace;
    double windowY = (event->position().y() - vy) * ( (totalRows + 2 * heiSpace) / (double)heightSide ) - heiSpace;

    // 4. 再转成行列
    int col = static_cast<int>(windowX);
    int row = static_cast<int>(windowY);

    if (col >= 0 && col < totalCols && row >= 0 && row < totalRows) {
        QPoint clicked(col, row);

        //如果点击的是空地或者道具，直接移过去
        if(mapType[row][col]<=0)
        {
            emit flashPosition(1,clicked);
        }

        //如果点击的是格子
        if(mapType[row][col] > 0)
        {
            //surroundbuffer随机返回指定格子周围的一个空格
            //如果周围没有空格，则返回指定格子本身
            QPoint flashPoint = surroundBuffer(clicked);

            //有空格的情况：
            if (flashPoint != clicked)
            {
                emit flashPosition(1,flashPoint);
                addSelected(clicked);
            }
        }

    }
}




//surroundbuffer随机返回指定格子周围的一个空格
//如果周围没有空格，则返回指定格子本身
QPoint GameMap::surroundBuffer(QPoint pt)
{
    //用vector存储所有的候选格子
    QVector<QPoint> candidate;
    int x = pt.x();
    int y = pt.y();

    int rows = mapType.size();
    int cols = mapType[0].size();

    // up
    if (y - 1 >= 0 && mapType[y - 1][x] == 0) {
        candidate.push_back(QPoint(x, y - 1));
    }
    // down
    if (y + 1 < rows && mapType[y + 1][x] == 0) {
        candidate.push_back(QPoint(x, y + 1));
    }
    // left
    if (x - 1 >= 0 && mapType[y][x - 1] == 0) {
        candidate.push_back(QPoint(x - 1, y));
    }
    // right
    if (x + 1 < cols && mapType[y][x + 1] == 0) {
        candidate.push_back(QPoint(x + 1, y));
    }


    //如果周围没有空格，则返回指定格子本身
    if (candidate.isEmpty()) {
        return pt;
    }

    //如果周围有空格，随机返回其中的一个
    int idx = QRandomGenerator::global()->bounded(candidate.size());
    return candidate[idx];
}




void GameMap::clearSelected(int index) {
    if(index == 1)selectedPts.clear();
    else selectedPts2.clear();
    update();
}


QPoint GameMap::getSelectPt(int index){
    if(index>1) {
        return QPoint(0,0);
        qDebug()<<"index out of range";
    }
    return selectedPts[index];
}




void GameMap::shuffleMap()
{
    int rows = getRowNum();
    int cols = getColNum();
    int buffer = getBufferNum();

    QVector<int> tiles;

    // 1. 收集所有方块（去掉玩家、空格、特殊元素的话这里可以过滤）
    for (int y = buffer; y < rows + buffer; ++y) {
        for (int x = buffer; x < cols + buffer; ++x) {
            int val = mapType[y][x];
            if (val != 0 && val != -1) {
                tiles.push_back(val);
            }
        }
    }

    // 2. 随机打乱
    std::shuffle(tiles.begin(), tiles.end(), *QRandomGenerator::global());

    // 3. 重新写回
    int idx = 0;
    for (int y = buffer; y < rows + buffer; ++y) {
        for (int x = buffer; x < cols + buffer; ++x) {
            int val = mapType[y][x];
            if (val != 0 && val != -1) {
                mapType[y][x] = tiles[idx++];
            }
        }
    }

    // 4. 更新显示
    update();
}


//重置地图
void GameMap::resetMap()
{
    initMap();
    linkPath.clear();
    hintPath.clear();
}



















