/**
 * @file bsp_key.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 板载按键
 * @version 0.1
 * @date 2025-08-15 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "bsp_key.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Class_Key BSP_Key;

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化按键
 */
void Class_Key::Init()
{
    Pre_GPIO_State = HAL_GPIO_ReadPin(KEY__INPUT_GPIO_Port, KEY__INPUT_Pin);
}

/**
 * @brief TIM定时器中断处理按键状态
 *
 */
void Class_Key::TIM_1ms_Process_PeriodElapsedCallback()
{
    if (Pre_GPIO_State == KEY_FREE_STATE)
    {
        if (Now_GPIO_State == KEY_FREE_STATE)
        {
            Key_Status = BSP_Key_Status_FREE;
        }
        else
        {
            Key_Status = BSP_Key_Status_TRIG_FREE_PRESSED;
        }
    }
    else
    {
        if (Now_GPIO_State == KEY_FREE_STATE)
        {
            Key_Status = BSP_Key_Status_TRIG_PRESSED_FREE;
        }
        else
        {
            Key_Status = BSP_Key_Status_PRESSED;
        }
    }

    Pre_GPIO_State = Now_GPIO_State;
}

/**
 * @brief TIM定时器中断读取按键状态
 *
 */
void Class_Key::TIM_50ms_Read_PeriodElapsedCallback()
{
    Now_GPIO_State = HAL_GPIO_ReadPin(KEY__INPUT_GPIO_Port, KEY__INPUT_Pin);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/