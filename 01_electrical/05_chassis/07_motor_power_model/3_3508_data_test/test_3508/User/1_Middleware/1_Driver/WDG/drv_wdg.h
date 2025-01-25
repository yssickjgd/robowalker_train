/**
 * @file drv_wdg.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的WDG配置流程
 * @version 0.1
 * @date 2024-02-02 0.1 新增
 *
 * @copyright USTC-RoboWalker (c) 2024
 *
 */

#ifndef DRV_WDG_H
#define DRV_WDG_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "iwdg.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

void IWDG_Independent_Feed();

void TIM_1ms_IWDG_PeriodElapsedCallback();

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
