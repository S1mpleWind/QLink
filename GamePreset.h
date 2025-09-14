
#pragma once

// ===================== 游戏参数 =====================

// 游戏总时长（秒）
extern int GAME_TIME;

// 道具生成间隔（毫秒）
extern int PROP_GENERATE_TIME;

// 增加时间道具的加成（秒）
extern int ADD_TIME;

// 闪电道具持续时间（毫秒）
extern int FLASH_TIME;

// 提示道具持续时间（毫秒）
extern int HINT_TIME;

// 缓冲区宽度
extern int BUFFER_NUM;
// 列数
extern int COL_NUM;
// 行数
extern int ROW_NUM;


// ===================== 地图格子类型枚举 =====================

enum CellType {
    PROP_ADD_ONE = -5,   // 增加时间道具
    PROP_SHUFFLE = -6,   // 洗牌道具
    PROP_HINT    = -7,   // 提示道具
    PROP_FLASH   = -8,   // 闪电道具
    //PROP_FREEZE  = -9  // 冻结道具（预留）
};
