/**
 * @file alg_fsm.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 有限自动机
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

#ifndef ALG_FSM_H
#define ALG_FSM_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/1_Driver/Math/drv_math.h"

/* Exported macros -----------------------------------------------------------*/

#define STATUS_MAX (10)

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 状态所处的阶段
 *
 */
enum Enum_Status_Stage
{
    Status_Stage_DISABLE = 0,
    Status_Stage_ENABLE,
};

/**
 * @brief 状态结构体
 *
 */
struct Struct_Status
{
    Enum_Status_Stage Status_Stage;
    uint32_t Count_Time;
};

/**
 * @brief Reusable, 有限自动机核心, 一般有时间需求的则采用有限自动机
 * 使用时请继承->声明友元后使用
 *
 */
class Class_FSM
{
public:
    Struct_Status Status[STATUS_MAX];

    void Init(uint8_t __Status_Number, uint8_t __Now_Status_Serial = 0);

    inline uint8_t Get_Now_Status_Serial();

    inline void Set_Status(uint8_t Next_Status_serial);

    void TIM_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 状态数量
    uint8_t Status_Number;

    // 常量

    // 内部变量

    // 读变量

    // FSM当前状态
    uint8_t Now_Status_Serial = 0;

    // 写变量

    // 读写变量

    // 内部函数
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取FSM当前状态
 *
 * @return uint8_t FSM当前状态
 */
inline uint8_t Class_FSM::Get_Now_Status_Serial()
{
    return (Now_Status_Serial);
}

/**
 * @brief 设置状态改变
 *
 * @param Next_Status_serial 下一个状态
 */
inline void Class_FSM::Set_Status(uint8_t Next_Status_serial)
{
    // 失能当前状态, 计数器清零
    Status[Now_Status_Serial].Status_Stage = Status_Stage_DISABLE;
    Status[Now_Status_Serial].Count_Time = 0;

    // 转到下一个状态
    Status[Next_Status_serial].Status_Stage = Status_Stage_ENABLE;
    Now_Status_Serial = Next_Status_serial;
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
