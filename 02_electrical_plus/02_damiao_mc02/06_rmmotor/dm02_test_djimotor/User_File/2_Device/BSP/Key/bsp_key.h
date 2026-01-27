/**
 * @file bsp_key.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 板载按键
 * @version 0.1
 * @date 2025-08-15 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef BSP_KEY_H
#define BSP_KEY_H

/* Includes ------------------------------------------------------------------*/

#include "gpio.h"
#include "stm32h7xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 按键状态
 *
 */
enum Enum_BSP_Key_Status
{
    BSP_Key_Status_FREE = 0,
    BSP_Key_Status_TRIG_FREE_PRESSED,
    BSP_Key_Status_TRIG_PRESSED_FREE,
    BSP_Key_Status_PRESSED,
};

/**
 * @brief Specialized, WS2812单个灯珠
 *
 */
class Class_Key
{
public:
    void Init();

    inline Enum_BSP_Key_Status Get_Key_Status() const;

    void TIM_1ms_Process_PeriodElapsedCallback();

    void TIM_50ms_Read_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // 按键未按下时GPIO状态
    const GPIO_PinState KEY_FREE_STATE = GPIO_PIN_SET;

    // 内部变量

    // 上一次按键状态
    GPIO_PinState Pre_GPIO_State;
    // 当前按键状态
    GPIO_PinState Now_GPIO_State;

    // 读变量

    // 按键状态
    Enum_BSP_Key_Status Key_Status = BSP_Key_Status_FREE;

    // 写变量

    // 读写变量

    // 内部函数
};

/* Exported variables --------------------------------------------------------*/

extern Class_Key BSP_Key;

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取按键状态
 *
 * @return Enum_BSP_Key_Status 按键状态
 */
inline Enum_BSP_Key_Status Class_Key::Get_Key_Status() const
{
    return (Key_Status);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/