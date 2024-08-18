/**
 * @file drv_uart.c
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的UART通信初始化与配置流程
 * @version 0.1
 * @date 2022-08-05
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_uart.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Struct_UART_Manage_Object UART1_Manage_Object = {0};
Struct_UART_Manage_Object UART2_Manage_Object = {0};
Struct_UART_Manage_Object UART3_Manage_Object = {0};
Struct_UART_Manage_Object UART4_Manage_Object = {0};
Struct_UART_Manage_Object UART5_Manage_Object = {0};
Struct_UART_Manage_Object UART6_Manage_Object = {0};
Struct_UART_Manage_Object UART7_Manage_Object = {0};
Struct_UART_Manage_Object UART8_Manage_Object = {0};

/* Private function declarations ---------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化UART
 *
 * @param huart UART编号
 * @param Callback_Function 处理回调函数
 */
void UART_Init(UART_HandleTypeDef *huart, UART_Call_Back Callback_Function, uint16_t Rx_Buffer_Length)
{
    if (huart->Instance == USART1)
    {
        UART1_Manage_Object.UART_Handler = huart;
        UART1_Manage_Object.Callback_Function = Callback_Function;
        UART1_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer, UART1_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == USART2)
    {
        UART2_Manage_Object.UART_Handler = huart;
        UART2_Manage_Object.Callback_Function = Callback_Function;
        UART2_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer, UART2_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == USART3)
    {
        UART3_Manage_Object.UART_Handler = huart;
        UART3_Manage_Object.Callback_Function = Callback_Function;
        UART3_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART3_Manage_Object.Rx_Buffer, UART3_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == UART4)
    {
        UART4_Manage_Object.UART_Handler = huart;
        UART4_Manage_Object.Callback_Function = Callback_Function;
        UART4_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART4_Manage_Object.Rx_Buffer, UART4_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == UART5)
    {
        UART5_Manage_Object.UART_Handler = huart;
        UART5_Manage_Object.Callback_Function = Callback_Function;
        UART5_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART5_Manage_Object.Rx_Buffer, UART5_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == USART6)
    {
        UART6_Manage_Object.UART_Handler = huart;
        UART6_Manage_Object.Callback_Function = Callback_Function;
        UART6_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer, UART6_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == UART7)
    {
        UART7_Manage_Object.UART_Handler = huart;
        UART7_Manage_Object.Callback_Function = Callback_Function;
        UART7_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART7_Manage_Object.Rx_Buffer, UART7_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == UART8)
    {
        UART8_Manage_Object.UART_Handler = huart;
        UART8_Manage_Object.Callback_Function = Callback_Function;
        UART8_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART8_Manage_Object.Rx_Buffer, UART8_Manage_Object.Rx_Buffer_Length);
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
uint8_t UART_Send_Data(UART_HandleTypeDef *huart, uint8_t *Data, uint16_t Length)
{
    return (HAL_UART_Transmit_DMA(huart, Data, Length));
}

/**
 * @brief UART的TIM定时器中断发送回调函数
 *
 */
void TIM_UART_PeriodElapsedCallback()
{
    // UART2串口绘图
    UART_Send_Data(&huart2, UART2_Manage_Object.Tx_Buffer, 1 + 12 * sizeof(float));
}

/**
 * @brief HAL库UART接收DMA空闲中断
 *
 * @param huart UART编号
 * @param Size 长度
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    //选择回调函数
    if (huart->Instance == USART1)
    {
        UART1_Manage_Object.Callback_Function(UART1_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer, UART1_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == USART2)
    {
        UART2_Manage_Object.Callback_Function(UART2_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer, UART2_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == USART3)
    {
        UART3_Manage_Object.Callback_Function(UART3_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART3_Manage_Object.Rx_Buffer, UART3_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == UART4)
    {
        UART4_Manage_Object.Callback_Function(UART4_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART4_Manage_Object.Rx_Buffer, UART4_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == UART5)
    {
        UART5_Manage_Object.Callback_Function(UART5_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART5_Manage_Object.Rx_Buffer, UART5_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == USART6)
    {
        UART6_Manage_Object.Callback_Function(UART6_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer, UART6_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == UART7)
    {
        UART7_Manage_Object.Callback_Function(UART7_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART7_Manage_Object.Rx_Buffer, UART7_Manage_Object.Rx_Buffer_Length);
    }
    else if (huart->Instance == UART8)
    {
        UART8_Manage_Object.Callback_Function(UART8_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART8_Manage_Object.Rx_Buffer, UART8_Manage_Object.Rx_Buffer_Length);
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
