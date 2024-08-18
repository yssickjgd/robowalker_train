/**
 * @file drv_bsp.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief A板的板级支持包
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

#ifndef DRV_BSP_H
#define DRV_BSP_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

//初始化DC24被按位或的参数
#define BSP_DC24_LU_ON (1 << 0)
#define BSP_DC24_LD_ON (1 << 1)
#define BSP_DC24_RU_ON (1 << 2)
#define BSP_DC24_RD_ON (1 << 3)

//初始化颜色LED被按位或的参数
#define BSP_LED_R_ON (1 << 4)
#define BSP_LED_G_ON (1 << 5)

//初始化并排LED被按位或的参数
#define BSP_LED_1_ON (1 << 6)
#define BSP_LED_2_ON (1 << 7)
#define BSP_LED_3_ON (1 << 8)
#define BSP_LED_4_ON (1 << 9)
#define BSP_LED_5_ON (1 << 10)
#define BSP_LED_6_ON (1 << 11)
#define BSP_LED_7_ON (1 << 12)
#define BSP_LED_8_ON (1 << 13)

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 板上DC24工作状态
 *
 */
enum Enum_BSP_DC24_Status
{
    BSP_DC24_Status_DISABLED = 0,
    BSP_DC24_Status_ENABLED,
};

/**
 * @brief 板上LED工作状态
 *
 */
enum Enum_BSP_LED_Status
{
    BSP_LED_Status_ENABLED = 0,
    BSP_LED_Status_DISABLED,
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

void BSP_Init(uint32_t Status, float IMU_Heater_Rate = 0, float Buzzer_Rate = 0);

void BSP_DC24_LU(Enum_BSP_DC24_Status Status);
void BSP_DC24_LD(Enum_BSP_DC24_Status Status);
void BSP_DC24_RU(Enum_BSP_DC24_Status Status);
void BSP_DC24_RD(Enum_BSP_DC24_Status Status);

void BSP_LED_R(Enum_BSP_LED_Status Status);
void BSP_LED_G(Enum_BSP_LED_Status Status);

void BSP_LED_1(Enum_BSP_LED_Status Status);
void BSP_LED_2(Enum_BSP_LED_Status Status);
void BSP_LED_3(Enum_BSP_LED_Status Status);
void BSP_LED_4(Enum_BSP_LED_Status Status);
void BSP_LED_5(Enum_BSP_LED_Status Status);
void BSP_LED_6(Enum_BSP_LED_Status Status);
void BSP_LED_7(Enum_BSP_LED_Status Status);
void BSP_LED_8(Enum_BSP_LED_Status Status);

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
