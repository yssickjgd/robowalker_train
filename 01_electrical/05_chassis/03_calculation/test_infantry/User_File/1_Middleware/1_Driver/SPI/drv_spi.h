/**
 * @file drv_spi.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的SPI通信初始化与配置流程
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-09-13 1.1 加入SPI配置操作
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

#ifndef DRV_SPI_H
#define DRV_SPI_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "spi.h"
#include <cstring>

/* Exported macros -----------------------------------------------------------*/

// 缓冲区字节长度
#define SPI_BUFFER_SIZE 256

/* Exported types ------------------------------------------------------------*/

/**
 * @brief SPI通信接收回调函数数据类型
 *
 */
typedef void (*SPI_Call_Back)(uint8_t *Tx_Buffer, uint8_t *Rx_Buffer, uint16_t Length);

/**
 * @brief CAN通信处理结构体
 *
 */
struct Struct_SPI_Manage_Object
{
    SPI_HandleTypeDef *SPI_Handler;
    GPIO_TypeDef *Now_GPIOx;
    uint16_t Now_GPIO_Pin;
    uint8_t Tx_Buffer[SPI_BUFFER_SIZE];
    uint8_t Rx_Buffer[SPI_BUFFER_SIZE];
    uint16_t Now_Tx_Length;
    uint16_t Now_Rx_Length;
    SPI_Call_Back Callback_Function;
};

/* Exported variables ---------------------------------------------------------*/

extern bool init_finished;

extern Struct_SPI_Manage_Object SPI1_Manage_Object;
extern Struct_SPI_Manage_Object SPI2_Manage_Object;
extern Struct_SPI_Manage_Object SPI3_Manage_Object;
extern Struct_SPI_Manage_Object SPI4_Manage_Object;
extern Struct_SPI_Manage_Object SPI5_Manage_Object;
extern Struct_SPI_Manage_Object SPI6_Manage_Object;

extern uint8_t SPI5_PF6_Tx_Data[];

/* Exported function declarations ---------------------------------------------*/

void SPI_Init(SPI_HandleTypeDef *hspi, SPI_Call_Back Callback_Function);

uint8_t SPI_Send_Receive_Data(SPI_HandleTypeDef *hspi, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint16_t Tx_Length, uint16_t Rx_Length);

void TIM_100us_SPI_PeriodElapsedCallback();

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
