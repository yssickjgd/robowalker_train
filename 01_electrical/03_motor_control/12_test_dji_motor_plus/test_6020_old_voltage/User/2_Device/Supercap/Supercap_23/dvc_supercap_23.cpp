/**
 * @file dvc_supercap_23.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 超级电容
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿, 由于哨兵无超级电容, 因此未启用
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_supercap_23.h"

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
void Class_Supercap_23::Init(CAN_HandleTypeDef *hcan, uint16_t __CAN_Rx_ID, uint16_t __CAN_Tx_ID)
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

    Power_Limit_Max = 45;
}

/**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Supercap_23::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    // 滑动窗口, 判断超级电容是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测超级电容是否存活
 *
 */
void Class_Supercap_23::TIM_1000ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过超级电容数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Supercap_Status = Supercap_23_Status_DISABLE;
    }
    else
    {
        // 电机保持连接
        Supercap_Status = Supercap_23_Status_ENABLE;
    }

    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断打包数据到缓冲区的回调函数
 *
 */
void Class_Supercap_23::TIM_10ms_Send_PeriodElapsedCallback()
{
    Output();

    CAN_Send_Data(CAN_Manage_Object->CAN_Handler, CAN_Tx_ID, Tx_Data, 2);
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Supercap_23::Data_Process()
{
    int16_t tmp_energy, tmp_chassis_power;
    Struct_Supercap_23_CAN_Rx_Data *tmp_buffer = (Struct_Supercap_23_CAN_Rx_Data *) CAN_Manage_Object->Rx_Buffer.Data;

    Math_Endian_Reverse_16((void *) &tmp_buffer->Now_Energy_Reverse, &tmp_energy);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Chassis_Power_Reverse, &tmp_chassis_power);

    Rx_Data.Now_Energy = tmp_energy;
    Rx_Data.Chassis_Power = tmp_chassis_power / 100.0f;
}

/**
 * @brief 超级电容数据输出到CAN总线发送缓冲区
 *
 */
void Class_Supercap_23::Output()
{
    Struct_Supercap_23_CAN_Tx_Data *tmp_buffer = (Struct_Supercap_23_CAN_Tx_Data *) Tx_Data;

    Math_Endian_Reverse_16(&Power_Limit_Max, &tmp_buffer->Power_Limit_Max);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
