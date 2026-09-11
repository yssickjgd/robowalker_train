/**
 * @file drv_spi.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的SPI通信初始化与配置流程
 * @version 3.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-09-13 1.1 加入SPI配置操作
 * @date 2023-11-19 1.2 修改成cpp, 24赛季定稿
 * @date 2024-08-22 2.1 新增回调函数空指针判定
 * @date 2025-08-13 3.1 适配达妙MC02板, 准备着手开发陀螺仪
 *
 * @copyright USTC-RoboWalker (c) 2023-2025
 *
 */

#ifndef DRV_SPI_H
#define DRV_SPI_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/System/Timestamp/sys_timestamp.h"
#include "spi.h"
#include "stm32h7xx_hal.h"
#include <string.h>

/* Exported macros -----------------------------------------------------------*/

// 缓冲区字节长度
#define SPI_BUFFER_SIZE 512

/* Exported types ------------------------------------------------------------*/

/**
 * @brief SPI通信接收回调函数数据类型
 *
 */
typedef void (*SPI_Callback)(uint8_t *Tx_Buffer, uint8_t *Rx_Buffer, uint16_t Tx_Length, uint16_t Rx_Length);

/**
 * @brief CAN通信处理结构体
 *
 */
struct Struct_SPI_Manage_Object
{
    SPI_HandleTypeDef *SPI_Handler;
    SPI_Callback Callback_Function;

    // 片选信号的GPIO与电平
    GPIO_TypeDef *Activate_GPIOx;
    uint16_t Activate_GPIO_Pin;
    GPIO_PinState Activate_Level;

    // 一次收发对应的数据长度
    uint8_t Tx_Buffer[SPI_BUFFER_SIZE];
    uint8_t Rx_Buffer[SPI_BUFFER_SIZE];
    uint16_t Tx_Buffer_Length;
    uint16_t Rx_Buffer_Length;

    // 接收时间戳
    uint64_t Rx_Timestamp;
};

/* Exported variables ---------------------------------------------------------*/

extern bool init_finished;

extern struct Struct_SPI_Manage_Object SPI1_Manage_Object;
extern struct Struct_SPI_Manage_Object SPI2_Manage_Object;
extern struct Struct_SPI_Manage_Object SPI3_Manage_Object;
extern struct Struct_SPI_Manage_Object SPI4_Manage_Object;
extern struct Struct_SPI_Manage_Object SPI5_Manage_Object;
extern struct Struct_SPI_Manage_Object SPI6_Manage_Object;

/* Exported function declarations ---------------------------------------------*/

void SPI_Init(SPI_HandleTypeDef *hspi, SPI_Callback Callback_Function);

uint8_t SPI_Transmit_Data(SPI_HandleTypeDef *hspi, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState Activate_Level, uint16_t Tx_Length);

uint8_t SPI_Transmit_Receive_Data(SPI_HandleTypeDef *hspi, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState Activate_Level, uint16_t Tx_Length, uint16_t Rx_Length);

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/