/**
 * @file dvc_powermeter.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 自制功率计
 * @version 0.1
 * @date 2025-01-25 0.1 新建
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_powermeter.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 串口绘图初始化
 *
 * @param __huart 指定的UART
 * @param __Rx_Variable_Assignment_Num 接收指令字典的数量
 * @param __Rx_Variable_Assignment_List 接收指令字典列表
 * @param __Data_Type 传输数据类型, 默认float
 * @param __Frame_Header 帧头标识符
 */
void Class_Powermeter::Init(UART_HandleTypeDef *huart, uint8_t __Frame_Header)
{
    if (huart->Instance == USART1)
    {
        UART_Manage_Object = &UART1_Manage_Object;
    }
    else if (huart->Instance == USART2)
    {
        UART_Manage_Object = &UART2_Manage_Object;
    }
    else if (huart->Instance == USART3)
    {
        UART_Manage_Object = &UART3_Manage_Object;
    }
    else if (huart->Instance == UART4)
    {
        UART_Manage_Object = &UART4_Manage_Object;
    }
    else if (huart->Instance == UART5)
    {
        UART_Manage_Object = &UART5_Manage_Object;
    }
    else if (huart->Instance == USART6)
    {
        UART_Manage_Object = &UART6_Manage_Object;
    }
    else if (huart->Instance == UART7)
    {
        UART_Manage_Object = &UART7_Manage_Object;
    }
    else if (huart->Instance == UART8)
    {
        UART_Manage_Object = &UART8_Manage_Object;
    }

    Frame_Header = __Frame_Header;
}

/**
 * @brief UART通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Powermeter::UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length)
{
    Data_Process(Length);
}

/**
 * @brief 数据处理过程
 * @param Length 接收数据长度
 */
void Class_Powermeter::Data_Process(uint16_t Length)
{
    // 数据处理过程
    Struct_Powermeter_UART_Data *tmp_buffer;

    tmp_buffer = (Struct_Powermeter_UART_Data *)UART_Manage_Object->Rx_Buffer;

    // 未通过头校验
    if (tmp_buffer->Frame_Header != Frame_Header)
    {
        return;
    }
    // 未通过校验和校验
    if (Math_Sum_8((uint8_t *)tmp_buffer + 1, sizeof(Struct_Powermeter_UART_Data) - 2) != tmp_buffer->Checksum)
    {
        return;
    }
    // 通过校验但帧不够长
    if (Length != sizeof(Struct_Powermeter_UART_Data))
    {
        return;
    }

    Data.Current = tmp_buffer->Current;
    Data.Voltage = tmp_buffer->Voltage;
    Data.Power = tmp_buffer->Power;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
