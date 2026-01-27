/**
 * @file sys_timestamp.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 时间计算相关支持库
 * @version 0.1
 * @date 2025-08-16 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

/**
 * 要求: 使能并绑定一个定时器, 开外部中断, PSC分频到1MHz, ARR为3600000000
 * 保证arr计数器1us增一次, 1h触发一次外部中断
 */

/* Includes ------------------------------------------------------------------*/

#include "sys_timestamp.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Class_Timestamp SYS_Timestamp;

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化时间戳
 *
 * @param __TIM_Manage_Object 绑定的定时器, psc后为1MHz, arr要求是3 600 000 000, arr至少32位的寄存器
 */
void Class_Timestamp::Init(TIM_HandleTypeDef *htim)
{
    TIM_Handler = htim;
}

/**
 * @brief TIM定时器回调函数, 每3600s调用一次
 *
 */
void Class_Timestamp::TIM_3600s_PeriodElapsedCallback()
{
    TIM_Overflow_Count++;
}

/**
 * @brief 计算当前时间戳, 单位微秒
 *
 * @return uint64_t 当前时间戳
 */
uint64_t Class_Timestamp::Calculate_Timestamp() const
{
    // 当前时间
    uint64_t timestamp;
    // arr计数
    uint32_t arr_counter = TIM_Handler->Instance->CNT;

    timestamp = (uint64_t)(TIM_Overflow_Count) * 3600000000ULL + (uint64_t) arr_counter;

    return (timestamp);
}

/**
 * @brief 延迟指定秒数
 *
 * @param Second 延迟秒数
 */
void Namespace_SYS_Timestamp::Delay_Second(const uint32_t &Second)
{
    volatile uint64_t start_time = SYS_Timestamp.Get_Current_Timestamp();

    while ((uint64_t)(Second) * 1000000ULL + start_time > SYS_Timestamp.Get_Current_Timestamp())
    {
    }
}

/**
 * @brief 延迟指定毫秒数
 *
 * @param Millisecond 延迟毫秒数
 */
void Namespace_SYS_Timestamp::Delay_Millisecond(const uint32_t &Millisecond)
{
    volatile uint64_t start_time = SYS_Timestamp.Get_Current_Timestamp();

    while ((uint64_t)(Millisecond) * 1000ULL + start_time > SYS_Timestamp.Get_Current_Timestamp())
    {
    }
}

/**
 * @brief 延迟指定微秒数
 *
 * @param Microsecond 延迟微秒数
 */
void Namespace_SYS_Timestamp::Delay_Microsecond(const uint32_t &Microsecond)
{
    volatile uint64_t start_time = SYS_Timestamp.Get_Current_Timestamp();

    while ((uint64_t)(Microsecond) + start_time > SYS_Timestamp.Get_Current_Timestamp())
    {
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/