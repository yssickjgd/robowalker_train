/**
 * @file drv_wdg.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的WDG配置流程
 * @version 0.1
 * @date 2024-02-02 0.1 新增
 *
 * @copyright USTC-RoboWalker (c) 2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_wdg.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 喂独立看门狗
 *
 */
void IWDG_Independent_Feed()
{
    HAL_IWDG_Refresh(&hiwdg);
}

/**
 * @brief UART的TIM定时器中断发送回调函数
 *
 */
void TIM_1ms_IWDG_PeriodElapsedCallback()
{
    IWDG_Independent_Feed();
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
