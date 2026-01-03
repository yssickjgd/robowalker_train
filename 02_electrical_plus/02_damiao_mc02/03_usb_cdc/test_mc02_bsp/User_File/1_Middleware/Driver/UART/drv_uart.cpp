/**
 * @file drv_uart.cpp
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

/* Includes ------------------------------------------------------------------*/

#include "drv_uart.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Struct_UART_Manage_Object UART1_Manage_Object = {nullptr};
Struct_UART_Manage_Object UART2_Manage_Object = {nullptr};
Struct_UART_Manage_Object UART3_Manage_Object = {nullptr};
Struct_UART_Manage_Object UART4_Manage_Object = {nullptr};
Struct_UART_Manage_Object UART5_Manage_Object = {nullptr};
Struct_UART_Manage_Object UART6_Manage_Object = {nullptr};
Struct_UART_Manage_Object UART7_Manage_Object = {nullptr};
Struct_UART_Manage_Object UART8_Manage_Object = {nullptr};
Struct_UART_Manage_Object UART9_Manage_Object = {nullptr};
Struct_UART_Manage_Object UART10_Manage_Object = {nullptr};

/* Private function declarations ---------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化UART
 *
 * @param huart UART编号
 * @param Callback_Function 处理回调函数
 * @param Rx_Buffer_Length 接收缓冲区长度
 */
void UART_Init(UART_HandleTypeDef *huart, UART_Callback Callback_Function)
{
    if (huart->Instance == USART1)
    {
        UART1_Manage_Object.UART_Handler = huart;
        UART1_Manage_Object.Callback_Function = Callback_Function;

        UART1_Manage_Object.Rx_Buffer_Active = UART1_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == USART2)
    {
        UART2_Manage_Object.UART_Handler = huart;
        UART2_Manage_Object.Callback_Function = Callback_Function;

        UART2_Manage_Object.Rx_Buffer_Active = UART2_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == USART3)
    {
        UART3_Manage_Object.UART_Handler = huart;
        UART3_Manage_Object.Callback_Function = Callback_Function;

        UART3_Manage_Object.Rx_Buffer_Active = UART3_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART3_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == UART4)
    {
        UART4_Manage_Object.UART_Handler = huart;
        UART4_Manage_Object.Callback_Function = Callback_Function;

        UART4_Manage_Object.Rx_Buffer_Active = UART4_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART4_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == UART5)
    {
        UART5_Manage_Object.UART_Handler = huart;
        UART5_Manage_Object.Callback_Function = Callback_Function;

        UART5_Manage_Object.Rx_Buffer_Active = UART5_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART5_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == USART6)
    {
        UART6_Manage_Object.UART_Handler = huart;
        UART6_Manage_Object.Callback_Function = Callback_Function;

        UART6_Manage_Object.Rx_Buffer_Active = UART6_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == UART7)
    {
        UART7_Manage_Object.UART_Handler = huart;
        UART7_Manage_Object.Callback_Function = Callback_Function;

        UART7_Manage_Object.Rx_Buffer_Active = UART7_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART7_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == UART8)
    {
        UART8_Manage_Object.UART_Handler = huart;
        UART8_Manage_Object.Callback_Function = Callback_Function;

        UART8_Manage_Object.Rx_Buffer_Active = UART8_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART8_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == UART9)
    {
        UART9_Manage_Object.UART_Handler = huart;
        UART9_Manage_Object.Callback_Function = Callback_Function;

        UART9_Manage_Object.Rx_Buffer_Active = UART9_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART9_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == USART10)
    {
        UART10_Manage_Object.UART_Handler = huart;
        UART10_Manage_Object.Callback_Function = Callback_Function;

        UART10_Manage_Object.Rx_Buffer_Active = UART10_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART10_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
}

/**
 * @brief 重新初始化UART
 *
 * @param huart UART编号
 */
void UART_Reinit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        UART1_Manage_Object.Rx_Buffer_Active = UART1_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == USART2)
    {
        UART2_Manage_Object.Rx_Buffer_Active = UART2_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == USART3)
    {
        UART3_Manage_Object.Rx_Buffer_Active = UART3_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART3_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == UART4)
    {
        UART4_Manage_Object.Rx_Buffer_Active = UART4_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART4_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == UART5)
    {
        UART5_Manage_Object.Rx_Buffer_Active = UART5_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART5_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == USART6)
    {
        UART6_Manage_Object.Rx_Buffer_Active = UART6_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == UART7)
    {
        UART7_Manage_Object.Rx_Buffer_Active = UART7_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART7_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == UART8)
    {
        UART8_Manage_Object.Rx_Buffer_Active = UART8_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART8_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == UART9)
    {
        UART9_Manage_Object.Rx_Buffer_Active = UART9_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART9_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
    else if (huart->Instance == USART10)
    {
        UART10_Manage_Object.Rx_Buffer_Active = UART10_Manage_Object.Rx_Buffer_0;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART10_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
    }
}

/**
 * @brief 发送数据帧
 *
 * @param huart UART编号
 * @param Data 被发送的数据指针
 * @param Length 长度
 * @return uint8_t 执行状态
 */
uint8_t UART_Transmit_Data(UART_HandleTypeDef *huart, uint8_t *Data, uint16_t Length)
{
    return (HAL_UART_Transmit_DMA(huart, Data, Length));
}

/**
 * @brief HAL库UART接收DMA空闲中断
 *
 * @param huart UART编号
 * @param Size 长度
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    // 判断程序初始化完成
    if (!init_finished)
    {
        // 重启接收
        if (huart->Instance == USART1)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
        }
        else if (huart->Instance == USART2)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
        }
        else if (huart->Instance == USART3)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(huart, UART3_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
        }
        else if (huart->Instance == UART4)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(huart, UART4_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
        }
        else if (huart->Instance == UART5)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(huart, UART5_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
        }
        else if (huart->Instance == USART6)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
        }
        else if (huart->Instance == UART7)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(huart, UART7_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
        }
        else if (huart->Instance == UART8)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(huart, UART8_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
        }
        else if (huart->Instance == UART9)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(huart, UART9_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
        }
        else if (huart->Instance == USART10)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(huart, UART10_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);
        }
        return;
    }

    // 选择回调函数
    if (huart->Instance == USART1)
    {
        UART1_Manage_Object.Rx_Buffer_Ready = UART1_Manage_Object.Rx_Buffer_Active;
        if (UART1_Manage_Object.Rx_Buffer_Active == UART1_Manage_Object.Rx_Buffer_0)
        {
            UART1_Manage_Object.Rx_Buffer_Active = UART1_Manage_Object.Rx_Buffer_1;
        }
        else
        {
            UART1_Manage_Object.Rx_Buffer_Active = UART1_Manage_Object.Rx_Buffer_0;
        }

        UART1_Manage_Object.Rx_Time_Stamp = SYS_Timestamp.Get_Current_Timestamp();

        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);

        if (UART1_Manage_Object.Callback_Function != nullptr)
        {
            UART1_Manage_Object.Callback_Function(UART1_Manage_Object.Rx_Buffer_Ready, Size);
        }
    }
    else if (huart->Instance == USART2)
    {
        UART2_Manage_Object.Rx_Buffer_Ready = UART2_Manage_Object.Rx_Buffer_Active;
        if (UART2_Manage_Object.Rx_Buffer_Active == UART2_Manage_Object.Rx_Buffer_0)
        {
            UART2_Manage_Object.Rx_Buffer_Active = UART2_Manage_Object.Rx_Buffer_1;
        }
        else
        {
            UART2_Manage_Object.Rx_Buffer_Active = UART2_Manage_Object.Rx_Buffer_0;
        }

        UART2_Manage_Object.Rx_Time_Stamp = SYS_Timestamp.Get_Current_Timestamp();

        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);

        if (UART2_Manage_Object.Callback_Function != nullptr)
        {
            UART2_Manage_Object.Callback_Function(UART2_Manage_Object.Rx_Buffer_Ready, Size);
        }
    }
    else if (huart->Instance == USART3)
    {
        UART3_Manage_Object.Rx_Buffer_Ready = UART3_Manage_Object.Rx_Buffer_Active;
        if (UART3_Manage_Object.Rx_Buffer_Active == UART3_Manage_Object.Rx_Buffer_0)
        {
            UART3_Manage_Object.Rx_Buffer_Active = UART3_Manage_Object.Rx_Buffer_1;
        }
        else
        {
            UART3_Manage_Object.Rx_Buffer_Active = UART3_Manage_Object.Rx_Buffer_0;
        }

        UART3_Manage_Object.Rx_Time_Stamp = SYS_Timestamp.Get_Current_Timestamp();

        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART3_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);

        if (UART3_Manage_Object.Callback_Function != nullptr)
        {
            UART3_Manage_Object.Callback_Function(UART3_Manage_Object.Rx_Buffer_Ready, Size);
        }
    }
    else if (huart->Instance == UART4)
    {
        UART4_Manage_Object.Rx_Buffer_Ready = UART4_Manage_Object.Rx_Buffer_Active;
        if (UART4_Manage_Object.Rx_Buffer_Active == UART4_Manage_Object.Rx_Buffer_0)
        {
            UART4_Manage_Object.Rx_Buffer_Active = UART4_Manage_Object.Rx_Buffer_1;
        }
        else
        {
            UART4_Manage_Object.Rx_Buffer_Active = UART4_Manage_Object.Rx_Buffer_0;
        }

        UART4_Manage_Object.Rx_Time_Stamp = SYS_Timestamp.Get_Current_Timestamp();

        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART4_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);

        if (UART4_Manage_Object.Callback_Function != nullptr)
        {
            UART4_Manage_Object.Callback_Function(UART4_Manage_Object.Rx_Buffer_Ready, Size);
        }
    }
    else if (huart->Instance == UART5)
    {
        UART5_Manage_Object.Rx_Buffer_Ready = UART5_Manage_Object.Rx_Buffer_Active;
        if (UART5_Manage_Object.Rx_Buffer_Active == UART5_Manage_Object.Rx_Buffer_0)
        {
            UART5_Manage_Object.Rx_Buffer_Active = UART5_Manage_Object.Rx_Buffer_1;
        }
        else
        {
            UART5_Manage_Object.Rx_Buffer_Active = UART5_Manage_Object.Rx_Buffer_0;
        }

        UART5_Manage_Object.Rx_Time_Stamp = SYS_Timestamp.Get_Current_Timestamp();

        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART5_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);

        if (UART5_Manage_Object.Callback_Function != nullptr)
        {
            UART5_Manage_Object.Callback_Function(UART5_Manage_Object.Rx_Buffer_Ready, Size);
        }
    }
    else if (huart->Instance == USART6)
    {
        UART6_Manage_Object.Rx_Buffer_Ready = UART6_Manage_Object.Rx_Buffer_Active;
        if (UART6_Manage_Object.Rx_Buffer_Active == UART6_Manage_Object.Rx_Buffer_0)
        {
            UART6_Manage_Object.Rx_Buffer_Active = UART6_Manage_Object.Rx_Buffer_1;
        }
        else
        {
            UART6_Manage_Object.Rx_Buffer_Active = UART6_Manage_Object.Rx_Buffer_0;
        }

        UART6_Manage_Object.Rx_Time_Stamp = SYS_Timestamp.Get_Current_Timestamp();

        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);

        if (UART6_Manage_Object.Callback_Function != nullptr)
        {
            UART6_Manage_Object.Callback_Function(UART6_Manage_Object.Rx_Buffer_Ready, Size);
        }
    }
    else if (huart->Instance == UART7)
    {
        UART7_Manage_Object.Rx_Buffer_Ready = UART7_Manage_Object.Rx_Buffer_Active;
        if (UART7_Manage_Object.Rx_Buffer_Active == UART7_Manage_Object.Rx_Buffer_0)
        {
            UART7_Manage_Object.Rx_Buffer_Active = UART7_Manage_Object.Rx_Buffer_1;
        }
        else
        {
            UART7_Manage_Object.Rx_Buffer_Active = UART7_Manage_Object.Rx_Buffer_0;
        }

        UART7_Manage_Object.Rx_Time_Stamp = SYS_Timestamp.Get_Current_Timestamp();

        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART7_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);

        if (UART7_Manage_Object.Callback_Function != nullptr)
        {
            UART7_Manage_Object.Callback_Function(UART7_Manage_Object.Rx_Buffer_Ready, Size);
        }
    }
    else if (huart->Instance == UART8)
    {
        UART8_Manage_Object.Rx_Buffer_Ready = UART8_Manage_Object.Rx_Buffer_Active;
        if (UART8_Manage_Object.Rx_Buffer_Active == UART8_Manage_Object.Rx_Buffer_0)
        {
            UART8_Manage_Object.Rx_Buffer_Active = UART8_Manage_Object.Rx_Buffer_1;
        }
        else
        {
            UART8_Manage_Object.Rx_Buffer_Active = UART8_Manage_Object.Rx_Buffer_0;
        }

        UART8_Manage_Object.Rx_Time_Stamp = SYS_Timestamp.Get_Current_Timestamp();

        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART8_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);

        if (UART8_Manage_Object.Callback_Function != nullptr)
        {
            UART8_Manage_Object.Callback_Function(UART8_Manage_Object.Rx_Buffer_Ready, Size);
        }
    }
    else if (huart->Instance == UART9)
    {
        UART9_Manage_Object.Rx_Buffer_Ready = UART9_Manage_Object.Rx_Buffer_Active;
        if (UART9_Manage_Object.Rx_Buffer_Active == UART9_Manage_Object.Rx_Buffer_0)
        {
            UART9_Manage_Object.Rx_Buffer_Active = UART9_Manage_Object.Rx_Buffer_1;
        }
        else
        {
            UART9_Manage_Object.Rx_Buffer_Active = UART9_Manage_Object.Rx_Buffer_0;
        }

        UART9_Manage_Object.Rx_Time_Stamp = SYS_Timestamp.Get_Current_Timestamp();

        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART9_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);

        if (UART9_Manage_Object.Callback_Function != nullptr)
        {
            UART9_Manage_Object.Callback_Function(UART9_Manage_Object.Rx_Buffer_Ready, Size);
        }
    }
    else if (huart->Instance == USART10)
    {
        UART10_Manage_Object.Rx_Buffer_Ready = UART10_Manage_Object.Rx_Buffer_Active;
        if (UART10_Manage_Object.Rx_Buffer_Active == UART10_Manage_Object.Rx_Buffer_0)
        {
            UART10_Manage_Object.Rx_Buffer_Active = UART10_Manage_Object.Rx_Buffer_1;
        }
        else
        {
            UART10_Manage_Object.Rx_Buffer_Active = UART10_Manage_Object.Rx_Buffer_0;
        }

        UART10_Manage_Object.Rx_Time_Stamp = SYS_Timestamp.Get_Current_Timestamp();

        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART10_Manage_Object.Rx_Buffer_Active, UART_BUFFER_SIZE);

        if (UART10_Manage_Object.Callback_Function != nullptr)
        {
            UART10_Manage_Object.Callback_Function(UART10_Manage_Object.Rx_Buffer_Ready, Size);
        }
    }
}

/**
 * @brief HAL库UART错误中断
 *
 * @param huart UART编号
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    UART_Reinit(huart);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/