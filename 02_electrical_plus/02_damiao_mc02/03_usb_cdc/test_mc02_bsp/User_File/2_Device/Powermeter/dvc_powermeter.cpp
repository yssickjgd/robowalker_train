/**
 * @file dvc_vofa.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief vofa
 * @version 0.1
 * @date 2025-09-22 0.1 新建
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
 * @brief 功率计UART初始化
 *
 * @param huart 指定的UART
 * @param __Frame_Header 帧头标识符
 */
void Class_Powermeter_UART::Init(const UART_HandleTypeDef *huart, const uint8_t &__Frame_Header)
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
 * @param Length 接收数据长度
 */
void Class_Powermeter_UART::UART_RxCpltCallback(const uint8_t *Rx_Data, const uint16_t &Length)
{
    // 滑动窗口, 判断功率计是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测功率计是否存活
 *
 */
void Class_Powermeter_UART::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否有接收过功率计数据
    if (Flag == Pre_Flag)
    {
        // 功率计掉线处理
        Powermeter_Status = Powermeter_Status_DISABLE;
        Rx_Data.Current = 0.0f;
        Rx_Data.Voltage = 0.0f;
        Rx_Data.Power = 0.0f;
    }
    else
    {
        // 功率计在线
        Powermeter_Status = Powermeter_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Powermeter_UART::Data_Process()
{
    Struct_Powermeter_UART_Rx_Data *tmp_buffer = (Struct_Powermeter_UART_Rx_Data *) UART_Manage_Object->Rx_Buffer_Ready;

    // 帧头错误直接返回
    if (tmp_buffer->Frame_Header != Frame_Header)
    {
        return;
    }

    // 数据处理过程
    memcpy(&Rx_Data, &((uint8_t *)tmp_buffer)[1], sizeof(Struct_Powermeter_Rx_Data));
}

/**
 * @brief 功率计CAN初始化
 *
 * @param hcan 指定的CAN
 */
void Class_Powermeter_CAN::Init(const FDCAN_HandleTypeDef *hcan)
{
    if (hcan->Instance == FDCAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == FDCAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    else if (hcan->Instance == FDCAN3)
    {
        CAN_Manage_Object = &CAN3_Manage_Object;
    }
}

/**
 * @brief CAN通信接收回调函数
 *
 */
void Class_Powermeter_CAN::CAN_RxCpltCallback()
{
    // 滑动窗口, 判断功率计是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测功率计是否存活
 *
 */
void Class_Powermeter_CAN::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否有接收过功率计数据
    if (Flag == Pre_Flag)
    {
        // 功率计掉线处理
        Powermeter_Status = Powermeter_Status_DISABLE;
        Rx_Data.Current = 0.0f;
        Rx_Data.Voltage = 0.0f;
        Rx_Data.Power = 0.0f;
    }
    else
    {
        // 功率计在线
        Powermeter_Status = Powermeter_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Powermeter_CAN::Data_Process()
{
    Struct_Powermeter_CAN_Rx_Data *tmp_buffer = (Struct_Powermeter_CAN_Rx_Data *) CAN_Manage_Object->Rx_Buffer;

    // 数据处理过程
    memcpy(&Rx_Data, tmp_buffer, sizeof(Struct_Powermeter_CAN_Rx_Data));
    Rx_Data.Power = Rx_Data.Current * Rx_Data.Voltage;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/