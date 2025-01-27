/**
 * @file dvc_manifold.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 视觉Manifold
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-01-18 1.1 新协议
 * @date 2024-07-11 1.2 新协议
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_manifold.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 视觉Manifold初始化
 *
 * @param huart 指定的UART
 * @param __Frame_Rear 数据包头标
 */
void Class_Manifold::Init(UART_HandleTypeDef *huart, uint16_t __Frame_Header)
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
    Tx_Data.Frame_Header = __Frame_Header;
}

/**
 * @brief UART通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Manifold::UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length)
{
    //滑动窗口, 判断视觉Manifold是否在线
    Flag += 1;

    Data_Process(Length);
}

/**
 * @brief TIM定时器中断定期检测视觉Manifold是否存活
 *
 */
void Class_Manifold::TIM_1000ms_Alive_PeriodElapsedCallback()
{
    //判断该时间段内是否接收过视觉Manifold数据
    if (Flag == Pre_Flag)
    {
        // 视觉Manifold断开连接
        Manifold_Status = Manifold_Status_DISABLE;

        UART_Reinit(UART_Manage_Object->UART_Handler);
    }
    else
    {
        // 视觉Manifold保持连接
        Manifold_Status = Manifold_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数
 *
 */
void Class_Manifold::TIM_10ms_Send_PeriodElapsedCallback()
{
    Output();

    UART_Send_Data(UART_Manage_Object->UART_Handler, UART_Manage_Object->Tx_Buffer, 12);
}

/**
 * @brief 数据处理过程
 *
 * @param Rx_Data 接收的数据
 */
void Class_Manifold::Data_Process(uint16_t Length)
{
    // 数据处理过程
    Struct_Manifold_UART_Rx_Data *tmp_buffer = (Struct_Manifold_UART_Rx_Data *) UART_Manage_Object->Rx_Buffer;

    // 未通过头校验
    if (tmp_buffer->Frame_Header != Frame_Header)
    {
        return;
    }

    Rx_Data.Shoot_Flag = tmp_buffer->Shoot_Flag;

    // pitch角度增量
    if(isnormal(tmp_buffer->Gimbal_Pitch_Angle_Increment) == true)
    {
        Rx_Data.Gimbal_Pitch_Angle_Increment = tmp_buffer->Gimbal_Pitch_Angle_Increment + Gimbal_Pitch_Angle_Offset;
        if(Rx_Data.Gimbal_Pitch_Angle_Increment > Gimbal_Pitch_Angle_Increment_Max)
        {
            Rx_Data.Gimbal_Pitch_Angle_Increment = Gimbal_Pitch_Angle_Increment_Max;
        }
        else if(Rx_Data.Gimbal_Pitch_Angle_Increment < -Gimbal_Pitch_Angle_Increment_Max)
        {
            Rx_Data.Gimbal_Pitch_Angle_Increment = -Gimbal_Pitch_Angle_Increment_Max;
        }
    }
    else
    {
        Rx_Data.Gimbal_Pitch_Angle_Increment = 0.0f;
    }
    // yaw角度增量
    if(isnormal(tmp_buffer->Gimbal_Yaw_Angle_Increment) == true)
    {
        Rx_Data.Gimbal_Yaw_Angle_Increment = tmp_buffer->Gimbal_Yaw_Angle_Increment + Gimbal_Yaw_Angle_Offset;
        if(Rx_Data.Gimbal_Yaw_Angle_Increment > Gimbal_Yaw_Angle_Increment_Max)
        {
            Rx_Data.Gimbal_Yaw_Angle_Increment = Gimbal_Yaw_Angle_Increment_Max;
        }
        else if(Rx_Data.Gimbal_Yaw_Angle_Increment < -Gimbal_Yaw_Angle_Increment_Max)
        {
            Rx_Data.Gimbal_Yaw_Angle_Increment = -Gimbal_Yaw_Angle_Increment_Max;
        }
    }
    else
    {
        Rx_Data.Gimbal_Yaw_Angle_Increment = 0.0f;
    }
    // pitch角速度前馈
    if(isnormal(tmp_buffer->Gimbal_Pitch_Omega_FeedForward) == true)
    {
        Rx_Data.Gimbal_Pitch_Omega_FeedForward = tmp_buffer->Gimbal_Pitch_Omega_FeedForward;
    }
    else
    {
        Rx_Data.Gimbal_Pitch_Omega_FeedForward = 0.0f;
    }
    // yaw角速度前馈
    if(isnormal(tmp_buffer->Gimbal_Yaw_Omega_FeedForward) == true)
    {
        Rx_Data.Gimbal_Yaw_Omega_FeedForward = tmp_buffer->Gimbal_Yaw_Omega_FeedForward;
    }
    else
    {
        Rx_Data.Gimbal_Yaw_Omega_FeedForward = 0.0f;
    }

    Rx_Data.Enemy_ID = tmp_buffer->Enemy_ID;
    Rx_Data.Confidence_Level = tmp_buffer->Confidence_Level;

    Target_Gimbal_Yaw = Now_Gimbal_Yaw + Rx_Data.Gimbal_Yaw_Angle_Increment;
    Target_Gimbal_Pitch = Now_Gimbal_Pitch + Rx_Data.Gimbal_Pitch_Angle_Increment;
}

/**
 * @brief 视觉Manifold数据输出到UART发送缓冲区
 *
 */
void Class_Manifold::Output()
{
    Struct_Manifold_UART_Tx_Data *tmp_buffer = (Struct_Manifold_UART_Tx_Data *) UART_Manage_Object->Tx_Buffer;

    tmp_buffer->Frame_Header = Tx_Data.Frame_Header;
    tmp_buffer->Aiming_Priority = Tx_Data.Aiming_Priority;
    tmp_buffer->Velocity_X = Tx_Data.Velocity_X;
    tmp_buffer->Velocity_Y = Tx_Data.Velocity_Y;
    tmp_buffer->Enemy_Color = Tx_Data.Enemy_Color;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
