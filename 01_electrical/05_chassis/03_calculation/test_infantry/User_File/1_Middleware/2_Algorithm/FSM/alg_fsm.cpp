/**
 * @file alg_fsm.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 有限自动机
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "alg_fsm.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 状态机初始化
 *
 * @param __Status_Number 状态数量
 * @param __Now_Status_Serial 当前指定状态机初始编号
 */
void Class_FSM::Init(uint8_t __Status_Number, uint8_t __Now_Status_Serial)
{
    Status_Number = __Status_Number;

    Now_Status_Serial = __Now_Status_Serial;

    // 所有状态全刷0
    for (int i = 0; i < Status_Number; i++)
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
 */
void Class_FSM::TIM_Calculate_PeriodElapsedCallback()
{
    Status[Now_Status_Serial].Count_Time++;

    // 自己接着编写状态转移函数
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
