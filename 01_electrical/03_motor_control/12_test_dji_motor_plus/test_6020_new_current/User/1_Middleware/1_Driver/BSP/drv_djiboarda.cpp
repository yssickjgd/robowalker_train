/**
 * @file drv_bsp-boarda.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief A板的板级支持包
 * @version 1.2
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-09-28 1.1 新增A板MPU6500的IST8310开关
 * @date 2023-11-18 1.2 修改成cpp
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_djiboarda.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化全部板级支持包引脚
 *
 * @param Status 各个状态的按位或
 */
void BSP_Init(uint32_t Status, float IMU_Heater_Rate, float Buzzer_Rate)
{
    BSP_Set_DC24_LU(static_cast<Enum_BSP_DC24_Status>((Status & BSP_DC24_LU_ON) == 0 ? BSP_DC24_Status_DISABLED : BSP_DC24_Status_ENABLED));
    BSP_Set_DC24_LD(static_cast<Enum_BSP_DC24_Status>((Status & BSP_DC24_LD_ON) == 0 ? BSP_DC24_Status_DISABLED : BSP_DC24_Status_ENABLED));
    BSP_Set_DC24_RU(static_cast<Enum_BSP_DC24_Status>((Status & BSP_DC24_RU_ON) == 0 ? BSP_DC24_Status_DISABLED : BSP_DC24_Status_ENABLED));
    BSP_Set_DC24_RD(static_cast<Enum_BSP_DC24_Status>((Status & BSP_DC24_RD_ON) == 0 ? BSP_DC24_Status_DISABLED : BSP_DC24_Status_ENABLED));

    BSP_Set_LED_R(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_R_ON) == 0 ? BSP_LED_Status_DISABLED : BSP_LED_Status_ENABLED));
    BSP_Set_LED_G(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_G_ON) == 0 ? BSP_LED_Status_DISABLED : BSP_LED_Status_ENABLED));

    BSP_Set_LED_1(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_1_ON) == 0 ? BSP_LED_Status_DISABLED : BSP_LED_Status_ENABLED));
    BSP_Set_LED_2(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_2_ON) == 0 ? BSP_LED_Status_DISABLED : BSP_LED_Status_ENABLED));
    BSP_Set_LED_3(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_3_ON) == 0 ? BSP_LED_Status_DISABLED : BSP_LED_Status_ENABLED));
    BSP_Set_LED_4(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_4_ON) == 0 ? BSP_LED_Status_DISABLED : BSP_LED_Status_ENABLED));
    BSP_Set_LED_5(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_5_ON) == 0 ? BSP_LED_Status_DISABLED : BSP_LED_Status_ENABLED));
    BSP_Set_LED_6(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_6_ON) == 0 ? BSP_LED_Status_DISABLED : BSP_LED_Status_ENABLED));
    BSP_Set_LED_7(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_7_ON) == 0 ? BSP_LED_Status_DISABLED : BSP_LED_Status_ENABLED));
    BSP_Set_LED_8(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_8_ON) == 0 ? BSP_LED_Status_DISABLED : BSP_LED_Status_ENABLED));

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);

    BSP_Set_PWM_IMU_Heater(IMU_Heater_Rate);
    BSP_Set_PWM_Buzzer(Buzzer_Rate);
}

/**
 * @brief 获取左上角DC24
 *
 * @return Enum_BSP_DC24_Status 状态
 */
Enum_BSP_DC24_Status BSP_Get_DC24_LU()
{
    return (static_cast<Enum_BSP_DC24_Status>(HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_5)));
}

/**
 * @brief 获取左下角DC24
 *
 * @return Enum_BSP_DC24_Status 状态
 */
Enum_BSP_DC24_Status BSP_Get_DC24_LD()
{
    return (static_cast<Enum_BSP_DC24_Status>(HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_4)));
}

/**
 * @brief 获取右上角DC24
 *
 * @return Enum_BSP_DC24_Status 状态
 */
Enum_BSP_DC24_Status BSP_Get_DC24_RU()
{
    return (static_cast<Enum_BSP_DC24_Status>(HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_3)));
}

/**
 * @brief 获取右下角DC24
 *
 * @return Enum_BSP_DC24_Status 状态
 */
Enum_BSP_DC24_Status BSP_Get_DC24_RD()
{
    return (static_cast<Enum_BSP_DC24_Status>(HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_2)));
}

/**
 * @brief 获取绿色LED
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_G()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_14)));
}

/**
 * @brief 获取红色LED
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_R()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11)));
}

/**
 * @brief 获取1号LED
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_1()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_1)));
}

/**
 * @brief 获取2号LED
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_2()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_2)));
}

/**
 * @brief 获取3号LED
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_3()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_3)));
}

/**
 * @brief 获取4号LED
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_4()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_4)));
}

/**
 * @brief 获取5号LED
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_5()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_5)));
}

/**
 * @brief 获取6号LED
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_6()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6)));
}

/**
 * @brief 获取7号LED
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_7()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7)));
}

/**
 * @brief 获取8号LED
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_8()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_8)));
}

/**
 * @brief 获取按键
 *
 * @return Enum_BSP_Key_Status 状态
 */
Enum_BSP_Key_Status BSP_Get_Key()
{
    static GPIO_PinState pre_key_status;
    GPIO_PinState key_status;
    Enum_BSP_Key_Status return_value;

    key_status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);

    switch (pre_key_status)
    {
    case (GPIO_PIN_RESET):
    {
        switch (key_status)
        {
        case (GPIO_PIN_RESET):
        {
            pre_key_status = key_status;
            return_value = BSP_Key_Status_FREE;

            break;
        }
        case (GPIO_PIN_SET):
        {
            pre_key_status = key_status;
            return_value = BSP_Key_Status_TRIG_FREE_PRESSED;

            break;
        }
        }

        break;
    }
    case (GPIO_PIN_SET):
    {
        switch (key_status)
        {
        case (GPIO_PIN_RESET):
        {
            pre_key_status = key_status;
            return_value = BSP_Key_Status_TRIG_PRESSED_FREE;

            break;
        }
        case (GPIO_PIN_SET):
        {
            pre_key_status = key_status;
            return_value = BSP_Key_Status_PRESSED;

            break;
        }
        }

        break;
    }
    }

    return (return_value);
}

/**
 * @brief 设定左上角DC24
 *
 * @param Status 状态
 */
void BSP_Set_DC24_LU(Enum_BSP_DC24_Status Status)
{
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定左下角DC24
 *
 * @param Status 状态
 */
void BSP_Set_DC24_LD(Enum_BSP_DC24_Status Status)
{
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定右上角DC24
 *
 * @param Status 状态
 */
void BSP_Set_DC24_RU(Enum_BSP_DC24_Status Status)
{
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_3, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定右下角DC24
 *
 * @param Status 状态
 */
void BSP_Set_DC24_RD(Enum_BSP_DC24_Status Status)
{
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定绿色LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_G(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定红色LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_R(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定1号LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_1(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_1, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定2号LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_2(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定3号LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_3(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定4号LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_4(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定5号LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_5(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_5, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定6号LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_6(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定7号LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_7(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定8号LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_8(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定IMU加热电阻
 *
 * @param Rate IMU加热电阻温度占空比
 */
void BSP_Set_PWM_IMU_Heater(float Rate)
{
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, Rate * 65535);
}

/**
 * @brief 设定蜂鸣器
 *
 * @param Rate 蜂鸣器响度占空比
 */
void BSP_Set_PWM_Buzzer(float Rate)
{
            __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_1, Rate * 1999);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
