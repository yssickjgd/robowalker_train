/**
 * @file drv_boarda_mpu6500.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief A板的板级支持包
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-09-28 1.1 新增A板MPU6500的IST8310开关
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

#ifndef DRV_DJIBOARDA_H
#define DRV_DJIBOARDA_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

// 初始化DC24被按位或的参数
#define BSP_DC24_LU_ON (1 << 0)
#define BSP_DC24_LD_ON (1 << 1)
#define BSP_DC24_RU_ON (1 << 2)
#define BSP_DC24_RD_ON (1 << 3)

// 初始化颜色LED被按位或的参数
#define BSP_LED_R_ON (1 << 4)
#define BSP_LED_G_ON (1 << 5)

// 初始化并排LED被按位或的参数
#define BSP_LED_1_ON (1 << 6)
#define BSP_LED_2_ON (1 << 7)
#define BSP_LED_3_ON (1 << 8)
#define BSP_LED_4_ON (1 << 9)
#define BSP_LED_5_ON (1 << 10)
#define BSP_LED_6_ON (1 << 11)
#define BSP_LED_7_ON (1 << 12)
#define BSP_LED_8_ON (1 << 13)

// A板引脚别名
#define BoardA_PWM_MPU6500_Heater_Pin GPIO_PIN_5
#define BoardA_PWM_MPU6500_Heater_GPIO_Port GPIOB
#define BoardA_USART6_Tx_Pin GPIO_PIN_14
#define BoardA_USART6_Tx_GPIO_Port GPIOG
#define BoardA_Pin_J2_Pin GPIO_PIN_4
#define BoardA_Pin_J2_GPIO_Port GPIOE
#define BoardA_Pin_J1_Pin GPIO_PIN_5
#define BoardA_Pin_J1_GPIO_Port GPIOE
#define BoardA_Pin_K1_Pin GPIO_PIN_6
#define BoardA_Pin_K1_GPIO_Port GPIOE
#define BoardA_DBUS_Rx_Pin GPIO_PIN_7
#define BoardA_DBUS_Rx_GPIO_Port GPIOB
#define BoardA_DBUS_Idle_Pin GPIO_PIN_6
#define BoardA_DBUS_Idle_GPIO_Port GPIOB
#define BoardA_Bluetooth_Rx_Pin GPIO_PIN_6
#define BoardA_Bluetooth_Rx_GPIO_Port GPIOD
#define BoardA_CAN1_Rx_Pin GPIO_PIN_0
#define BoardA_CAN1_Rx_GPIO_Port GPIOD
#define BoardA_USB_DP_Pin GPIO_PIN_12
#define BoardA_USB_DP_GPIO_Port GPIOA
#define BoardA_Pin_Y_Pin GPIO_PIN_7
#define BoardA_Pin_Y_GPIO_Port GPIOI
#define BoardA_Pin_X_Pin GPIO_PIN_6
#define BoardA_Pin_X_GPIO_Port GPIOI
#define BoardA_Pin_W_Pin GPIO_PIN_5
#define BoardA_Pin_W_GPIO_Port GPIOI
#define BoardA_USART6_Rx_Pin GPIO_PIN_9
#define BoardA_USART6_Rx_GPIO_Port GPIOG
#define BoardA_Bluetooth_Tx_Pin GPIO_PIN_5
#define BoardA_Bluetooth_Tx_GPIO_Port GPIOD
#define BoardA_CAN1_Tx_Pin GPIO_PIN_1
#define BoardA_CAN1_Tx_GPIO_Port GPIOD
#define BoardA_Pin_Z_Pin GPIO_PIN_2
#define BoardA_Pin_Z_GPIO_Port GPIOI
#define BoardA_USB_DM_Pin GPIO_PIN_11
#define BoardA_USB_DM_GPIO_Port GPIOA
#define BoardA_Pin_Q2_Pin GPIO_PIN_9
#define BoardA_Pin_Q2_GPIO_Port GPIOI
#define BoardA_Pin_I2_Pin GPIO_PIN_0
#define BoardA_Pin_I2_GPIO_Port GPIOF
#define BoardA_Pin_A_Pin GPIO_PIN_0
#define BoardA_Pin_A_GPIO_Port GPIOI
#define BoardA_DC24_RD_Pin GPIO_PIN_2
#define BoardA_DC24_RD_GPIO_Port GPIOH
#define BoardA_DC24_RU_Pin GPIO_PIN_3
#define BoardA_DC24_RU_GPIO_Port GPIOH
#define BoardA_Pin_I1_Pin GPIO_PIN_1
#define BoardA_Pin_I1_GPIO_Port GPIOF
#define BoardA_DC24_LD_Pin GPIO_PIN_4
#define BoardA_DC24_LD_GPIO_Port GPIOH
#define BoardA_LED_8_Pin GPIO_PIN_8
#define BoardA_LED_8_GPIO_Port GPIOG
#define BoardA_DC24_LU_Pin GPIO_PIN_5
#define BoardA_DC24_LU_GPIO_Port GPIOH
#define BoardA_LED_7_Pin GPIO_PIN_7
#define BoardA_LED_7_GPIO_Port GPIOG
#define BoardA_LED_6_Pin GPIO_PIN_6
#define BoardA_LED_6_GPIO_Port GPIOG
#define BoardA_SPI5_Clk_Pin GPIO_PIN_7
#define BoardA_SPI5_Clk_GPIO_Port GPIOF
#define BoardA_MPU6500_CS_Pin GPIO_PIN_6
#define BoardA_MPU6500_CS_GPIO_Port GPIOF
#define BoardA_Pin_B_Pin GPIO_PIN_12
#define BoardA_Pin_B_GPIO_Port GPIOH
#define BoardA_LED_5_Pin GPIO_PIN_5
#define BoardA_LED_5_GPIO_Port GPIOG
#define BoardA_LED_4_Pin GPIO_PIN_4
#define BoardA_LED_4_GPIO_Port GPIOG
#define BoardA_LED_3_Pin GPIO_PIN_3
#define BoardA_LED_3_GPIO_Port GPIOG
#define BoardA_Pin_Q1_Pin GPIO_PIN_10
#define BoardA_Pin_Q1_GPIO_Port GPIOF
#define BoardA_SPI5_MOSI_Pin GPIO_PIN_9
#define BoardA_SPI5_MOSI_GPIO_Port GPIOF
#define BoardA_SPI5_MISO_Pin GPIO_PIN_8
#define BoardA_SPI5_MISO_GPIO_Port GPIOF
#define BoardA_Pin_C_Pin GPIO_PIN_11
#define BoardA_Pin_C_GPIO_Port GPIOH
#define BoardA_Pin_D_Pin GPIO_PIN_10
#define BoardA_Pin_D_GPIO_Port GPIOH
#define BoardA_Pin_E_Pin GPIO_PIN_15
#define BoardA_Pin_E_GPIO_Port GPIOD
#define BoardA_LED_2_Pin GPIO_PIN_2
#define BoardA_LED_2_GPIO_Port GPIOG
#define BoardA_Pin_N2_Pin GPIO_PIN_0
#define BoardA_Pin_N2_GPIO_Port GPIOC
#define BoardA_Pin_O2_Pin GPIO_PIN_1
#define BoardA_Pin_O2_GPIO_Port GPIOC
#define BoardA_Pin_L1_Pin GPIO_PIN_2
#define BoardA_Pin_L1_GPIO_Port GPIOC
#define BoardA_Pin_M1_Pin GPIO_PIN_3
#define BoardA_Pin_M1_GPIO_Port GPIOC
#define BoardA_LED_1_Pin GPIO_PIN_1
#define BoardA_LED_1_GPIO_Port GPIOG
#define BoardA_PWM_Buzzer_Pin GPIO_PIN_6
#define BoardA_PWM_Buzzer_GPIO_Port GPIOH
#define BoardA_Pin_F_Pin GPIO_PIN_14
#define BoardA_Pin_F_GPIO_Port GPIOD
#define BoardA_Pin_G_Pin GPIO_PIN_13
#define BoardA_Pin_G_GPIO_Port GPIOD
#define BoardA_Pin_T_Pin GPIO_PIN_1
#define BoardA_Pin_T_GPIO_Port GPIOA
#define BoardA_Pin_S_Pin GPIO_PIN_0
#define BoardA_Pin_S_GPIO_Port GPIOA
#define BoardA_Pin_P2_Pin GPIO_PIN_4
#define BoardA_Pin_P2_GPIO_Port GPIOA
#define BoardA_Pin_N1_Pin GPIO_PIN_4
#define BoardA_Pin_N1_GPIO_Port GPIOC
#define BoardA_Pin_H_Pin GPIO_PIN_12
#define BoardA_Pin_H_GPIO_Port GPIOD
#define BoardA_Pin_U_Pin GPIO_PIN_2
#define BoardA_Pin_U_GPIO_Port GPIOA
#define BoardA_Pin_P1_Pin GPIO_PIN_5
#define BoardA_Pin_P1_GPIO_Port GPIOA
#define BoardA_Pin_O1_Pin GPIO_PIN_5
#define BoardA_Pin_O1_GPIO_Port GPIOC
#define BoardA_UART7_Tx_Pin GPIO_PIN_8
#define BoardA_UART7_Tx_GPIO_Port GPIOE
#define BoardA_LED_R_Pin GPIO_PIN_11
#define BoardA_LED_R_GPIO_Port GPIOE
#define BoardA_CAN2_Rx_Pin GPIO_PIN_12
#define BoardA_CAN2_Rx_GPIO_Port GPIOB
#define BoardA_CAN2_Tx_Pin GPIO_PIN_13
#define BoardA_CAN2_Tx_GPIO_Port GPIOB
#define BoardA_USART3_Rx_Pin GPIO_PIN_9
#define BoardA_USART3_Rx_GPIO_Port GPIOD
#define BoardA_USART3_Tx_Pin GPIO_PIN_8
#define BoardA_USART3_Tx_GPIO_Port GPIOD
#define BoardA_Pin_M2_Pin GPIO_PIN_1
#define BoardA_Pin_M2_GPIO_Port GPIOB
#define BoardA_Pin_L2_Pin GPIO_PIN_0
#define BoardA_Pin_L2_GPIO_Port GPIOB
#define BoardA_LED_G_Pin GPIO_PIN_14
#define BoardA_LED_G_GPIO_Port GPIOF
#define BoardA_UART7_Rx_Pin GPIO_PIN_7
#define BoardA_UART7_Rx_GPIO_Port GPIOE
#define BoardA_Pin_K2_Pin GPIO_PIN_12
#define BoardA_Pin_K2_GPIO_Port GPIOE

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

/* Exported variables --------------------------------------------------------*/

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim12;

/* Exported function declarations --------------------------------------------*/

void BSP_Init(uint32_t Status, float IMU_Heater_Rate = 0, float Buzzer_Rate = 0);

Enum_BSP_DC24_Status BSP_Get_DC24_LU();

Enum_BSP_DC24_Status BSP_Get_DC24_LD();

Enum_BSP_DC24_Status BSP_Get_DC24_RU();

Enum_BSP_DC24_Status BSP_Get_DC24_RD();

Enum_BSP_LED_Status BSP_Get_LED_G();

Enum_BSP_LED_Status BSP_Get_LED_R();

Enum_BSP_LED_Status BSP_Get_LED_1();

Enum_BSP_LED_Status BSP_Get_LED_2();

Enum_BSP_LED_Status BSP_Get_LED_3();

Enum_BSP_LED_Status BSP_Get_LED_4();

Enum_BSP_LED_Status BSP_Get_LED_5();

Enum_BSP_LED_Status BSP_Get_LED_6();

Enum_BSP_LED_Status BSP_Get_LED_7();

Enum_BSP_LED_Status BSP_Get_LED_8();

Enum_BSP_Key_Status BSP_Get_Key();

void BSP_Set_DC24_LU(Enum_BSP_DC24_Status Status);

void BSP_Set_DC24_LD(Enum_BSP_DC24_Status Status);

void BSP_Set_DC24_RU(Enum_BSP_DC24_Status Status);

void BSP_Set_DC24_RD(Enum_BSP_DC24_Status Status);

void BSP_Set_LED_R(Enum_BSP_LED_Status Status);

void BSP_Set_LED_G(Enum_BSP_LED_Status Status);

void BSP_Set_LED_1(Enum_BSP_LED_Status Status);

void BSP_Set_LED_2(Enum_BSP_LED_Status Status);

void BSP_Set_LED_3(Enum_BSP_LED_Status Status);

void BSP_Set_LED_4(Enum_BSP_LED_Status Status);

void BSP_Set_LED_5(Enum_BSP_LED_Status Status);

void BSP_Set_LED_6(Enum_BSP_LED_Status Status);

void BSP_Set_LED_7(Enum_BSP_LED_Status Status);

void BSP_Set_LED_8(Enum_BSP_LED_Status Status);

void BSP_Set_PWM_IMU_Heater(float Rate);

void BSP_Set_PWM_Buzzer(float Rate);

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
