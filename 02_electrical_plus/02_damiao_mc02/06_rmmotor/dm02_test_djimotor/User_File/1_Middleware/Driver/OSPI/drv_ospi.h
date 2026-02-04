/**
 * @file drv_ospi.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的OSPI通信初始化与配置流程
 * @version 0.1
 * @date 2023-08-29 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef DRV_OSPI_H
#define DRV_OSPI_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/System/Timestamp/sys_timestamp.h"
#include "octospi.h"
#include "stm32h7xx_hal.h"
#include <string.h>

/* Exported macros -----------------------------------------------------------*/

// 缓冲区字节长度
#define OSPI_BUFFER_SIZE 512

/* Exported types ------------------------------------------------------------*/

typedef void (*OSPI_Status_Match_Callback)();

typedef void (*OSPI_Rx_Callback)(uint8_t *Buffer);

typedef void (*OSPI_Tx_Callback)(uint8_t *Buffer);

/**
 * @brief OSPI通信处理结构体
 *
 */
struct Struct_OSPI_Manage_Object
{
    OSPI_HandleTypeDef *OSPI_Handler;
    OSPI_Status_Match_Callback Status_Match_Callback_Function;
    OSPI_Rx_Callback Rx_Callback_Function;
    OSPI_Tx_Callback Tx_Callback_Function;

    // 收发缓冲区
    uint8_t Tx_Buffer[OSPI_BUFFER_SIZE];
    uint8_t Rx_Buffer[OSPI_BUFFER_SIZE];

    // 轮询结束时间戳
    uint64_t Auto_Polling_Timestamp;
    // 接收时间戳
    uint64_t Rx_Timestamp;
};

/* Exported variables ---------------------------------------------------------*/

extern bool init_finished;

extern struct Struct_OSPI_Manage_Object OSPI1_Manage_Object;
extern struct Struct_OSPI_Manage_Object OSPI2_Manage_Object;

/* Exported function declarations ---------------------------------------------*/

void OSPI_Init(OSPI_HandleTypeDef *hospi, OSPI_Status_Match_Callback Auto_Polling_Callback_Function, OSPI_Rx_Callback Rx_Callback_Function, OSPI_Tx_Callback Tx_Callback_Function);

void OSPI_Auto_Polling(OSPI_HandleTypeDef * hospi, OSPI_AutoPollingTypeDef * Config);

void OSPI_Command(OSPI_HandleTypeDef * hospi, OSPI_RegularCmdTypeDef * Command);

void OSPI_Command_Transmit_Data(OSPI_HandleTypeDef * hospi, OSPI_RegularCmdTypeDef * Command);

void OSPI_Command_Receive_Data(OSPI_HandleTypeDef * hospi, OSPI_RegularCmdTypeDef * Command);

void OSPI_Command_Transmit_Receive_Data(OSPI_HandleTypeDef * hospi, OSPI_RegularCmdTypeDef * Command);

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/