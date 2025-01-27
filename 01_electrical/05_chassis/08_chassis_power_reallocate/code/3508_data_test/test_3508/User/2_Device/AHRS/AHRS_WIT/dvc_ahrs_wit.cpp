/**
 * @file dvc_ahrs_wit.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief WIT姿态传感器
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-05-23 1.1 粘包处理
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_ahrs_wit.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief WIT姿态传感器初始化
 * 
 * @param __huart 指定的UART
 * @param __Frame_Header 数据包头标
 */
void Class_AHRS_WIT::Init(UART_HandleTypeDef *huart, uint16_t __Frame_Header)
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
void Class_AHRS_WIT::UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length)
{
    // 滑动窗口, 判断陀螺仪是否在线
    Flag += 1;
    Data_Process(Length);
}

/**
 * @brief TIM定时器中断定期检测WIT姿态传感器是否存活
 *
 */
void Class_AHRS_WIT::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过陀螺仪数据
    if (Flag == Pre_Flag)
    {
        // 陀螺仪断开连接
        WIT_AHRS_Status = AHRS_WIT_Status_DISABLE;

        UART_Reinit(UART_Manage_Object->UART_Handler);
    }
    else
    {
        // 陀螺仪保持连接
        WIT_AHRS_Status = AHRS_WIT_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief 数据处理过程
 *
 */
void Class_AHRS_WIT::Data_Process(uint16_t Length)
{
    // 数据处理过程
    Struct_AHRS_WIT_UART_Data *tmp_buffer;

    for (int i = 0; i < Length;)
    {
        tmp_buffer = (Struct_AHRS_WIT_UART_Data *) &UART_Manage_Object->Rx_Buffer[i];

        // 未通过头校验
        if (tmp_buffer->Frame_Header != Frame_Header)
        {
            i++;
            continue;
        }
        // 未通过校验和, 顺一位继续判断
        if (Math_Sum_8((uint8_t *) tmp_buffer, 10) != tmp_buffer->Checksum)
        {
            i++;
            continue;
        }
        // 通过校验但帧不够长
        if (i + 11 > Length)
        {
            break;
        }

        switch (tmp_buffer->Data_Type)
        {
        case (AHRS_WIT_Data_Type_ACCELERATE):
        {
            Data_Accelerate.Accelerate_X = (int16_t) (tmp_buffer->Data[0] | tmp_buffer->Data[1] << 8) / 32768.0f * 16.0f * 9.8f;

            Data_Accelerate.Accelerate_Y = (int16_t) (tmp_buffer->Data[2] | tmp_buffer->Data[3] << 8) / 32768.0f * 16.0f * 9.8f;

            Data_Accelerate.Accelerate_Z = (int16_t) (tmp_buffer->Data[4] | tmp_buffer->Data[5] << 8) / 32768.0f * 16.0f * 9.8f;

            Data_Accelerate.Temperature = (int16_t) (tmp_buffer->Data[6] | tmp_buffer->Data[7] << 8) / 100.0f + CELSIUS_TO_KELVIN;

            break;
        }
        case (AHRS_WIT_Data_Type_OMEGA):
        {
            Data_Omega.Omega_X = (int16_t) (tmp_buffer->Data[0] | tmp_buffer->Data[1] << 8) / 32768.0f * 2000.0f * DEG_TO_RAD;

            Data_Omega.Omega_Y = (int16_t) (tmp_buffer->Data[2] | tmp_buffer->Data[3] << 8) / 32768.0f * 2000.0f * DEG_TO_RAD;

            Data_Omega.Omega_Z = (int16_t) (tmp_buffer->Data[4] | tmp_buffer->Data[5] << 8) / 32768.0f * 2000.0f * DEG_TO_RAD;

            Data_Omega.Voltage = (int16_t) (tmp_buffer->Data[6] | tmp_buffer->Data[7] << 8) / 100.0f;

            break;
        }
        case (AHRS_WIT_Data_Type_ANGLE):
        {
            Data_Angle.Angle_Roll = (int16_t) (tmp_buffer->Data[0] | tmp_buffer->Data[1] << 8) / 32768.0f * PI;

            Data_Angle.Angle_Pitch = (int16_t) (tmp_buffer->Data[2] | tmp_buffer->Data[3] << 8) / 32768.0f * PI;

            Data_Angle.Angle_Yaw = (int16_t) (tmp_buffer->Data[4] | tmp_buffer->Data[5] << 8) / 32768.0f * PI;

            Data_Angle.Version = (uint16_t) tmp_buffer->Data[6] | tmp_buffer->Data[7] << 8;

            break;
        }
        case (AHRS_WIT_Data_Type_QUATERNION):
        {
            Data_Quaternion.Q_0 = (int16_t) (tmp_buffer->Data[0] | tmp_buffer->Data[1] << 8) / 32768.0f;

            Data_Quaternion.Q_1 = (int16_t) (tmp_buffer->Data[2] | tmp_buffer->Data[3] << 8) / 32768.0f;

            Data_Quaternion.Q_2 = (int16_t) (tmp_buffer->Data[4] | tmp_buffer->Data[5] << 8) / 32768.0f;

            Data_Quaternion.Q_3 = (int16_t) (tmp_buffer->Data[6] | tmp_buffer->Data[7] << 8) / 32768.0f;

            break;
        }
        }

        // 缓冲区直接推移
        i += 11;
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
