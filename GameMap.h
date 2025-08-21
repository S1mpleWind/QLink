#pragma once
#include <QWidget>
#include <Qvector>


//#define MAX_MAP_SIZE 60


class GameMap : public QWidget
{
    Q_OBJECT

public:
    GameMap(QWidget* parent = nullptr);
    ~GameMap();

    //ATTENTION :
    //parameter for getBoxType and setBoxType are [x,y]
    int getBoxTpyeNum(){    return boxType;}
    int getBoxType(int x, int y) const {
        // change the sequnce of x and y to match the array and coordinates system
        return mapType[y][x]; }
    int getColNum() const {return column;}
    int getRowNum() const {return row;}
    int getBufferNum() const {return buffer;}

    void setBoxType(int x, int y, int type) { mapType[y][x] = type; }
    QPoint getSelectPt(int index);

    void setLinkPath(const QVector<QPoint> & path){
        linkPath = path;
    }
    void setHintPath(const QVector<QPoint> & path)
    {
        hintPath = path ;
    }



    void enablePaintPath(){paintPath = true;
        qDebug()<<"enable paint path";}
    void disablePaintPath(){paintPath = false;
        qDebug()<<"disable paint path";}

    void enablePaintHint(){paintHintPath = true;
        qDebug()<<"enable paint hint";}
    void disablePaintHint(){paintHintPath = false;
        qDebug()<<"disable paint hint";}

    void clearSelected();
    void addSelected(QPoint);

    void resetMap(){initMap();}

    void flashModeON(){ flashMode = true ; }
    void flashModeOff(){flashMode = false; }

    QPoint surroundBuffer(QPoint);

    void shuffleMap();




//public slots:
    //void updatePlayerPosition(QPoint);



signals:
    void checkCanLink(QPoint,QPoint);
    void flashPosition(QPoint);


protected:
    void paintEvent(QPaintEvent* event) override;

    void drawMap(QPainter *painter) const;
    void drawBufferBox(QPainter* painter,int , int) const;
    void drawPairBox(QPainter* painter , int , int) const;
    void drawPlayer(QPainter* painter,int,int) const;
    void highlightSelectedPt(QPainter* painter) const;

    void drawLinkPath(QPainter* painter) const;
    void drawHintPath(QPainter* painter) const;


    void drawProp(QPainter *painter,int,int,int) const;



    void initMap();
    //void loadUrl();


    //void keyPressEvent(QKeyEvent* event) override;



    void mousePressEvent(QMouseEvent* event) override;



private:

    int row = 8;
    int column = 13;
    int buffer = 3 ;

    int totalRows = row + 2 * buffer;
    int totalCols = column + 2 * buffer;

    QList<QList<int>> mapType;
    //QVector<QPoint> edgePts;


    QVector<QPoint> linkPath;
    bool paintPath = true;

    QVector<QPoint> hintPath;
    bool paintHintPath = true;


    QVector<QPoint> selectedPts;

    int boxType = 20 ;
    QVector<QString> boxImgUrl = {
        ":/img/images/box_img/blockroad.png",
        ":/img/images/box_img/Berries.png",
        ":/img/images/box_img/Butterfly_Wings.png",
        ":/img/images/box_img/Carrot.png",
        ":/img/images/box_img/Cut_Grass.png",
        ":/img/images/box_img/Cut_Reeds.png",
        ":/img/images/box_img/Drumstick.png",
        ":/img/images/box_img/Egg.png",
        ":/img/images/box_img/Flint.png",
        ":/img/images/box_img/Gold_Nugget.png",
        ":/img/images/box_img/Green_Cap.png",
        ":/img/images/box_img/Log.png",
        ":/img/images/box_img/Mandrake.png",
        ":/img/images/box_img/Meat.png",
        ":/img/images/box_img/Nitre.png",
        ":/img/images/box_img/Pig_Skin.png",
        ":/img/images/box_img/Red_Cap.png",
        ":/img/images/box_img/Rocks.png",
        ":/img/images/box_img/Toma_Root.png",
        ":/img/images/box_img/Twigs.png",
        ":/img/images/box_img/Watermelon.png"
    };


    QVector<QString>propImgUrl={
        ":/img/images/prop_img/timer.png",
        ":/img/images/prop_img/flash.png",
        ":/img/images/prop_img/shuffle.png",
        ":/img/images/prop_img/hint.png"
    };


    bool flashMode = false ;

};
