#pragma once
#include <QWidget>
#include <Qvector>


//#define MAX_MAP_SIZE 60



class GameMap : public QWidget
{
    Q_OBJECT

public:
    // ===================== 构造与析构 =====================
    GameMap(QWidget* parent = nullptr); // 构造函数
    ~GameMap(); // 析构函数


    // ===================== 地图参数与访问接口 =====================

    // 设置测试用地图
    void setTestMap(const QVector<QVector<int>>& newMap) { mapType = newMap; }

    // 获取方块种类数
    int getBoxTpyeNum() { return boxType; }

    // 获取指定位置的方块类型（注意参数顺序[x, y]）
    int getBoxType(int x, int y) const { return mapType[y][x]; }

    // 获取列数
    int getColNum() const { return column; }
    // 获取行数
    int getRowNum() const { return row; }
    // 获取缓冲区宽度
    int getBufferNum() const { return buffer; }

    // 设置列数
    void setColNum(int x) { column = x; }
    // 设置行数
    void setRowNum(int x) { row = x; }
    // 设置缓冲区宽度
    void setBufferNum(int x) { buffer = x; }

    // 设置指定位置的方块类型
    void setBoxType(int x, int y, int type) { mapType[y][x] = type; }

    // 获取选中的点
    QPoint getSelectPt(int index);


    // ===================== 路径与提示相关 =====================

    // 设置连线路径
    void setLinkPath(const QVector<QPoint>& path) { linkPath = path; }

    // 设置提示路径
    void setHintPath(const QVector<QPoint>& path) { hintPath = path; }

    // 启用/禁用连线路径绘制
    void enablePaintPath() { paintPath = true; }
    void disablePaintPath() { paintPath = false; }

    // 启用/禁用提示路径绘制
    void enablePaintHint() { paintHintPath = true; }
    void disablePaintHint() { paintHintPath = false; }


    // ===================== 地图操作 =====================

    // 清除选中点
    void clearSelected(int index);
    // 添加选中点（玩家1）
    void addSelected(QPoint pt);
    // 添加选中点（玩家2）
    void addSelected2(QPoint pt);

    // 重置地图
    void resetMap();

    // 闪电模式开关
    void flashModeON() { flashMode = true; }
    void flashModeOff() { flashMode = false; }

    // 获取缓冲区包围点
    QPoint surroundBuffer(QPoint pt);

    // 随机打乱地图
    void shuffleMap();

    // 多人模式开关
    void multiModeOn() { multimode = true; }
    void multimodeOff() { multimode = false; }


signals:
    // ===================== 信号 =====================

    // 检查两点是否可连通
    void checkCanLink(QPoint, QPoint, int);
    // 闪电道具触发位置
    void flashPosition(int, QPoint);


protected:
    // ===================== 绘制相关 =====================

    // 重载绘图事件
    void paintEvent(QPaintEvent* event) override;

    // 绘制地图
    void drawMap(QPainter* painter) const;
    // 绘制缓冲区方块
    void drawBufferBox(QPainter* painter, int, int) const;
    // 绘制普通方块
    void drawPairBox(QPainter* painter, int, int) const;
    // 绘制玩家1
    void drawPlayer(QPainter* painter, int, int) const;
    // 绘制玩家2
    void drawPlayer2(QPainter* painter, int, int) const;

    // 高亮选中点
    void highlightSelectedPt(QPainter* painter) const;

    // 绘制连线路径
    void drawLinkPath(QPainter* painter) const;
    // 绘制提示路径
    void drawHintPath(QPainter* painter) const;

    // 绘制道具
    void drawProp(QPainter* painter, int, int, int) const;


    // ===================== 初始化与事件处理 =====================

    // 初始化地图
    void initMap();

    // 鼠标点击事件
    void mousePressEvent(QMouseEvent* event) override;


private:
    // ===================== 成员变量 =====================

    int row = 8;                // 行数
    int column = 13;            // 列数
    int buffer = 3;             // 缓冲区宽度

    int totalRows = row + 2 * buffer; // 总行数（含缓冲区）
    int totalCols = column + 2 * buffer; // 总列数（含缓冲区）

    QList<QList<int>> mapType;  // 地图数据

    QVector<QPoint> linkPath;   // 连线路径
    bool paintPath = true;      // 是否绘制连线路径

    QVector<QPoint> hintPath;   // 提示路径
    bool paintHintPath = true;  // 是否绘制提示路径

    QVector<QPoint> selectedPts;    // 玩家1选中点
    QVector<QPoint> selectedPts2;   // 玩家2选中点

    int boxType = 20;           // 方块种类数
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

    QVector<QString> propImgUrl = {
        ":/img/images/prop_img/timer.png",
        ":/img/images/prop_img/flash.png",
        ":/img/images/prop_img/shuffle.png",
        ":/img/images/prop_img/hint.png"
    };

    bool flashMode = false;     // 闪电模式
    bool multimode = false;     // 多人模式

};
