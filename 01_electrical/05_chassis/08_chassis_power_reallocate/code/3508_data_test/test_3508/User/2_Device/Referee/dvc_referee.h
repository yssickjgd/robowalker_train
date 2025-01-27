/**
 * @file dvc_referee.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief PM01裁判系统
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-01-30 1.1 适配1.6.1通信协议
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/**
 * Command_ID分全局, 己方, 指定机器人
 * 0x00xx, 全局是Game
 * 0x01xx, 己方是Event_Self
 * 0x02xx, 指定机器人是Robot_x, 没有x则是任意己方机器人
 * 0x03xx的指令帧直接具体指明收发方
 */

#ifndef DVC_REFEREE_H
#define DVC_REFEREE_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/1_Driver/UART/drv_uart.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 裁判系统状态
 *
 */
enum Enum_Referee_Status
{
    Referee_Status_DISABLE = 0,
    Referee_Status_ENABLE,
};

/**
 * @brief 各种标签, 场地, 相关设施激活与存活状态
 *
 */
enum Enum_Referee_Data_Status : uint8_t
{
    Referee_Data_Status_DISABLE = 0,
    Referee_Data_Status_ENABLE,
};

/**
 * @brief 裁判系统命令码类型
 *
 */
enum Enum_Referee_Command_ID : uint16_t
{
    Referee_Command_ID_GAME_STATUS = 0x0001,
    Referee_Command_ID_GAME_RESULT,
    Referee_Command_ID_GAME_ROBOT_HP,
    Referee_Command_ID_EVENT_SELF_DATA = 0x0101,
    Referee_Command_ID_EVENT_SELF_SUPPLY,
    Referee_Command_ID_EVENT_SELF_REFEREE_WARNING = 0x0104,
    Referee_Command_ID_EVENT_SELF_DART_STATUS,
    Referee_Command_ID_ROBOT_STATUS = 0x0201,
    Referee_Command_ID_ROBOT_POWER_HEAT,
    Referee_Command_ID_ROBOT_POSITION,
    Referee_Command_ID_ROBOT_BUFF,
    Referee_Command_ID_ROBOT_AERIAL_STATUS,
    Referee_Command_ID_ROBOT_DAMAGE,
    Referee_Command_ID_ROBOT_BOOSTER,
    Referee_Command_ID_ROBOT_REMAINING_AMMO,
    Referee_Command_ID_ROBOT_RFID,
    Referee_Command_ID_ROBOT_DART_COMMAND,
    Referee_Command_ID_ROBOT_SENTRY_LOCATION,
    Referee_Command_ID_ROBOT_RADAR_MARK,
    Referee_Command_ID_ROBOT_SENTRY_DECISION,
    Referee_Command_ID_ROBOT_RADAR_DECISION,
    Referee_Command_ID_INTERACTION = 0x0301,
    Referee_Command_ID_INTERACTION_ROBOT_RECEIVE_CUSTOM_CONTROLLER,
    Referee_Command_ID_INTERACTION_ROBOT_RECEIVE_CLIENT_MINIMAP,
    Referee_Command_ID_INTERACTION_ROBOT_RECEIVE_CLIENT_REMOTE_CONTROL,
    Referee_Command_ID_INTERACTION_CLIENT_RECEIVE_RADAR,
    Referee_Command_ID_INTERACTION_CLIENT_RECEIVE_CUSTOM_CONTROLLER,
    Referee_Command_ID_INTERACTION_CLIENT_RECEIVE_SENTRY_SEMIAUTOMATIC_MINIMAP,
    Referee_Command_ID_INTERACTION_CLIENT_RECEIVE_ROBOT_MINIMAP,
};

/**
 * @brief 裁判系统 interaction 0x0301子命令码类型
 *
 */
enum Enum_Referee_Interaction_Command_ID : uint16_t
{
    Referee_Interaction_Command_ID_UI_LAYER_DELETE = 0x0100,
    Referee_Interaction_Command_ID_UI_GRAPHIC_1,
    Referee_Interaction_Command_ID_UI_GRAPHIC_2,
    Referee_Interaction_Command_ID_UI_GRAPHIC_5,
    Referee_Interaction_Command_ID_UI_GRAPHIC_7,
    Referee_Interaction_Command_ID_UI_GRAPHIC_STRING = 0x0110,
    Referee_Interaction_Command_ID_SENTRY = 0x0120,
    Referee_Interaction_Command_ID_RADAR = 0x0121,
};

/**
 * @brief 通用单方机器人ID
 *
 */
enum Enum_Referee_Data_Robot_ID : uint8_t
{
    Referee_Data_Robot_ID_NULL = 0,
    Referee_Data_Robot_ID_HERO_1,
    Referee_Data_Robot_ID_ENGINEER_2,
    Referee_Data_Robot_ID_INFANTRY_3,
    Referee_Data_Robot_ID_INFANTRY_4,
    Referee_Data_Robot_ID_INFANTRY_5,
    Referee_Data_Robot_ID_AERIAL_6,
    Referee_Data_Robot_ID_SENTRY_7,
    Referee_Data_Robot_ID_DART_8,
    Referee_Data_Robot_ID_RADAR_9,
    Referee_Data_Robot_ID_BASE_10,
    Referee_Data_Robot_ID_OUTPOST_11,
};

/**
 * @brief 通用双方机器人ID
 *
 */
enum Enum_Referee_Data_Robots_ID : uint8_t
{
    Referee_Data_Robots_ID_NO = 0,
    Referee_Data_Robots_ID_RED_HERO_1,
    Referee_Data_Robots_ID_RED_ENGINEER_2,
    Referee_Data_Robots_ID_RED_INFANTRY_3,
    Referee_Data_Robots_ID_RED_INFANTRY_4,
    Referee_Data_Robots_ID_RED_INFANTRY_5,
    Referee_Data_Robots_ID_RED_AERIAL_6,
    Referee_Data_Robots_ID_RED_SENTRY_7,
    Referee_Data_Robots_ID_RED_DART_8,
    Referee_Data_Robots_ID_RED_RADAR_9,
    Referee_Data_Robots_ID_RED_BASE_10,
    Referee_Data_Robots_ID_RED_OUTPOST_11,
    Referee_Data_Robots_ID_BLUE_HERO_1 = 101,
    Referee_Data_Robots_ID_BLUE_ENGINEER_2,
    Referee_Data_Robots_ID_BLUE_INFANTRY_3,
    Referee_Data_Robots_ID_BLUE_INFANTRY_4,
    Referee_Data_Robots_ID_BLUE_INFANTRY_5,
    Referee_Data_Robots_ID_BLUE_AERIAL_6,
    Referee_Data_Robots_ID_BLUE_SENTRY_7,
    Referee_Data_Robots_ID_BLUE_DART_8,
    Referee_Data_Robots_ID_BLUE_RADAR_9,
    Referee_Data_Robots_ID_BLUE_BASE_10,
    Referee_Data_Robots_ID_BLUE_OUTPOST_11,
};
/**
 * @brief 通用双方机器人ID
 *
 */
enum Enum_Referee_Data_Robots_Client_ID : uint16_t
{
    Referee_Data_Robots_Client_ID_NO = 0,
    Referee_Data_Robots_Client_ID_RED_HERO_1 = 0x0101,
    Referee_Data_Robots_Client_ID_RED_ENGINEER_2,
    Referee_Data_Robots_Client_ID_RED_INFANTRY_3,
    Referee_Data_Robots_Client_ID_RED_INFANTRY_4,
    Referee_Data_Robots_Client_ID_RED_INFANTRY_5,
    Referee_Data_Robots_Client_ID_RED_AERIAL_6,
    Referee_Data_Robots_Client_ID_BLUE_HERO_1 = 0x0165,
    Referee_Data_Robots_Client_ID_BLUE_ENGINEER_2,
    Referee_Data_Robots_Client_ID_BLUE_INFANTRY_3,
    Referee_Data_Robots_Client_ID_BLUE_INFANTRY_4,
    Referee_Data_Robots_Client_ID_BLUE_INFANTRY_5,
    Referee_Data_Robots_Client_ID_BLUE_AERIAL_6,
    Referee_Data_Robots_Server = 0x8080,
};

/**
 * @brief 比赛类型
 *
 */
enum Enum_Referee_Game_Status_Type
{
    Referee_Game_Status_Type_RMUC = 1,
    Referee_Game_Status_Type_SINGLE,
    Referee_Game_Status_Type_ICRA,
    Referee_Game_Status_Type_RMUL_3V3,
    Referee_Game_Status_Type_RMUL_1V1,
};

/**
 * @brief 比赛阶段
 *
 */
enum Enum_Referee_Game_Status_Stage
{
    Referee_Game_Status_Stage_NOT_STARTED = 0,
    Referee_Game_Status_Stage_READY,
    Referee_Game_Status_Stage_15s_SELF_TESTING,
    Referee_Game_Status_Stage_5S_COUNTDOWN,
    Referee_Game_Status_Stage_BATTLE,
    Referee_Game_Status_Stage_SETTLEMENT,
};

/**
 * @brief 比赛结果
 *
 */
enum Enum_Referee_Game_Result : uint8_t
{
    Referee_Game_Result_DRAW = 0,
    Referee_Game_Result_RED_WIN,
    Referee_Game_Result_BLUE_WIN,
};

/**
 * @brief 飞镖命中目标
 *
 */
enum Enum_Referee_Dart_Hit_Target : uint8_t
{
    Referee_Dart_Hit_Target_NULL = 0,
    Referee_Dart_Hit_Target_OUTPOST,
    Referee_Dart_Hit_Target_BASE_STATIC,
    Referee_Dart_Hit_Target_BASE_RANDOM,
};

/**
 * @brief 补给站状态
 *
 */
enum Enum_Referee_Data_Event_Supply_Status : uint8_t
{
    Referee_Data_Event_Supply_Status_CLOSED = 0,
    Referee_Data_Event_Supply_Status_READY,
    Referee_Data_Event_Supply_Status_DROPPING,
};

/**
 * @brief 补给站提供子弹数量
 *
 */
enum Enum_Referee_Data_Event_Supply_Ammo_Number : uint8_t
{
    Referee_Data_Event_Supply_Ammo_Number_50 = 50,
    Referee_Data_Event_Supply_Ammo_Number_100 = 100,
    Referee_Data_Event_Supply_Ammo_Number_150 = 150,
    Referee_Data_Event_Supply_Ammo_Number_200 = 200,
};

/**
 * @brief 裁判警告等级
 *
 */
enum Enum_Referee_Data_Event_Referee_Warning_Level : uint8_t
{
    Referee_Data_Referee_Warning_Level_BOTH_YELLOW = 1,
    Referee_Data_Referee_Warning_Level_YELLOW,
    Referee_Data_Referee_Warning_Level_RED,
    Referee_Data_Referee_Warning_Level_FAIL,
};

/**
 * @brief 裁判警告等级
 *
 */
enum Enum_Referee_Data_Event_Aerial_Status : uint8_t
{
    Referee_Data_Event_Aerial_Status_COOLING = 0,
    Referee_Data_Event_Aerial_Status_READY,
    Referee_Data_Event_Aerial_Status_EXECUTING,
};

/**
 * @brief 伤害类型
 *
 */
enum Enum_Referee_Data_Event_Robot_Damage_Type
{
    Referee_Data_Robot_Damage_Type_ARMOR_ATTACKED = 0,
    Referee_Data_Robot_Damage_Type_MODULE_OFFLINE,
    Referee_Data_Robot_Damage_Type_BOOSTER_SPEED,
    Referee_Data_Robot_Damage_Type_BOOSTER_HEAT,
    Referee_Data_Robot_Damage_Type_CHASSIS_POWER,
    Referee_Data_Robot_Damage_Type_ARMOR_COLLISION,
};

/**
 * @brief 子弹类型
 *
 */
enum Enum_Referee_Data_Robot_Ammo_Type : uint8_t
{
    Referee_Data_Robot_Ammo_Type_BOOSTER_17MM = 1,
    Referee_Data_Robot_Ammo_Type_BOOSTER_42mm,
};

/**
 * @brief 发射机构类型
 *
 */
enum Enum_Referee_Data_Robot_Booster_Type : uint8_t
{
    Referee_Data_Robot_Booster_Type_BOOSTER_17MM_1 = 1,
    Referee_Data_Robot_Booster_Type_BOOSTER_17MM_2,
    Referee_Data_Robot_Booster_Type_BOOSTER_42mm,
};

/**
 * @brief 飞镖发射口状态
 *
 */
enum Enum_Referee_Data_Robot_Dart_Command_Status : uint8_t
{
    Referee_Data_Robot_Dart_Command_Status_OPEN = 0,
    Referee_Data_Robot_Dart_Command_Status_CLOSED,
    Referee_Data_Robot_Dart_Command_Status_EXECUTING,
};

/**
 * @brief 飞镖击打目标
 *
 */
enum Enum_Referee_Data_Robot_Dart_Command_Target : uint8_t
{
    Referee_Data_Robot_Dart_Command_Target_OUTPOST = 0,
    Referee_Data_Robot_Dart_Command_Target_BASE,
};

/**
 * @brief 图形操作交互信息
 *
 */
enum Enum_Referee_Data_Interaction_Layer_Delete_Operation : uint8_t
{
    Referee_Data_Interaction_Layer_Delete_Operation_NULL = 0,
    Referee_Data_Interaction_Layer_Delete_Operation_CLEAR_ONE,
    Referee_Data_Interaction_Layer_Delete_Operation_CLEAR_ALL,
};

/**
 * @brief 图形操作
 *
 */
enum Enum_Referee_Data_Interaction_Graphic_Operation
{
    Referee_Data_Interaction_Graphic_Operation_NULL = 0,
    Referee_Data_Interaction_Graphic_Operation_ADD,
    Referee_Data_Interaction_Graphic_Operation_CHANGE,
    Referee_Data_Interaction_Graphic_Operation_DELETE,
};

/**
 * @brief 图形类型
 *
 */
enum Enum_Referee_Data_Interaction_Graphic_Type
{
    Referee_Data_Interaction_Graphic_Type_LINE = 0,
    Referee_Data_Interaction_Graphic_Type_RECTANGLE,
    Referee_Data_Interaction_Graphic_Type_CIRCLE,
    Referee_Data_Interaction_Graphic_Type_OVAL,
    Referee_Data_Interaction_Graphic_Type_ARC,
    Referee_Data_Interaction_Graphic_Type_FLOAT,
    Referee_Data_Interaction_Graphic_Type_INTEGER,
    Referee_Data_Interaction_Graphic_Type_STRING,
};

/**
 * @brief 图形颜色
 *
 */
enum Enum_Referee_Data_Interaction_Graphic_Color
{
    Referee_Data_Interaction_Graphic_Color_MAIN = 0,
    Referee_Data_Interaction_Graphic_Color_YELLOW,
    Referee_Data_Interaction_Graphic_Color_GREEN,
    Referee_Data_Interaction_Graphic_Color_ORANGE,
    Referee_Data_Interaction_Graphic_Color_PURPLE,
    Referee_Data_Interaction_Graphic_Color_PINK,
    Referee_Data_Interaction_Graphic_Color_CYAN,
    Referee_Data_Interaction_Graphic_Color_BLACK,
    Referee_Data_Interaction_Graphic_Color_WHITE,
};

/**
 * @brief 图形操作交互信息
 *
 */
enum Enum_Referee_Data_Interaction_Semiautomatic_Command : uint8_t
{
    Referee_Data_Interaction_Semiautomatic_Command_ATTACK = 1,
    Referee_Data_Interaction_Semiautomatic_Command_DEFENCE,
    Referee_Data_Interaction_Semiautomatic_Command_MOVE,
};

/**
 * @brief 图形配置结构体
 *
 */
struct Struct_Referee_Data_Interaction_Graphic_Config
{
    uint8_t Index[3];
    uint32_t Operation_Enum : 3;
    uint32_t Type_Enum : 3;
    uint32_t Layer_Num : 4;
    uint32_t Color_Enum : 4;
    uint32_t Details_A : 9;
    uint32_t Details_B : 9;
    uint32_t Line_Width : 10;
    uint32_t Start_X : 11;
    uint32_t Start_Y : 11;
    uint32_t Details_C : 10;
    uint32_t Details_D : 11;
    uint32_t Details_E : 11;
} __attribute__((packed));

// TODO
// /**
//  * @brief 图形配置结构体
//  *
//  */
// struct Struct_Referee_Data_Interaction_Graphic_Config_test
// {
//     uint8_t Index[3];
//     Enum_Referee_Data_Interaction_Graphic_Operation Operation : 3;
//     Enum_Referee_Data_Interaction_Graphic_Type Type : 3;
//     uint8_t Layer_Num : 4;
//     Enum_Referee_Data_Interaction_Graphic_Color Color : 4;
//     uint32_t Details_A : 9;
//     uint32_t Details_B : 9;
//     uint32_t Line_Width : 10;
//     uint32_t Start_X : 11;
//     uint32_t Start_Y : 11;
//     uint32_t Details_C : 10;
//     uint32_t Details_D : 11;
//     uint32_t Details_E : 11;
// } __attribute__((packed));

/**
 * @brief 裁判系统源数据
 *
 */
struct Struct_Referee_UART_Data
{
    uint8_t Frame_Header = 0xa5;
    uint16_t Data_Length;
    uint8_t Sequence;
    uint8_t CRC_8;
    Enum_Referee_Command_ID Referee_Command_ID;
    uint8_t Data[121];
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0001比赛状态, 3Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Game_Status
{
    uint8_t Type_Enum : 4;
    uint8_t Stage_Enum : 4;
    uint16_t Remaining_Time;
    uint64_t Timestamp;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0002比赛结果, 比赛结束后发送
 *
 */
struct Struct_Referee_Rx_Data_Game_Result
{
    Enum_Referee_Game_Result Result;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0003机器人血量, 1Hz
 *
 */
struct Struct_Referee_Rx_Data_Game_Robot_HP
{
    uint16_t Red_Hero_1;
    uint16_t Red_Engineer_2;
    uint16_t Red_Infantry_3;
    uint16_t Red_Infantry_4;
    uint16_t Red_Infantry_5;
    uint16_t Red_Sentry_7;
    uint16_t Red_Outpost_11;
    uint16_t Red_Base_10;
    uint16_t Blue_Hero_1;
    uint16_t Blue_Engineer_2;
    uint16_t Blue_Infantry_3;
    uint16_t Blue_Infantry_4;
    uint16_t Blue_Infantry_5;
    uint16_t Blue_Sentry_7;
    uint16_t Blue_Outpost_11;
    uint16_t Blue_Base_10;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0101场地事件, 1Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Event_Self_Data
{
    uint32_t Supply_Front_Status_Enum : 1;
    uint32_t Supply_Inner_Status_Enum : 1;
    uint32_t Supply_Status_Enum : 1;
    uint32_t Power_Rune_Status_Enum : 1;
    uint32_t Power_Rune_Small_Status_Enum : 1;
    uint32_t Power_Rune_Big_Status_Enum : 1;
    uint32_t Highland_2_Status_Enum : 2;
    uint32_t Highland_3_Status_Enum : 2;
    uint32_t Highland_4_Status_Enum : 2;
    uint32_t Base_Virtual_Shield_Percent : 7;
    uint32_t Enemy_Dart_Hit_Time : 9;
    uint32_t Enemy_Dart_Hit_Target_Enum : 2;
    uint32_t Middle_Buff_Status_Enum : 2;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0102补给站状态, 补给请求后对应发送
 *
 */
struct Struct_Referee_Rx_Data_Event_Self_Supply
{
    uint8_t Reserved;
    Enum_Referee_Data_Robots_ID Robot;
    Enum_Referee_Data_Event_Supply_Status Status;
    Enum_Referee_Data_Event_Supply_Ammo_Number Ammo_Number;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0104裁判警告信息, 判罚发生后发送
 *
 */
struct Struct_Referee_Rx_Data_Event_Referee_Warning
{
    Enum_Referee_Data_Event_Referee_Warning_Level Level;
    Enum_Referee_Data_Robot_ID Robot_ID;
    uint8_t Count;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0105飞镖15s倒计时, 1Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Event_Dart_Status
{
    uint8_t Dart_Remaining_Time;
    uint16_t Dart_Hit_Target_Enum_Last : 2;
    uint16_t Dart_Hit_Target_Count : 3;
    uint16_t Dart_Hit_Target_Enum_Now : 2;
    uint16_t Reserved : 9;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0201机器人状态, 10Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Status
{
    Enum_Referee_Data_Robots_ID Robot_ID;
    uint8_t Level;
    uint16_t HP;
    uint16_t HP_Max;
    uint16_t Booster_Heat_CD;
    uint16_t Booster_Heat_Max;
    uint16_t Chassis_Power_Max;
    uint8_t PM01_Gimbal_Status_Enum : 1;
    uint8_t PM01_Chassis_Status_Enum : 1;
    uint8_t PM01_Booster_Status_Enum : 1;
    uint8_t Reserved : 5;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0202当前机器人实时功率热量, 50Hz发送
 * 电压mV, 电流mA
 *
 */
struct Struct_Referee_Rx_Data_Robot_Power_Heat
{
    uint16_t Chassis_Voltage;
    uint16_t Chassis_Current;
    float Chassis_Power;
    uint16_t Chassis_Energy_Buffer;
    uint16_t Booster_17mm_1_Heat;
    uint16_t Booster_17mm_2_Heat;
    uint16_t Booster_42mm_Heat;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0203当前机器人实时位置, 10Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Position
{
    float Location_X;
    float Location_Y;
    float Location_Yaw;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0204当前机器人增益, 1Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Buff
{
    uint8_t HP_Buff_Percent;
    uint8_t Booster_Heat_CD_Buff_Value;
    uint8_t Defend_Buff_Percent;
    uint8_t Defend_Debuff_Percent;
    uint16_t Damage_Buff_Percent;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0205无人机可攻击时间, 10Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Aerial_Status
{
    Enum_Referee_Data_Event_Aerial_Status Aerial_Status;
    uint8_t Remaining_Time;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0206伤害情况, 伤害发生后发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Damage
{
    uint8_t Armor_ID : 4;
    uint8_t Type_Enum : 4;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0207子弹信息, 射击发生后发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Booster
{
    Enum_Referee_Data_Robot_Ammo_Type Ammo_Type;
    Enum_Referee_Data_Robot_Booster_Type Booster_Type;
    uint8_t Frequency;
    float Speed;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0208子弹剩余信息, 10Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Remaining_Ammo
{
    uint16_t Booster_17mm;
    uint16_t Booster_42mm;
    uint16_t Money;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x0209RFID状态信息, 1Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_RFID
{
    uint32_t Base_Status_Enum : 1;
    uint32_t Highland_2_Self_Status_Enum : 1;
    uint32_t Highland_2_Enemy_Status_Enum : 1;
    uint32_t Highland_3_Self_Status_Enum : 1;
    uint32_t Highland_3_Enemy_Status_Enum : 1;
    uint32_t Highland_4_Self_Status_Enum : 1;
    uint32_t Highland_4_Enemy_Status_Enum : 1;
    uint32_t Energy_Status_Enum : 1;
    uint32_t Flyover_1_Self_Status_Enum : 1;
    uint32_t Flyover_2_Self_Status_Enum : 1;
    uint32_t Flyover_1_Enemy_Status_Enum : 1;
    uint32_t Flyover_2_Enemy_Status_Enum : 1;
    uint32_t Outpost_Status_Enum : 1;
    uint32_t HP_Status_Enum : 1;
    uint32_t Sentry_Self_Status_Enum : 1;
    uint32_t Sentry_Enemy_Status_Enum : 1;
    uint32_t Engineer_Self_Status_Enum : 1;
    uint32_t Engineer_Enemy_Status_Enum : 1;
    uint32_t Engineer_Exchange_Status_Enum : 1;
    uint32_t Middle_Status_Enum : 1;
    uint32_t Reserved : 12;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x020a飞镖状态, 10Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Dart_Command
{
    Enum_Referee_Data_Robot_Dart_Command_Status Status;
    uint8_t Reserved;
    uint16_t Switch_Remaining_Time;
    uint16_t Launch_Remaining_Time;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x020b哨兵获取己方位置信息, 1Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Sentry_Location
{
    float Hero_1_X;
    float Hero_1_Y;
    float Engineer_2_X;
    float Engineer_2_Y;
    float Infantry_3_X;
    float Infantry_3_Y;
    float Infantry_4_X;
    float Infantry_4_Y;
    float Infantry_5_X;
    float Infantry_5_Y;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x020c雷达标记进度, 1Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Radar_Mark
{
    uint8_t Hero_1_Mark;
    uint8_t Engineer_2_Mark;
    uint8_t Infantry_3_Mark;
    uint8_t Infantry_4_Mark;
    uint8_t Infantry_5_Mark;
    uint8_t Sentry_7_Mark;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x020d哨兵决策信息, 1Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Sentry_Decision
{
    uint32_t Ammo_Exchange_Number : 11;
    uint32_t Ammo_Exchange_Time : 4;
    uint32_t HP_Exchange_Time : 4;
    uint32_t Reserved : 13;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统经过处理的数据, 0x020e雷达决策信息, 1Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Robot_Radar_Decision
{
    uint8_t Double_Damage_Chance : 2;
    uint8_t Double_Damage_Enemy_Status_Enum : 1;
    uint8_t Reserved : 5;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0301机器人间交互信息, 用户自主发送
 * Header 0x0200~0x02ff, 机器人之间通信
 * Header 0x0100~0x0104, UI绘制图形
 * Header 0x0110, UI绘制字符
 * Header 0x0120, 哨兵决策
 * Header 0x0121, 雷达决策
 * Data 最大112
 *
 */
// struct Struct_Referee_Data_Interaction
// {
//     uint16_t Header;
//     Enum_Referee_Data_Robots_ID Sender;
//     Enum_Referee_Data_Robots_ID Receiver;
//     uint8_t Data[113];
//     uint16_t CRC_16;
// }__attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0301图形删除交互信息, 用户自主发送
 * Header 0x0100
 *
 */
struct Struct_Referee_Tx_Data_Interaction_Layer_Delete
{
    uint16_t Header = Referee_Interaction_Command_ID_UI_LAYER_DELETE;
    Enum_Referee_Data_Robots_ID Sender;
    uint8_t Reserved;
    Enum_Referee_Data_Robots_Client_ID Receiver;
    Enum_Referee_Data_Interaction_Layer_Delete_Operation Operation;
    uint8_t Delete_Serial;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0301画一个图形交互信息, 用户自主发送
 *
 */
struct Struct_Referee_Tx_Data_Interaction_Graphic_1
{
    uint16_t Header = Referee_Interaction_Command_ID_UI_GRAPHIC_1;
    Enum_Referee_Data_Robots_ID Sender;
    uint8_t Reserved;
    Enum_Referee_Data_Robots_Client_ID Receiver;
    Struct_Referee_Data_Interaction_Graphic_Config Graphic[1];
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0301画两个图形交互信息, 用户自主发送
 *
 */
struct Struct_Referee_Tx_Data_Interaction_Graphic_2
{
    uint16_t Header = Referee_Interaction_Command_ID_UI_GRAPHIC_2;
    Enum_Referee_Data_Robots_ID Sender;
    uint8_t Reserved;
    Enum_Referee_Data_Robots_Client_ID Receiver;
    Struct_Referee_Data_Interaction_Graphic_Config Graphic[2];
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0301画五个图形交互信息, 用户自主发送
 *
 */
struct Struct_Referee_Tx_Data_Interaction_Graphic_5
{
    uint16_t Header = Referee_Interaction_Command_ID_UI_GRAPHIC_5;
    Enum_Referee_Data_Robots_ID Sender;
    uint8_t Reserved;
    Enum_Referee_Data_Robots_Client_ID Receiver;
    Struct_Referee_Data_Interaction_Graphic_Config Graphic[5];
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0301画七个图形交互信息, 用户自主发送
 *
 */
struct Struct_Referee_Tx_Data_Interaction_Graphic_7
{
    uint16_t Header = Referee_Interaction_Command_ID_UI_GRAPHIC_7;
    Enum_Referee_Data_Robots_ID Sender;
    uint8_t Reserved;
    Enum_Referee_Data_Robots_Client_ID Receiver;
    Struct_Referee_Data_Interaction_Graphic_Config Graphic[7];
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0301画字符图形交互信息, 用户自主发送
 *
 */
struct Struct_Referee_Tx_Data_Interaction_Graphic_String
{
    uint16_t Header = Referee_Interaction_Command_ID_UI_GRAPHIC_STRING;
    Enum_Referee_Data_Robots_ID Sender;
    uint8_t Reserved;
    Enum_Referee_Data_Robots_Client_ID Receiver;
    Struct_Referee_Data_Interaction_Graphic_Config Graphic_String;
    uint8_t String[30];
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0301哨兵自主决策交互信息, 哨兵自主发送
 *
 */
struct Struct_Referee_Tx_Data_Interaction_Sentry
{
    uint16_t Header = Referee_Interaction_Command_ID_SENTRY;
    uint32_t Confirm_Respawn_Status_Enum : 1;
    uint32_t Confirm_Exchange_Respawn_Status_Enum : 1;
    uint32_t Request_Exchange_Ammo_Number : 11;
    uint32_t Request_Exchange_Ammo_Time : 4;
    uint32_t Request_Exchange_HP_Time : 4;
    uint32_t Reserved : 11;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0301雷达自主决策交互信息, 雷达自主发送
 *
 */
struct Struct_Referee_Tx_Data_Interaction_Radar
{
    uint16_t Header = Referee_Interaction_Command_ID_RADAR;
    Enum_Referee_Data_Status Request_Double_Damage;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0302自定义控制器交互信息, 用户自主发送
 * TODO 视情况赋予Data含义
 *
 */
struct Struct_Referee_Rx_Data_Interaction_Custom_Controller
{
    uint8_t Data[30];
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0303客户端发送小地图交互信息, 用户自主最高2Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Interaction_Robot_Receive_Client_Minimap
{
    float Coordinate_X;
    float Coordinate_Y;
    Enum_Referee_Data_Status Keyboard;
    Enum_Referee_Data_Robots_ID Enemy_ID;
    Enum_Referee_Data_Robots_ID Source_ID;
    uint8_t Reserved;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统发送的数据, 0x0304图传键鼠遥控交互信息, 30Hz发送
 * TODO 等待扩展
 *
 */
struct Struct_Referee_Rx_Data_Interaction_Robot_Receive_Client_Remote_Control
{
    uint16_t Mouse_X;
    uint16_t Mouse_Y;
    uint16_t Mouse_Z;
    Enum_Referee_Data_Status Mouse_Left_Key_Status;
    Enum_Referee_Data_Status Mouse_Right_Key_Status;
    uint16_t Keyboard_Key;
    uint16_t Reserved;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0305客户端接收小地图交互信息, 用户自主最高10Hz发送
 *
 */
struct Struct_Referee_Rx_Data_Interaction_Client_Receive_Radar
{
    Enum_Referee_Data_Robots_ID Robot_ID;
    float Coordinate_X;
    float Coordinate_Y;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0306客户端接收模拟键鼠遥控交互信息, 用户自主最高30Hz发送
 * TODO 等待扩展
 *
 */
struct Struct_Referee_Tx_Data_Interaction_Client_Receive_Custom_Controller
{
    uint16_t Keyboard_Key;
    uint16_t Mouse_X : 12;
    Enum_Referee_Data_Status Mouse_Left_Key_Status : 4;
    uint16_t Mouse_Y : 12;
    Enum_Referee_Data_Status Mouse_Right_Key_Status : 4;
    uint16_t Reserved;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0307客户端接收模拟键鼠遥控交互信息, 用户自主最高30Hz发送
 *
 */
struct Struct_Referee_Tx_Data_Interaction_Client_Receive_Sentry_Semiautomatic_Minimap
{
    Enum_Referee_Data_Interaction_Semiautomatic_Command Command;
    uint16_t Start_X;
    uint16_t Start_Y;
    int8_t Delta_X_List[49];
    int8_t Delta_Y_List[49];
    Enum_Referee_Data_Robots_Client_ID Sender_ID;
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief 裁判系统接收的数据, 0x0308客户端接收模拟键鼠遥控交互信息, 用户自主最高30Hz发送
 *
 */
struct Struct_Referee_Tx_Data_Interaction_Client_Receive_Robot_Minimap
{
    Enum_Referee_Data_Robots_Client_ID Sender_ID;
    Enum_Referee_Data_Robots_Client_ID Receiver_ID;
    uint8_t Data[30];
    uint16_t CRC_16;
} __attribute__((packed));

/**
 * @brief Specialized, 裁判系统
 *
 */
class Class_Referee
{
public:
    void Init(UART_HandleTypeDef *huart, uint8_t __Frame_Header = 0xa5);

    inline Enum_Referee_Status Get_Status();

    inline Enum_Referee_Data_Status Get_Referee_Trust_Status();

    inline Enum_Referee_Game_Status_Type Get_Game_Type();

    inline Enum_Referee_Game_Status_Stage Get_Game_Stage();

    inline uint16_t Get_Remaining_Time();

    inline uint64_t Get_Timestamp();

    inline Enum_Referee_Game_Result Get_Result();

    inline uint16_t Get_HP(Enum_Referee_Data_Robots_ID Robots_ID);

    inline Enum_Referee_Data_Status Get_Supply_Front_Status();

    inline Enum_Referee_Data_Status Get_Supply_Inner_Status();

    inline Enum_Referee_Data_Status Get_Supply_Status();

    inline Enum_Referee_Data_Status Get_Power_Rune_Status();

    inline Enum_Referee_Data_Status Get_Power_Rune_Small_Status();

    inline Enum_Referee_Data_Status Get_Power_Rune_Big_Status();

    inline Enum_Referee_Data_Status Get_Highland_2_Status();

    inline Enum_Referee_Data_Status Get_Highland_3_Status();

    inline Enum_Referee_Data_Status Get_Highland_4_Status();

    inline uint8_t Get_Base_Virtual_Shield_Percent();

    inline uint16_t Get_Enemy_Dart_Hit_Time();

    inline Enum_Referee_Dart_Hit_Target Get_Enemy_Dart_Hit_Target_Enum();

    inline Enum_Referee_Data_Status Get_Middle_Buff_Status();

    inline Enum_Referee_Data_Robots_ID Get_Supply_Request_Robot();

    inline Enum_Referee_Data_Event_Supply_Status Get_Supply_Request_Status();

    inline Enum_Referee_Data_Event_Supply_Ammo_Number Get_Supply_Ammo_Number();

    inline Enum_Referee_Data_Event_Referee_Warning_Level Get_Referee_Warning();

    inline Enum_Referee_Data_Robot_ID Get_Referee_Warning_Robot();

    inline uint8_t Get_Dart_Remaining_Time();

    inline Enum_Referee_Dart_Hit_Target Get_Last_Dart_Hit_Target();

    inline uint8_t Get_Dart_Hit_Target_Count();

    inline Enum_Referee_Dart_Hit_Target Get_Now_Dart_Hit_Target();

    inline Enum_Referee_Data_Robots_ID Get_Self_ID();

    inline uint8_t Get_Self_Level();

    inline uint16_t Get_Self_HP();

    inline uint16_t Get_Self_HP_Max();

    inline uint16_t Get_Self_Booster_Heat_CD();

    inline uint16_t Get_Self_Booster_Heat_Max();

    inline uint16_t Get_Self_Chassis_Power_Max();

    inline Enum_Referee_Data_Status Get_Self_PM01_Gimbal_Status();

    inline Enum_Referee_Data_Status Get_Self_PM01_Chassis_Status();

    inline Enum_Referee_Data_Status Get_Self_PM01_Booster_Status();

    inline float Get_Chassis_Voltage();

    inline float Get_Chassis_Current();

    inline float Get_Chassis_Power();

    inline uint16_t Get_Chassis_Energy_Buffer();

    inline uint16_t Get_Booster_17mm_1_Heat();

    inline uint16_t Get_Booster_17mm_2_Heat();

    inline uint16_t Get_Booster_42mm_Heat();

    inline float Get_Location_X();

    inline float Get_Location_Y();

    inline float Get_Location_Yaw();

    inline uint8_t Get_HP_Buff_Percent();

    inline uint8_t Get_Booster_Heat_CD_Buff_Value();

    inline uint8_t Get_Defend_Buff_Percent();

    inline uint8_t Get_Defend_Debuff_Percent();

    inline uint8_t Get_Damage_Buff_Percent();

    inline Enum_Referee_Data_Event_Aerial_Status Get_Aerial_Status();

    inline uint8_t Get_Aerial_Remaining_Time();

    inline uint8_t Get_Armor_Attacked_ID();

    inline Enum_Referee_Data_Event_Robot_Damage_Type Get_Attacked_Type();

    inline Enum_Referee_Data_Robot_Ammo_Type Get_Shoot_Ammo_Type();

    inline Enum_Referee_Data_Robot_Booster_Type Get_Shoot_Booster_Type();

    inline uint8_t Get_Shoot_Frequency();

    inline float Get_Shoot_Speed();

    inline uint16_t Get_17mm_Remaining();

    inline uint16_t Get_42mm_Remaining();

    inline uint16_t Get_Money_Remaining();

    inline Enum_Referee_Data_Status Get_Base_RFID_Status();

    inline Enum_Referee_Data_Status Get_Highland_2_Self_RFID_Status();

    inline Enum_Referee_Data_Status Get_Highland_2_Enemy_RFID_Status();

    inline Enum_Referee_Data_Status Get_Highland_3_Self_RFID_Status();

    inline Enum_Referee_Data_Status Get_Highland_3_Enemy_RFID_Status();

    inline Enum_Referee_Data_Status Get_Highland_4_Self_RFID_Status();

    inline Enum_Referee_Data_Status Get_Highland_4_Enemy_RFID_Status();

    inline Enum_Referee_Data_Status Get_Energy_RFID_Status();

    inline Enum_Referee_Data_Status Get_Flyover_1_Self_RFID_Status();

    inline Enum_Referee_Data_Status Get_Flyover_2_Self_RFID_Status();

    inline Enum_Referee_Data_Status Get_Flyover_1_Enemy_RFID_Status();

    inline Enum_Referee_Data_Status Get_Flyover_2_Enemy_RFID_Status();

    inline Enum_Referee_Data_Status Get_Outpost_RFID_Status();

    inline Enum_Referee_Data_Status Get_HP_RFID_Status();

    inline Enum_Referee_Data_Status Get_Sentry_Self_RFID_Status();

    inline Enum_Referee_Data_Status Get_Sentry_Enemy_RFID_Status();

    inline Enum_Referee_Data_Status Get_Engineer_Self_RFID_Status();

    inline Enum_Referee_Data_Status Get_Engineer_Enemy_RFID_Status();

    inline Enum_Referee_Data_Status Get_Engineer_Exchange_RFID_Status();

    inline Enum_Referee_Data_Status Get_Middle_RFID_Status();

    inline Enum_Referee_Data_Robot_Dart_Command_Status Get_Dart_Command_Status();

    inline uint16_t Get_Dart_Switch_Remaining_Time();

    inline uint16_t Get_Dart_Launch_Remaining_Time();

    inline float Get_Sentry_Location_Hero_1_X();

    inline float Get_Sentry_Location_Hero_1_Y();

    inline float Get_Sentry_Location_Engineer_2_X();

    inline float Get_Sentry_Location_Engineer_2_Y();

    inline float Get_Sentry_Location_Infantry_3_X();

    inline float Get_Sentry_Location_Infantry_3_Y();

    inline float Get_Sentry_Location_Infantry_4_X();

    inline float Get_Sentry_Location_Infantry_4_Y();

    inline float Get_Sentry_Location_Infantry_5_X();

    inline float Get_Sentry_Location_Infantry_5_Y();

    inline uint8_t Get_Radar_Mark_Status_Hero_1();

    inline uint8_t Get_Radar_Mark_Status_Engineer_2();

    inline uint8_t Get_Radar_Mark_Status_Infantry_3();

    inline uint8_t Get_Radar_Mark_Status_Infantry_4();

    inline uint8_t Get_Radar_Mark_Status_Infantry_5();

    inline uint8_t Get_Radar_Mark_Status_Sentry_7();

    inline uint16_t Get_Sentry_Decision_Ammo_Exchange_Number();

    inline uint8_t Get_Sentry_Decision_Ammo_Exchange_Time();

    inline uint8_t Get_Sentry_Decision_HP_Exchange_Time();

    inline uint8_t Get_Radar_Decision_Double_Damage_Chance();

    inline Enum_Referee_Data_Status Get_Radar_Decision_Double_Damage_Enemy_Status();

    inline void Set_Referee_Trust_Status(Enum_Referee_Data_Status __Referee_Trust_Status);

    inline void Set_Referee_UI_Change_Flag_Clear();

    inline Struct_Referee_Data_Interaction_Graphic_Config *Set_Referee_UI_Clear(uint8_t Layer_Num, uint8_t Graphic_Num);

    inline Struct_Referee_Data_Interaction_Graphic_Config *Set_Referee_UI_Line(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t End_X, uint32_t End_Y);

    inline Struct_Referee_Data_Interaction_Graphic_Config *Set_Referee_UI_Rectangle(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t End_X, uint32_t End_Y);

    inline Struct_Referee_Data_Interaction_Graphic_Config *Set_Referee_UI_Circle(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Radius);

    inline Struct_Referee_Data_Interaction_Graphic_Config *Set_Referee_UI_Oval(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Length_X, uint32_t Length_Y);

    inline Struct_Referee_Data_Interaction_Graphic_Config *Set_Referee_UI_Arc(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Angle_Start, uint32_t Angle_End, uint32_t Length_X, uint32_t Length_Y);

    inline Struct_Referee_Data_Interaction_Graphic_Config *Set_Referee_UI_Float(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, float Float);

    inline Struct_Referee_Data_Interaction_Graphic_Config *Set_Referee_UI_Integer(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, int32_t Integer);

    inline Struct_Referee_Data_Interaction_Graphic_Config *Set_Referee_UI_String(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, uint32_t String_Length);

    void UART_Send_Interaction_UI_Layer_Delete(Enum_Referee_Data_Interaction_Layer_Delete_Operation Layer_Delete_Operation, uint8_t Layer);

    void UART_Send_Interaction_UI_Graphic_1(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1);

    void UART_Send_Interaction_UI_Graphic_2(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2);

    void UART_Send_Interaction_UI_Graphic_5(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_3, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_4, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_5);

    void UART_Send_Interaction_UI_Graphic_7(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_3, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_4, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_5, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_6, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_7);

    void UART_Send_Interaction_UI_Graphic_String(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_String, const char *String_Content);

    void UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length);

    void TIM_1000ms_Alive_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;
    // 数据包头标
    uint8_t Frame_Header;

    // 常量

    // 内部变量

    // 当前时刻的裁判系统接收flag
    uint32_t Flag = 0;
    // 前一时刻的裁判系统接收flag
    uint32_t Pre_Flag = 0;

    // 发送序列号
    uint8_t Sequence = 0;

    // UI是否是初次绘制, 没绘制过是0
    uint8_t UI_Change_Flag[10][10] = {0};

    // 读变量

    // 裁判系统状态
    Enum_Referee_Status Referee_Status = Referee_Status_DISABLE;
    // 比赛状态
    Struct_Referee_Rx_Data_Game_Status Game_Status;
    // 比赛结果
    Struct_Referee_Rx_Data_Game_Result Game_Result;
    // 机器人血量
    Struct_Referee_Rx_Data_Game_Robot_HP Game_Robot_HP;

    // 场地事件
    Struct_Referee_Rx_Data_Event_Self_Data Event_Self_Data;
    // 补给站状态
    Struct_Referee_Rx_Data_Event_Self_Supply Event_Self_Supply;
    // 裁判警告信息
    Struct_Referee_Rx_Data_Event_Referee_Warning Event_Referee_Warning;
    // 飞镖15s倒计时
    Struct_Referee_Rx_Data_Event_Dart_Status Event_Dart_Status;

    // 机器人状态
    Struct_Referee_Rx_Data_Robot_Status Robot_Status;
    // 当前机器人实时功率热量
    Struct_Referee_Rx_Data_Robot_Power_Heat Robot_Power_Heat;
    // 当前机器人实时位置
    Struct_Referee_Rx_Data_Robot_Position Robot_Position;
    // 当前机器人增益
    Struct_Referee_Rx_Data_Robot_Buff Robot_Buff;
    // 无人机可攻击时间
    Struct_Referee_Rx_Data_Robot_Aerial_Status Robot_Aerial_Status;
    // 伤害情况
    Struct_Referee_Rx_Data_Robot_Damage Robot_Damage;
    // 子弹信息
    Struct_Referee_Rx_Data_Robot_Booster Robot_Booster;
    // 子弹剩余信息
    Struct_Referee_Rx_Data_Robot_Remaining_Ammo Robot_Remaining_Ammo;
    // RFID状态信息
    Struct_Referee_Rx_Data_Robot_RFID Robot_RFID;
    // 飞镖状态
    Struct_Referee_Rx_Data_Robot_Dart_Command Robot_Dart_Command;
    // 哨兵获取己方位置信息
    Struct_Referee_Rx_Data_Robot_Sentry_Location Robot_Sentry_Location;
    // 雷达标记进度
    Struct_Referee_Rx_Data_Robot_Radar_Mark Robot_Radar_Mark;
    // 哨兵决策信息
    Struct_Referee_Rx_Data_Robot_Sentry_Decision Robot_Sentry_Decision;
    // 雷达决策信息
    Struct_Referee_Rx_Data_Robot_Radar_Decision Robot_Radar_Decision;

    // 写变量

    Struct_Referee_Data_Interaction_Graphic_Config Graphic_Config[10][10];

    // 读写变量

    // 裁判系统是否可信
    Enum_Referee_Data_Status Referee_Trust_Status = Referee_Data_Status_ENABLE;

    // 内部函数

    uint8_t Verify_CRC_8(uint8_t *Message, uint32_t Length);

    uint16_t Verify_CRC_16(uint8_t *Message, uint32_t Length);

    void Data_Process(uint16_t Length);
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取裁判系统状态
 *
 * @return Enum_Referee_Status 裁判系统状态
 */
inline Enum_Referee_Status Class_Referee::Get_Status()
{
    return (Referee_Status);
}

/**
 * @brief 获取裁判系统可信状态
 *
 * @return Enum_Referee_Data_Status 裁判系统可信状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Referee_Trust_Status()
{
    return (Referee_Trust_Status);
}

/**
 * @brief 获取比赛类型
 *
 * @return Enum_Referee_Game_Status_Type 比赛类型
 */
inline Enum_Referee_Game_Status_Type Class_Referee::Get_Game_Type()
{
    return (static_cast<Enum_Referee_Game_Status_Type>(Game_Status.Type_Enum));
}

/**
 * @brief 获取比赛阶段
 *
 * @return Enum_Referee_Game_Status_Stage 比赛阶段
 */
inline Enum_Referee_Game_Status_Stage Class_Referee::Get_Game_Stage()
{
    return (static_cast<Enum_Referee_Game_Status_Stage>(Game_Status.Stage_Enum));
}

/**
 * @brief 获取当前阶段剩余时间
 *
 * @return uint16_t 当前阶段剩余时间
 */
inline uint16_t Class_Referee::Get_Remaining_Time()
{
    return (Game_Status.Remaining_Time);
}

/**
 * @brief 获取系统时间戳
 *
 * @return uint64_t 系统时间戳
 */
inline uint64_t Class_Referee::Get_Timestamp()
{
    return (Game_Status.Timestamp);
}

/**
 * @brief 获取比赛结果
 *
 * @return Enum_Referee_Game_Result 比赛结果
 */
inline Enum_Referee_Game_Result Class_Referee::Get_Result()
{
    return (Game_Result.Result);
}

/**
 * @brief 获取机器人血量
 *
 * @param Robots_ID 通用双方机器人ID
 * @return uint16_t 机器人血量
 */
inline uint16_t Class_Referee::Get_HP(Enum_Referee_Data_Robots_ID Robots_ID)
{
    switch (Robots_ID)
    {
    case (Referee_Data_Robots_ID_RED_HERO_1):
    {
        return (Game_Robot_HP.Red_Hero_1);
    }
        break;
    case (Referee_Data_Robots_ID_RED_ENGINEER_2):
    {
        return (Game_Robot_HP.Red_Engineer_2);
    }
        break;
    case (Referee_Data_Robots_ID_RED_INFANTRY_3):
    {
        return (Game_Robot_HP.Red_Infantry_3);
    }
        break;
    case (Referee_Data_Robots_ID_RED_INFANTRY_4):
    {
        return (Game_Robot_HP.Red_Infantry_4);
    }
        break;
    case (Referee_Data_Robots_ID_RED_INFANTRY_5):
    {
        return (Game_Robot_HP.Red_Infantry_5);
    }
        break;
    case (Referee_Data_Robots_ID_RED_SENTRY_7):
    {
        return (Game_Robot_HP.Red_Sentry_7);
    }
        break;
    case (Referee_Data_Robots_ID_RED_OUTPOST_11):
    {
        return (Game_Robot_HP.Red_Outpost_11);
    }
        break;
    case (Referee_Data_Robots_ID_RED_BASE_10):
    {
        return (Game_Robot_HP.Red_Base_10);
    }
        break;
    case (Referee_Data_Robots_ID_BLUE_HERO_1):
    {
        return (Game_Robot_HP.Blue_Hero_1);
    }
        break;
    case (Referee_Data_Robots_ID_BLUE_ENGINEER_2):
    {
        return (Game_Robot_HP.Blue_Engineer_2);
    }
        break;
    case (Referee_Data_Robots_ID_BLUE_INFANTRY_3):
    {
        return (Game_Robot_HP.Blue_Infantry_3);
    }
        break;
    case (Referee_Data_Robots_ID_BLUE_INFANTRY_4):
    {
        return (Game_Robot_HP.Blue_Infantry_4);
    }
        break;
    case (Referee_Data_Robots_ID_BLUE_INFANTRY_5):
    {
        return (Game_Robot_HP.Blue_Infantry_5);
    }
        break;
    case (Referee_Data_Robots_ID_BLUE_SENTRY_7):
    {
        return (Game_Robot_HP.Blue_Sentry_7);
    }
        break;
    case (Referee_Data_Robots_ID_BLUE_OUTPOST_11):
    {
        return (Game_Robot_HP.Blue_Outpost_11);
    }
        break;
    case (Referee_Data_Robots_ID_BLUE_BASE_10):
    {
        return (Game_Robot_HP.Blue_Base_10);
    }
        break;
    }
}

/**
 * @brief 获取补给站前占领状态
 *
 * @return Enum_Referee_Data_Status 补给站前占领状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Supply_Front_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Event_Self_Data.Supply_Front_Status_Enum));
}

/**
 * @brief 获取补给站内占领状态
 *
 * @return Enum_Referee_Data_Status 补给站内占领状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Supply_Inner_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Event_Self_Data.Supply_Inner_Status_Enum));
}

/**
 * @brief 获取补给站占领状态
 *
 * @return Enum_Referee_Data_Status 补给站占领状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Supply_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Event_Self_Data.Supply_Status_Enum));
}

/**
 * @brief 获取能量机关占领状态
 *
 * @return Enum_Referee_Data_Status 能量机关占领状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Power_Rune_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Event_Self_Data.Power_Rune_Status_Enum));
}

/**
 * @brief 获取小能量机关激活状态
 *
 * @return Enum_Referee_Data_Status 小能量机关激活状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Power_Rune_Small_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Event_Self_Data.Power_Rune_Small_Status_Enum));
}

/**
 * @brief 获取大能量机关激活状态
 *
 * @return Enum_Referee_Data_Status 大能量机关激活状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Power_Rune_Big_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Event_Self_Data.Power_Rune_Big_Status_Enum));
}

/**
 * @brief 获取环形高地占领状态
 *
 * @return Enum_Referee_Data_Status 环形高地占领状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Highland_2_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Event_Self_Data.Highland_2_Status_Enum));
}

/**
 * @brief 获取梯形高地3占领状态
 *
 * @return Enum_Referee_Data_Status 梯形高地3占领状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Highland_3_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Event_Self_Data.Highland_3_Status_Enum));
}

/**
 * @brief 获取梯形高地4占领状态
 *
 * @return Enum_Referee_Data_Status 梯形高地4占领状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Highland_4_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Event_Self_Data.Highland_4_Status_Enum));
}

/**
 * @brief 获取基地护盾状态
 *
 * @return uint8_t 基地护盾状态
 */
inline uint8_t Class_Referee::Get_Base_Virtual_Shield_Percent()
{
    return (Event_Self_Data.Base_Virtual_Shield_Percent);
}

/**
 * @brief 获取敌方飞镖最后一次命中己方时间
 *
 * @return uint16_t 敌方飞镖最后一次命中己方时间
 */
inline uint16_t Class_Referee::Get_Enemy_Dart_Hit_Time()
{
    return (Event_Self_Data.Enemy_Dart_Hit_Time);
}

/**
 * @brief 获取敌方飞镖最后一次命中己方建筑物
 *
 * @return Enum_Referee_Dart_Hit_Target 敌方飞镖最后一次命中己方建筑物
 */
inline Enum_Referee_Dart_Hit_Target Class_Referee::Get_Enemy_Dart_Hit_Target_Enum()
{
    return (static_cast<Enum_Referee_Dart_Hit_Target>(Event_Self_Data.Enemy_Dart_Hit_Target_Enum));
}

/**
 * @brief 获取中心增益点占领状态
 *
 * @return Enum_Referee_Data_Status 中心增益点占领状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Middle_Buff_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Event_Self_Data.Middle_Buff_Status_Enum));
}

/**
 * @brief 获取请求补给的机器人ID
 *
 * @return Enum_Referee_Data_Robots_ID 请求补给的机器人ID
 */
inline Enum_Referee_Data_Robots_ID Class_Referee::Get_Supply_Request_Robot()
{
    return (Event_Self_Supply.Robot);
}

/**
 * @brief 获取补给站的补给状态
 *
 * @return Enum_Referee_Data_Event_Supply_Status 补给站的补给状态
 */
inline Enum_Referee_Data_Event_Supply_Status Class_Referee::Get_Supply_Request_Status()
{
    return (Event_Self_Supply.Status);
}

/**
 * @brief 获取补给子弹数量
 *
 * @return Enum_Referee_Data_Event_Supply_Ammo_Number 补给子弹数量
 */
inline Enum_Referee_Data_Event_Supply_Ammo_Number Class_Referee::Get_Supply_Ammo_Number()
{
    return (Event_Self_Supply.Ammo_Number);
}

/**
 * @brief 获取裁判判罚信息
 *
 * @return Enum_Referee_Data_Event_Referee_Warning_Level 裁判判罚信息
 */
inline Enum_Referee_Data_Event_Referee_Warning_Level Class_Referee::Get_Referee_Warning()
{
    return (Event_Referee_Warning.Level);
}

/**
 * @brief 获取裁判判罚机器人
 *
 * @return Enum_Referee_Data_Robot_ID 裁判判罚机器人
 */
inline Enum_Referee_Data_Robot_ID Class_Referee::Get_Referee_Warning_Robot()
{
    return (Event_Referee_Warning.Robot_ID);
}

/**
 * @brief 获取飞镖剩余时间
 *
 * @return uint8_t 飞镖剩余时间
 */
inline uint8_t Class_Referee::Get_Dart_Remaining_Time()
{
    return (Event_Dart_Status.Dart_Remaining_Time);
}

/**
 * @brief 获取飞镖上一次击中的目标
 *
 * @return Enum_Referee_Dart_Hit_Target 飞镖上一次击中的目标
 */
inline Enum_Referee_Dart_Hit_Target Class_Referee::Get_Last_Dart_Hit_Target()
{
    return (static_cast<Enum_Referee_Dart_Hit_Target>(Event_Dart_Status.Dart_Hit_Target_Enum_Last));
}

/**
 * @brief 获取飞镖上一次击中目标的次数
 *
 * @return uint8_t 飞镖上一次击中目标的次数
 */
inline uint8_t Class_Referee::Get_Dart_Hit_Target_Count()
{
    return (Event_Dart_Status.Dart_Hit_Target_Count);
}

/**
 * @brief 获取飞镖当前的目标
 *
 * @return Enum_Referee_Dart_Hit_Target 飞镖当前的目标
 */
inline Enum_Referee_Dart_Hit_Target Class_Referee::Get_Now_Dart_Hit_Target()
{
    return (static_cast<Enum_Referee_Dart_Hit_Target>(Event_Dart_Status.Dart_Hit_Target_Enum_Now));
}

/**
 * @brief 获取自身ID
 *
 * @return Enum_Referee_Data_Robots_ID 自身ID
 */
inline Enum_Referee_Data_Robots_ID Class_Referee::Get_Self_ID()
{
    return (Robot_Status.Robot_ID);
}

/**
 * @brief 获取自身等级
 *
 * @return uint8_t 自身等级
 */
inline uint8_t Class_Referee::Get_Self_Level()
{
    return (Robot_Status.Level);
}

/**
 * @brief 获取自身血量
 *
 * @return uint16_t 自身血量
 */
inline uint16_t Class_Referee::Get_Self_HP()
{
    return (Robot_Status.HP);
}

/**
 * @brief 获取自身血量上限
 *
 * @return uint16_t 自身血量上限
 */
inline uint16_t Class_Referee::Get_Self_HP_Max()
{
    return (Robot_Status.HP_Max);
}

/**
 * @brief 获取发射机构冷却
 *
 * @return uint16_t 发射机构冷却
 */
inline uint16_t Class_Referee::Get_Self_Booster_Heat_CD()
{
    return (Robot_Status.Booster_Heat_CD);
}

/**
 * @brief 获取发射机构热量上限
 *
 * @return uint16_t 发射机构热量上限
 */
inline uint16_t Class_Referee::Get_Self_Booster_Heat_Max()
{
    return (Robot_Status.Booster_Heat_Max);
}

/**
 * @brief 获取底盘功率上限
 *
 * @return uint16_t 底盘功率上限
 */
inline uint16_t Class_Referee::Get_Self_Chassis_Power_Max()
{
    return (Robot_Status.Chassis_Power_Max);
}

/**
 * @brief 获取Gimbal供电状态
 *
 * @return Enum_Referee_Data_Status Gimbal供电状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Self_PM01_Gimbal_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_Status.PM01_Gimbal_Status_Enum));
}

/**
 * @brief 获取Chassis供电状态
 *
 * @return Enum_Referee_Data_Status Chassis供电状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Self_PM01_Chassis_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_Status.PM01_Chassis_Status_Enum));
}

/**
 * @brief 获取Booster供电状态
 *
 * @return Enum_Referee_Data_Status Booster供电状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Self_PM01_Booster_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_Status.PM01_Booster_Status_Enum));
}

/**
 * @brief 获取底盘电压
 *
 * @return float 底盘电压
 */
inline float Class_Referee::Get_Chassis_Voltage()
{
    return (Robot_Power_Heat.Chassis_Voltage / 1000.0f);
}

/**
 * @brief 获取底盘电流
 *
 * @return float 底盘电流
 */
inline float Class_Referee::Get_Chassis_Current()
{
    return (Robot_Power_Heat.Chassis_Current / 1000.0f);
}

/**
 * @brief 获取底盘功率
 *
 * @return float 底盘功率
 */
inline float Class_Referee::Get_Chassis_Power()
{
    return (Robot_Power_Heat.Chassis_Power);
}

/**
 * @brief 获取底盘能量缓冲
 *
 * @return uint16_t 底盘能量缓冲
 */
inline uint16_t Class_Referee::Get_Chassis_Energy_Buffer()
{
    return (Robot_Power_Heat.Chassis_Energy_Buffer);
}

/**
 * @brief 获取17mm1热量
 *
 * @return uint16_t 17mm1热量
 */
inline uint16_t Class_Referee::Get_Booster_17mm_1_Heat()
{
    return (Robot_Power_Heat.Booster_17mm_1_Heat);
}

/**
 * @brief 获取17mm2热量
 *
 * @return uint16_t 17mm2热量
 */
inline uint16_t Class_Referee::Get_Booster_17mm_2_Heat()
{
    return (Robot_Power_Heat.Booster_17mm_2_Heat);
}

/**
 * @brief 获取42mm热量
 *
 * @return uint16_t 42mm热量
 */
inline uint16_t Class_Referee::Get_Booster_42mm_Heat()
{
    return (Robot_Power_Heat.Booster_42mm_Heat);
}

/**
 * @brief 获取自身位置x
 *
 * @return float 自身位置x
 */
inline float Class_Referee::Get_Location_X()
{
    return (Robot_Position.Location_X);
}

/**
 * @brief 获取自身位置y
 *
 * @return float 自身位置y
 */
inline float Class_Referee::Get_Location_Y()
{
    return (Robot_Position.Location_Y);
}

/**
 * @brief 获取自身方向yaw
 *
 * @return float 自身方向yaw
 */
inline float Class_Referee::Get_Location_Yaw()
{
    return (Robot_Position.Location_Yaw);
}

/**
 * @brief 获取补血buff百分比
 *
 * @return uint8_t 补血buff百分比
 */
inline uint8_t Class_Referee::Get_HP_Buff_Percent()
{
    return (Robot_Buff.HP_Buff_Percent);
}

/**
 * @brief 获取冷却缩减buff倍数
 *
 * @return uint8_t 冷却缩减buff倍数
 */
inline uint8_t Class_Referee::Get_Booster_Heat_CD_Buff_Value()
{
    return (Robot_Buff.Booster_Heat_CD_Buff_Value);
}

/**
 * @brief 获取防御加成buff百分比
 *
 * @return uint8_t 防御加成buff百分比
 */
inline uint8_t Class_Referee::Get_Defend_Buff_Percent()
{
    return (Robot_Buff.Defend_Buff_Percent);
}

/**
 * @brief 获取负防御加成buff百分比
 *
 * @return uint8_t 负防御加成buff百分比
 */
inline uint8_t Class_Referee::Get_Defend_Debuff_Percent()
{
    return (Robot_Buff.Defend_Buff_Percent);
}

/**
 * @brief 获取攻击加成buff状态
 *
 * @return uint8_t 攻击加成buff状态
 */
inline uint8_t Class_Referee::Get_Damage_Buff_Percent()
{
    return (Robot_Buff.Damage_Buff_Percent);
}

/**
 * @brief 获取无人机状态
 *
 * @return Enum_Referee_Data_Event_Aerial_Status 无人机状态
 */
inline Enum_Referee_Data_Event_Aerial_Status Class_Referee::Get_Aerial_Status()
{
    return (Robot_Aerial_Status.Aerial_Status);
}

/**
 * @brief 获取无人机时间
 *
 * @return uint8_t 无人机时间
 */
inline uint8_t Class_Referee::Get_Aerial_Remaining_Time()
{
    return (Robot_Aerial_Status.Remaining_Time);
}

/**
 * @brief 获取受击装甲板ID
 *
 * @return uint8_t 受击装甲板ID
 */
inline uint8_t Class_Referee::Get_Armor_Attacked_ID()
{
    return (Robot_Damage.Armor_ID);
}

/**
 * @brief 获取受击类型
 *
 * @return Enum_Referee_Data_Event_Robot_Damage_Type 受击类型
 */
inline Enum_Referee_Data_Event_Robot_Damage_Type Class_Referee::Get_Attacked_Type()
{
    return (static_cast<Enum_Referee_Data_Event_Robot_Damage_Type>(Robot_Damage.Type_Enum));
}

/**
 * @brief 获取射击子弹类型
 *
 * @return Enum_Referee_Data_Robot_Ammo_Type 射击子弹类型
 */
inline Enum_Referee_Data_Robot_Ammo_Type Class_Referee::Get_Shoot_Ammo_Type()
{
    return (Robot_Booster.Ammo_Type);
}

/**
 * @brief 获取发射机构类型
 *
 * @return Enum_Referee_Data_Robot_Booster_Type 发射机构类型
 */
inline Enum_Referee_Data_Robot_Booster_Type Class_Referee::Get_Shoot_Booster_Type()
{
    return (Robot_Booster.Booster_Type);
}

/**
 * @brief 获取射频, Hz
 *
 * @return uint8_t 射频, Hz
 */
inline uint8_t Class_Referee::Get_Shoot_Frequency()
{
    return (Robot_Booster.Frequency);
}

/**
 * @brief 获取射速
 *
 * @return float 射速
 */
inline float Class_Referee::Get_Shoot_Speed()
{
    return (Robot_Booster.Speed);
}

/**
 * @brief 获取17mm弹丸剩余数
 *
 * @return uint16_t 17mm弹丸剩余数
 */
inline uint16_t Class_Referee::Get_17mm_Remaining()
{
    return (Robot_Remaining_Ammo.Booster_17mm);
}

/**
 * @brief 获取42mm弹丸剩余数
 *
 * @return uint16_t 42mm弹丸剩余数
 */
inline uint16_t Class_Referee::Get_42mm_Remaining()
{
    return (Robot_Remaining_Ammo.Booster_42mm);
}

/**
 * @brief 获取金币剩余数
 *
 * @return uint16_t 金币剩余数
 */
inline uint16_t Class_Referee::Get_Money_Remaining()
{
    return (Robot_Remaining_Ammo.Money);
}

/**
 * @brief 获取基地增益RFID状态
 *
 * @return Enum_Referee_Data_Status 基地增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Base_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Base_Status_Enum));
}

/**
 * @brief 获取己方环形高地RFID状态
 *
 * @return Enum_Referee_Data_Status 己方环形高地RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Highland_2_Self_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Highland_2_Self_Status_Enum));
}

/**
 * @brief 获取敌方环形高地RFID状态
 *
 * @return Enum_Referee_Data_Status 敌方环形高地RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Highland_2_Enemy_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Highland_2_Enemy_Status_Enum));
}

/**
 * @brief 获取己方高地3RFID状态
 *
 * @return Enum_Referee_Data_Status 己方环形高地3RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Highland_3_Self_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Highland_3_Self_Status_Enum));
}

/**
 * @brief 获取敌方高地3RFID状态
 *
 * @return Enum_Referee_Data_Status 敌方高地3RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Highland_3_Enemy_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Highland_3_Enemy_Status_Enum));
}

/**
 * @brief 获取己方高地4RFID状态
 *
 * @return Enum_Referee_Data_Status 己方环形高地4RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Highland_4_Self_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Highland_4_Self_Status_Enum));
}

/**
 * @brief 获取敌方高地4RFID状态
 *
 * @return Enum_Referee_Data_Status 敌方高地4RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Highland_4_Enemy_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Highland_4_Enemy_Status_Enum));
}

/**
 * @brief 获取能量机关增益RFID状态
 *
 * @return Enum_Referee_Data_Status 能量机关增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Energy_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Energy_Status_Enum));
}

/**
 * @brief 获取己方1阶段飞坡增益RFID状态
 *
 * @return Enum_Referee_Data_Status 己方1阶段飞坡增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Flyover_1_Self_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Flyover_1_Self_Status_Enum));
}

/**
 * @brief 获取己方2阶段飞坡增益RFID状态
 *
 * @return Enum_Referee_Data_Status 己方2阶段飞坡增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Flyover_2_Self_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Flyover_2_Self_Status_Enum));
}

/**
 * @brief 获取敌方1阶段飞坡增益RFID状态
 *
 * @return Enum_Referee_Data_Status 敌方1阶段飞坡增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Flyover_1_Enemy_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Flyover_1_Enemy_Status_Enum));
}

/**
 * @brief 获取敌方2阶段飞坡增益RFID状态
 *
 * @return Enum_Referee_Data_Status 敌方2阶段飞坡增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Flyover_2_Enemy_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Flyover_2_Enemy_Status_Enum));
}

/**
 * @brief 获取前哨站增益RFID状态
 *
 * @return Enum_Referee_Data_Status 前哨站增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Outpost_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Outpost_Status_Enum));
}

/**
 * @brief 获取补血点增益RFID状态
 *
 * @return Enum_Referee_Data_Status 补血点增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_HP_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.HP_Status_Enum));
}

/**
 * @brief 获取己方哨兵巡逻区增益RFID状态
 *
 * @return Enum_Referee_Data_Status 己方哨兵巡逻区增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Sentry_Self_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Sentry_Self_Status_Enum));
}

/**
 * @brief 获取敌方哨兵巡逻区增益RFID状态
 *
 * @return Enum_Referee_Data_Status 敌方哨兵巡逻区增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Sentry_Enemy_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Sentry_Enemy_Status_Enum));
}

/**
 * @brief 获取己方工程采矿区增益RFID状态
 *
 * @return Enum_Referee_Data_Status 己方工程采矿区增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Engineer_Self_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Engineer_Self_Status_Enum));
}

/**
 * @brief 获取敌方工程采矿区增益RFID状态
 *
 * @return Enum_Referee_Data_Status 敌方工程采矿区增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Engineer_Enemy_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Engineer_Enemy_Status_Enum));
}

/**
 * @brief 获取工程兑换站增益RFID状态
 *
 * @return Enum_Referee_Data_Status 工程兑换站增益RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Engineer_Exchange_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Engineer_Exchange_Status_Enum));
}

/**
 * @brief 获取中心增益点RFID状态
 *
 * @return Enum_Referee_Data_Status 中心增益点RFID状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Middle_RFID_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_RFID.Middle_Status_Enum));
}

/**
 * @brief 获取飞镖发射口状态
 *
 * @return Enum_Referee_Data_Robot_Dart_Command_Status 飞镖发射口状态
 */
inline Enum_Referee_Data_Robot_Dart_Command_Status Class_Referee::Get_Dart_Command_Status()
{
    return (Robot_Dart_Command.Status);
}

/**
 * @brief 获取飞镖切换击打目标时的比赛剩余时间
 *
 * @return uint16_t 飞镖切换击打目标时的比赛剩余时间
 */
inline uint16_t Class_Referee::Get_Dart_Switch_Remaining_Time()
{
    return (Robot_Dart_Command.Switch_Remaining_Time);
}

/**
 * @brief 获取最后一次操作手下达发射指令时的比赛剩余时间
 *
 * @return uint16_t 最后一次操作手下达发射指令时的比赛剩余时间
 */
inline uint16_t Class_Referee::Get_Dart_Launch_Remaining_Time()
{
    return (Robot_Dart_Command.Launch_Remaining_Time);
}

/**
 * @brief 获取己方英雄坐标x
 *
 * @return float 己方英雄坐标x
 */
inline float Class_Referee::Get_Sentry_Location_Hero_1_X()
{
    return (Robot_Sentry_Location.Hero_1_X);
}

/**
 * @brief 获取己方英雄坐标y
 *
 * @return float 己方英雄坐标y
 */
inline float Class_Referee::Get_Sentry_Location_Hero_1_Y()
{
    return (Robot_Sentry_Location.Hero_1_Y);
}

/**
 * @brief 获取己方工程坐标x
 *
 * @return float 己方工程坐标x
 */
inline float Class_Referee::Get_Sentry_Location_Engineer_2_X()
{
    return (Robot_Sentry_Location.Engineer_2_X);
}

/**
 * @brief 获取己方工程坐标y
 *
 * @return float 己方工程坐标y
 */
inline float Class_Referee::Get_Sentry_Location_Engineer_2_Y()
{
    return (Robot_Sentry_Location.Engineer_2_Y);
}

/**
 * @brief 获取己方步兵坐标x
 *
 * @return float 己方步兵坐标x
 */
inline float Class_Referee::Get_Sentry_Location_Infantry_3_X()
{
    return (Robot_Sentry_Location.Infantry_3_X);
}

/**
 * @brief 获取己方步兵坐标y
 *
 * @return float 己方步兵坐标y
 */
inline float Class_Referee::Get_Sentry_Location_Infantry_3_Y()
{
    return (Robot_Sentry_Location.Infantry_3_Y);
}

/**
 * @brief 获取己方步兵坐标x
 *
 * @return float 己方步兵坐标x
 */
inline float Class_Referee::Get_Sentry_Location_Infantry_4_X()
{
    return (Robot_Sentry_Location.Infantry_4_X);
}

/**
 * @brief 获取己方步兵坐标y
 *
 * @return float 己方步兵坐标y
 */
inline float Class_Referee::Get_Sentry_Location_Infantry_4_Y()
{
    return (Robot_Sentry_Location.Infantry_4_Y);
}

/**
 * @brief 获取己方步兵坐标x
 *
 * @return float 己方步兵坐标x
 */
inline float Class_Referee::Get_Sentry_Location_Infantry_5_X()
{
    return (Robot_Sentry_Location.Infantry_5_X);
}

/**
 * @brief 获取己方步兵坐标y
 *
 * @return float 己方步兵坐标y
 */
inline float Class_Referee::Get_Sentry_Location_Infantry_5_Y()
{
    return (Robot_Sentry_Location.Infantry_5_Y);
}

/**
 * @brief 获取雷达对敌方英雄标记进度
 *
 * @return uint8_t 雷达对敌方英雄标记进度
 */
inline uint8_t Class_Referee::Get_Radar_Mark_Status_Hero_1()
{
    return (Robot_Radar_Mark.Hero_1_Mark);
}

/**
 * @brief 获取雷达对敌方工程标记进度
 *
 * @return uint8_t 雷达对敌方工程标记进度
 */
inline uint8_t Class_Referee::Get_Radar_Mark_Status_Engineer_2()
{
    return (Robot_Radar_Mark.Engineer_2_Mark);
}

/**
 * @brief 获取雷达对敌方步兵标记进度
 *
 * @return uint8_t 雷达对敌方步兵标记进度
 */
inline uint8_t Class_Referee::Get_Radar_Mark_Status_Infantry_3()
{
    return (Robot_Radar_Mark.Infantry_3_Mark);
}

/**
 * @brief 获取雷达对敌方步兵标记进度
 *
 * @return uint8_t 雷达对敌方步兵标记进度
 */
inline uint8_t Class_Referee::Get_Radar_Mark_Status_Infantry_4()
{
    return (Robot_Radar_Mark.Infantry_4_Mark);
}

/**
 * @brief 获取雷达对敌方步兵标记进度
 *
 * @return uint8_t 雷达对敌方步兵标记进度
 */
inline uint8_t Class_Referee::Get_Radar_Mark_Status_Infantry_5()
{
    return (Robot_Radar_Mark.Infantry_5_Mark);
}

/**
 * @brief 获取雷达对敌方哨兵标记进度
 *
 * @return uint8_t 雷达对敌方哨兵标记进度
 */
inline uint8_t Class_Referee::Get_Radar_Mark_Status_Sentry_7()
{
    return (Robot_Radar_Mark.Sentry_7_Mark);
}

/**
 * @brief 获取哨兵决策购买子弹数量
 *
 * @return uint16_t 哨兵决策购买子弹数量
 */
inline uint16_t Class_Referee::Get_Sentry_Decision_Ammo_Exchange_Number()
{
    return (Robot_Sentry_Decision.Ammo_Exchange_Number);
}

/**
 * @brief 获取哨兵决策购买子弹次数
 *
 * @return uint8_t 哨兵决策购买子弹次数
 */
inline uint8_t Class_Referee::Get_Sentry_Decision_Ammo_Exchange_Time()
{
    return (Robot_Sentry_Decision.Ammo_Exchange_Time);
}

/**
 * @brief 获取哨兵决策购买血量次数
 *
 * @return uint8_t 哨兵决策购买血量次数
 */
inline uint8_t Class_Referee::Get_Sentry_Decision_HP_Exchange_Time()
{
    return (Robot_Sentry_Decision.HP_Exchange_Time);
}

/**
 * @brief 获取雷达决策重伤敌方机会
 *
 * @return uint8_t 雷达决策重伤敌方机会
 */
inline uint8_t Class_Referee::Get_Radar_Decision_Double_Damage_Chance()
{
    return (Robot_Radar_Decision.Double_Damage_Chance);
}

/**
 * @brief 获取雷达决策重伤敌方状态
 *
 * @return Enum_Referee_Data_Status 雷达决策重伤敌方状态
 */
inline Enum_Referee_Data_Status Class_Referee::Get_Radar_Decision_Double_Damage_Enemy_Status()
{
    return (static_cast<Enum_Referee_Data_Status>(Robot_Radar_Decision.Double_Damage_Enemy_Status_Enum));
}

/**
 * @brief 设定裁判系统是否可信
 *
 * @return Enum_Referee_Data_Status 裁判系统是否可信
 */
inline void Class_Referee::Set_Referee_Trust_Status(Enum_Referee_Data_Status __Referee_Trust_Status)
{
    Referee_Trust_Status = __Referee_Trust_Status;
}

/**
 * @brief 设定裁判系统UI清除
 *
 */
inline void Class_Referee::Set_Referee_UI_Change_Flag_Clear()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            UI_Change_Flag[i][j] = 0;
        }
    }
}

/**
 * @brief 设定裁判系统UI清除
 *
 * @param Layer_Num 图层编号, 0~9
 * @param Graphic_Num 图形编号, 0~9
 */
inline Struct_Referee_Data_Interaction_Graphic_Config *Class_Referee::Set_Referee_UI_Clear(uint8_t Layer_Num, uint8_t Graphic_Num)
{
    Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
    Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;

    Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_DELETE;

    UI_Change_Flag[Layer_Num][Graphic_Num] = 0;

    return (&Graphic_Config[Layer_Num][Graphic_Num]);
}

/**
 * @brief 设定裁判系统UI直线
 *
 * @param Layer_Num 图层编号, 0~9
 * @param Graphic_Num 图形编号, 0~9
 * @param Color 图形颜色
 * @param Line_Width 线宽
 * @param Start_X 起点x
 * @param Start_Y 起点y
 * @param End_X 终点x
 * @param End_Y 终点y
 * @return 对应图层指针
 */
inline Struct_Referee_Data_Interaction_Graphic_Config *Class_Referee::Set_Referee_UI_Line(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t End_X, uint32_t End_Y)
{
    Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
    Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
        UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_LINE;
    Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Graphic_Config[Layer_Num][Graphic_Num].Start_X = Start_X;
    Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Start_Y;
    Graphic_Config[Layer_Num][Graphic_Num].Details_D = End_X;
    Graphic_Config[Layer_Num][Graphic_Num].Details_E = End_Y;

    return (&Graphic_Config[Layer_Num][Graphic_Num]);
}

/**
 * @brief 设定裁判系统UI矩形
 *
 * @param Layer_Num 图层编号, 0~9
 * @param Graphic_Num 图形编号, 0~9
 * @param Color 图形颜色
 * @param Line_Width 线宽
 * @param Start_X 起点x
 * @param Start_Y 起点y
 * @param End_X 终点x
 * @param End_Y 终点y
 * @return 对应图层指针
 */
inline Struct_Referee_Data_Interaction_Graphic_Config *Class_Referee::Set_Referee_UI_Rectangle(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t End_X, uint32_t End_Y)
{
    Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
    Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
        UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_RECTANGLE;
    Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Graphic_Config[Layer_Num][Graphic_Num].Start_X = Start_X;
    Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Start_Y;
    Graphic_Config[Layer_Num][Graphic_Num].Details_D = End_X;
    Graphic_Config[Layer_Num][Graphic_Num].Details_E = End_Y;

    return (&Graphic_Config[Layer_Num][Graphic_Num]);
}

/**
 * @brief 设定裁判系统UI圆形
 *
 * @param Layer_Num 图层编号, 0~9
 * @param Graphic_Num 图形编号, 0~9
 * @param Color 图形颜色
 * @param Line_Width 线宽
 * @param Center_X 圆心x
 * @param Center_Y 圆心y
 * @param Radius 半径
 * @return 对应图层指针
 */
inline Struct_Referee_Data_Interaction_Graphic_Config *Class_Referee::Set_Referee_UI_Circle(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Radius)
{
    Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
    Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
        UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_CIRCLE;
    Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Graphic_Config[Layer_Num][Graphic_Num].Start_X = Center_X;
    Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Center_Y;
    Graphic_Config[Layer_Num][Graphic_Num].Details_C = Radius;

    return (&Graphic_Config[Layer_Num][Graphic_Num]);
}

/**
 * @brief 设定裁判系统UI椭圆形
 *
 * @param Layer_Num 图层编号, 0~9
 * @param Graphic_Num 图形编号, 0~9
 * @param Color 图形颜色
 * @param Line_Width 线宽
 * @param Center_X 圆心x
 * @param Center_Y 圆心y
 * @param Length_X x半轴长度
 * @param Length_Y y半轴长度
 * @return 对应图层指针
 */
inline Struct_Referee_Data_Interaction_Graphic_Config *Class_Referee::Set_Referee_UI_Oval(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Length_X, uint32_t Length_Y)
{
    Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
    Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
        UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_OVAL;
    Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Graphic_Config[Layer_Num][Graphic_Num].Start_X = Center_X;
    Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Center_Y;
    Graphic_Config[Layer_Num][Graphic_Num].Details_D = Length_X;
    Graphic_Config[Layer_Num][Graphic_Num].Details_E = Length_Y;

    return (&Graphic_Config[Layer_Num][Graphic_Num]);
}

/**
 * @brief 设定裁判系统UI圆弧形
 *
 * @param Layer_Num 图层编号, 0~9
 * @param Graphic_Num 图形编号, 0~9
 * @param Color 图形颜色
 * @param Line_Width 线宽
 * @param Center_X 圆心x
 * @param Center_Y 圆心y
 * @param Angle_Start 起始角度
 * @param Angle_End 终止角度
 * @param Length_X x半轴长度
 * @param Length_Y y半轴长度
 * @return 对应图层指针
 */
inline Struct_Referee_Data_Interaction_Graphic_Config *Class_Referee::Set_Referee_UI_Arc(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Angle_Start, uint32_t Angle_End, uint32_t Length_X, uint32_t Length_Y)
{
    Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
    Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
        UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_ARC;
    Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Graphic_Config[Layer_Num][Graphic_Num].Start_X = Center_X;
    Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Center_Y;
    Graphic_Config[Layer_Num][Graphic_Num].Details_A = Angle_Start;
    Graphic_Config[Layer_Num][Graphic_Num].Details_B = Angle_End;
    Graphic_Config[Layer_Num][Graphic_Num].Details_D = Length_X;
    Graphic_Config[Layer_Num][Graphic_Num].Details_E = Length_Y;

    return (&Graphic_Config[Layer_Num][Graphic_Num]);
}

/**
 * @brief 设定裁判系统UI浮点数
 *
 * @param Layer_Num 图层编号, 0~9
 * @param Graphic_Num 图形编号, 0~9
 * @param Color 图形颜色
 * @param Line_Width 线宽
 * @param Start_X 起点x
 * @param Start_Y 起点y
 * @param Font_Width 字体大小
 * @param Float 数值
 * @return 对应图层指针
 */
inline Struct_Referee_Data_Interaction_Graphic_Config *Class_Referee::Set_Referee_UI_Float(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, float Float)
{
    Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
    Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
        UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_FLOAT;
    Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Graphic_Config[Layer_Num][Graphic_Num].Start_X = Start_X;
    Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Start_Y;
    Graphic_Config[Layer_Num][Graphic_Num].Details_A = Font_Width;
    int32_t *tmp_pointer = (int32_t *) ((uint32_t) &Graphic_Config[Layer_Num][Graphic_Num] + 11);
    *tmp_pointer = (int32_t) (Float * 1000.0f);

    return (&Graphic_Config[Layer_Num][Graphic_Num]);
}

/**
 * @brief 设定裁判系统UI整型数
 *
 * @param Layer_Num 图层编号, 0~9
 * @param Graphic_Num 图形编号, 0~9
 * @param Color 图形颜色
 * @param Line_Width 线宽
 * @param Start_X 起点x
 * @param Start_Y 起点y
 * @param Font_Width 字体大小
 * @param Integer 数值
 * @return 对应图层指针
 */
inline Struct_Referee_Data_Interaction_Graphic_Config *Class_Referee::Set_Referee_UI_Integer(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, int32_t Integer)
{
    Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
    Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
        UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_INTEGER;
    Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Graphic_Config[Layer_Num][Graphic_Num].Start_X = Start_X;
    Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Start_Y;
    Graphic_Config[Layer_Num][Graphic_Num].Details_A = Font_Width;
    int32_t *tmp_pointer = (int32_t *) ((uint32_t) &Graphic_Config[Layer_Num][Graphic_Num] + 11);
    *tmp_pointer = (int32_t) (Integer);

    return (&Graphic_Config[Layer_Num][Graphic_Num]);
}

/**
 * @brief 设定裁判系统UI字符串
 *
 * @param Layer_Num 图层编号, 0~9
 * @param Graphic_Num 图形编号, 0~9
 * @param Color 图形颜色
 * @param Line_Width 线宽
 * @param Start_X 起点x
 * @param Start_Y 起点y
 * @param Font_Width 字体大小
 * @param String_Length 字符串长度
 * @return
 */
inline Struct_Referee_Data_Interaction_Graphic_Config *Class_Referee::Set_Referee_UI_String(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, uint32_t String_Length)
{
    Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
    Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
        UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
        Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_STRING;
    Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Graphic_Config[Layer_Num][Graphic_Num].Start_X = Start_X;
    Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Start_Y;
    Graphic_Config[Layer_Num][Graphic_Num].Details_A = Font_Width;
    Graphic_Config[Layer_Num][Graphic_Num].Details_B = String_Length;

    return (&Graphic_Config[Layer_Num][Graphic_Num]);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
