/**
 * @file dvc_supercap_24.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 超级电容
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿, 由于哨兵无超级电容, 因此未启用
 * @date 2024-03-09 1.1 适配新赛季超级电容
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_supercap_24.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 超级电容初始化
 *
 * @param hcan CAN编号
 * @param __CAN_Rx_ID CAN ID
 */
void Class_Supercap_24::Init(CAN_HandleTypeDef *hcan, uint16_t __CAN_Rx_ID, uint16_t __CAN_Tx_ID)
{
    if (hcan->Instance == CAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == CAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    CAN_Rx_ID = __CAN_Rx_ID;
    CAN_Tx_ID = __CAN_Tx_ID;
    Tx_Data = CAN_Supercap_Tx_Data;

    Power_Limit_Max = 55;
    Chassis_Buffer_Energy = 60;
    Power_Compensate_Max = 50;
    Supercap_Enable_Status = Supercap_24_Data_Status_ENABLE;
    Buffer_Energy_Loop_Status = Supercap_24_Data_Status_DISABLE;
}

/**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Supercap_24::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    // 滑动窗口, 判断超级电容是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测超级电容是否存活
 *
 */
void Class_Supercap_24::TIM_1000ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过超级电容数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Supercap_Status = Supercap_24_Status_DISABLE;
    }
    else
    {
        // 电机保持连接
        Supercap_Status = Supercap_24_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断打包数据到缓冲区的回调函数
 *
 */
void Class_Supercap_24::TIM_10ms_Send_PeriodElapsedCallback()
{
    Output();

    CAN_Send_Data(CAN_Manage_Object->CAN_Handler, CAN_Tx_ID, Tx_Data, 8);
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Supercap_24::Data_Process()
{
    Struct_Supercap_24_CAN_Rx_Data *tmp_buffer = (Struct_Supercap_24_CAN_Rx_Data *) CAN_Manage_Object->Rx_Buffer.Data;

    Rx_Data.Now_Energy = tmp_buffer->Now_Energy;
    Rx_Data.Energy_Status = tmp_buffer->Energy_Status;
    Rx_Data.Chassis_Power = (float) ((int16_t) Math_Endian_Reverse_16(&tmp_buffer->Chassis_Power, nullptr)) / 100.0f;
}

/**
 * @brief 超级电容数据输出到CAN总线发送缓冲区
 *
 */
void Class_Supercap_24::Output()
{
    Struct_Supercap_24_CAN_Tx_Data *tmp_buffer = (Struct_Supercap_24_CAN_Tx_Data *) Tx_Data;

    tmp_buffer->Power_Limit_Max = Power_Limit_Max;
    tmp_buffer->Chassis_Buffer_Energy = Chassis_Buffer_Energy;
    tmp_buffer->Power_Compensate_Max = Power_Compensate_Max;
    tmp_buffer->Supercap_Enable_Status_Enum = Supercap_Enable_Status;
    tmp_buffer->Buffer_Energy_Loop_Status_Enum = Buffer_Energy_Loop_Status;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
