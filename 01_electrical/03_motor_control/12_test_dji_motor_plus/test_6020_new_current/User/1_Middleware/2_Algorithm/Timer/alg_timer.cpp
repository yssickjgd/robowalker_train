/**
 * @file alg_timer.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 斜坡函数, 用于速度规划等
 * @version 0.1
 * @date 2024-06-02 0.1 新建文件
 *
 * @copyright USTC-RoboWalker (c) 2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "alg_timer.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化
 *
 * @param __Delay 增长最大幅度
 */
void Class_Timer::Init(uint32_t __Delay)
{
    Delay = __Delay;
}

/**
 * @brief 定时器调整状态, 计时周期1ms
 *
 */
void Class_Timer::TIM_1ms_Calculate_PeriodElapsedCallback()
{
    Tick++;

    if (Delay == 0)
    {
        Now_Status = Timer_Status_RESET;
    }
    else if (Tick < Delay)
    {
        Now_Status = Timer_Status_WAIT;
    }
    else if (Tick == Delay)
    {
        Now_Status = Timer_Status_TRIGGER;
    }
    else if (Tick > Delay)
    {
        Now_Status = Timer_Status_TIMEOUT;
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
