#pragma once

// 定义地图格子类型
extern int GAME_TIME  ;

extern int PROP_GENERATE_TIME ; //(ms)

extern int ADD_TIME ;

extern int FLASH_TIME ;

extern int HINT_TIME;

extern int BUFFER_NUM ;
extern int COL_NUM;
extern int ROW_NUM;

enum CellType {
    PROP_ADD_ONE = -5,
    PROP_SHUFFLE = -6,
    PROP_HINT    = -7,
    PROP_FLASH   = -8,
    //PROP_FREEZE  = -9
};
