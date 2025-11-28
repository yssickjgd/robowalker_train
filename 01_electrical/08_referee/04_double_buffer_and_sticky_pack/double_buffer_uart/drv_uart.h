/**
 * @file drv_uart.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的UART通信初始化与配置流程
 * @version 2.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-11-18 1.1 修改成cpp
 * @date 2024-05-05 1.2 新增错误中断
 * @date 2025-08-13 2.1 适配达妙MC02板
 *
 * @copyright USTC-RoboWalker (c) 2023-2025
 *
 */

#ifndef DRV_UART_H
#define DRV_UART_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/System/Timestamp/sys_timestamp.h"
#include "usart.h"
#include "stm32h7xx_hal.h"
#include <string.h>

/* Exported macros -----------------------------------------------------------*/

// 缓冲区字节长度
#define UART_BUFFER_SIZE 512

/* Exported types ------------------------------------------------------------*/

/**
 * @brief UART通信接收回调函数数据类型
 *
 */
typedef void (*UART_Callback)(uint8_t *Buffer, uint16_t Length);

/**
 * @brief UART通信处理结构体
 */
struct Struct_UART_Manage_Object
{
    UART_HandleTypeDef *UART_Handler;
    UART_Callback Callback_Function;

    // 双缓冲适配的缓冲区以及当前激活的缓冲区
    uint8_t Rx_Buffer_0[UART_BUFFER_SIZE];
    uint8_t Rx_Buffer_1[UART_BUFFER_SIZE];
    // 正在接收的缓冲区
    uint8_t *Rx_Buffer_Active;
    // 接收完毕的缓冲区
    uint8_t *Rx_Buffer_Ready;

    // 接收时间戳
    uint64_t Rx_Time_Stamp;
};

/* Exported variables --------------------------------------------------------*/

extern bool init_finished;

extern struct Struct_UART_Manage_Object UART1_Manage_Object;
extern struct Struct_UART_Manage_Object UART2_Manage_Object;
extern struct Struct_UART_Manage_Object UART3_Manage_Object;
extern struct Struct_UART_Manage_Object UART4_Manage_Object;
extern struct Struct_UART_Manage_Object UART5_Manage_Object;
extern struct Struct_UART_Manage_Object UART6_Manage_Object;
extern struct Struct_UART_Manage_Object UART7_Manage_Object;
extern struct Struct_UART_Manage_Object UART8_Manage_Object;
extern struct Struct_UART_Manage_Object UART9_Manage_Object;
extern struct Struct_UART_Manage_Object UART10_Manage_Object;

/* Exported function declarations --------------------------------------------*/

void UART_Init(UART_HandleTypeDef *huart, UART_Callback Callback_Function);

void UART_Reinit(UART_HandleTypeDef * huart);

uint8_t UART_Transmit_Data(UART_HandleTypeDef *huart, uint8_t *Data, uint16_t Length);

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/