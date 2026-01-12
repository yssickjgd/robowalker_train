/**
 * @file alg_fsm.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 有限自动机
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2025-08-15 1.1 修改为模板类更易于适配
 *
 * @copyright USTC-RoboWalker (c) 2023-2025
 *
 */

#ifndef ALG_FSM_H
#define ALG_FSM_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Algorithm/Basic/alg_basic.h"

/* Exported macros -----------------------------------------------------------*/

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
template<uint8_t Status_Max = 10>
class Class_FSM
{
public:
    Struct_Status Status[Status_Max];

    void Init(const uint8_t &__Now_Status_Serial = 0);

    inline uint8_t Get_Now_Status_Serial() const;

    inline void Set_Status(const uint8_t &Next_Status_serial);

    void TIM_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

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
 * @brief 状态机初始化
 *
 * @tparam Status_Max 状态机最大状态数量
 * @param __Now_Status_Serial 当前指定状态机初始编号
 */
template<uint8_t Status_Max>
void Class_FSM<Status_Max>::Init(const uint8_t &__Now_Status_Serial)
{
    Now_Status_Serial = __Now_Status_Serial;

    // 所有状态全刷0
    for (int i = 0; i < Status_Max; i++)
    {
        Status[i].Status_Stage = Status_Stage_DISABLE;
        Status[i].Count_Time = 0;
    }

    // 使能初始状态
    Status[__Now_Status_Serial].Status_Stage = Status_Stage_ENABLE;
}

/**
 * @brief 定时器处理函数, 计算周期与模型有关
 * 这是一个模板, 使用时请根据不同处理情况在不同文件内重新定义
 *
 * @tparam Status_Max 状态机最大状态数量
 */
template<uint8_t Status_Max>
void Class_FSM<Status_Max>::TIM_Calculate_PeriodElapsedCallback()
{
    Status[Now_Status_Serial].Count_Time++;

    // 自己接着编写状态转移函数
}

/**
 * @brief 获取FSM当前状态
 *
 * @tparam Status_Max 状态机最大状态数量
 * @return uint8_t FSM当前状态
 */
template<uint8_t Status_Max>
inline uint8_t Class_FSM<Status_Max>::Get_Now_Status_Serial() const
{
    return (Now_Status_Serial);
}

/**
 * @brief 设置状态改变
 *
 * @tparam Status_Max 状态机最大状态数量
 * @param Next_Status_serial 下一个状态
 */
template<uint8_t Status_Max>
inline void Class_FSM<Status_Max>::Set_Status(const uint8_t &Next_Status_serial)
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