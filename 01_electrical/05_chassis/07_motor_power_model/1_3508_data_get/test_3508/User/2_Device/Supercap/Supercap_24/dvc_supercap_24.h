/**
 * @file dvc_supercap_24.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 超级电容
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿, 由于哨兵无超级电容, 因此未启用
 * @date 2024-03-09 1.1 适配新赛季超级电容
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

#ifndef DVC_SUPERCAP_24_H
#define DVC_SUPERCAP_24_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/1_Driver/CAN/drv_can.h"
#include "1_Middleware/1_Driver/Math/drv_math.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 超级电容状态
 *
 */
enum Enum_RM24_Supercap_Status
{
    Supercap_24_Status_DISABLE = 0,
    Supercap_24_Status_ENABLE,
};

/**
 * @brief 各种子模块使能状态
 *
 */
enum Enum_Supercap_24_Data_Status : uint8_t
{
    Supercap_24_Data_Status_DISABLE = 0,
    Supercap_24_Data_Status_ENABLE,
};

/**
 * @brief 超级电容储能状态
 *
 */
enum Enum_Supercap_24_Energy_Status : uint8_t
{
    Supercap_24_Energy_Status_DISABLE = 0,
    Supercap_24_Energy_Status_WARNING,
    Supercap_24_Energy_Status_NORMAL,
};

/**
 * @brief 超级电容给控制板的源数据
 *
 */
struct Struct_Supercap_24_CAN_Rx_Data
{
    // 当前能量值
    int32_t Now_Energy;
    // 当前状态, 指示是否可用
    Enum_Supercap_24_Energy_Status Energy_Status;
    // 底盘功率100倍
    int16_t Chassis_Power;
} __attribute__((packed));

/**
 * @brief 超级电容给控制板的源数据
 *
 */
struct Struct_Supercap_24_Rx_Data
{
    // 当前能量值
    int32_t Now_Energy;
    // 当前状态, 指示是否可用
    Enum_Supercap_24_Energy_Status Energy_Status;
    // 底盘功率
    float Chassis_Power;
} __attribute__((packed));

/**
 * @brief 控制板给超级电容的源数据
 *
 */
struct Struct_Supercap_24_CAN_Tx_Data
{
    uint16_t Power_Limit_Max;
    uint16_t Chassis_Buffer_Energy;
    uint16_t Power_Compensate_Max;
    uint8_t Supercap_Enable_Status_Enum : 1;
    uint8_t Buffer_Energy_Loop_Status_Enum : 1;
    uint8_t Reserved_1 : 6;
    uint8_t Reserved_2;
} __attribute__((packed));

/**
 * @brief Specialized, 超级电容
 * 
 */
class Class_Supercap_24
{
public:
    void Init(CAN_HandleTypeDef *hcan, uint16_t __CAN_Rx_ID = 0x030, uint16_t __CAN_Tx_ID = 0x02f);

    inline Enum_RM24_Supercap_Status Get_Status();

    inline uint16_t Get_Now_Energy();

    inline Enum_Supercap_24_Energy_Status Get_Energy_Status();

    inline float Get_Chassis_Power();

    inline void Set_Power_Limit_Max(uint16_t __Power_Limit_Max);

    inline void Set_Chassis_Buffer_Energy(uint16_t __Chassis_Buffer_Energy);

    inline void Set_Power_Compensate_Max(uint16_t __Power_Compensate_Max);

    inline void Set_Supercap_Enable_Status(Enum_Supercap_24_Data_Status __Supercap_Enable_Status);

    inline void Set_Buffer_Energy_Loop_Status(Enum_Supercap_24_Data_Status __Buffer_Energy_Loop_Status);

    void CAN_RxCpltCallback(uint8_t *Rx_Data);

    void TIM_1000ms_Alive_PeriodElapsedCallback();

    void TIM_10ms_Send_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;
    // 数据绑定的CAN ID, 切记避开0x201~0x20b
    // 收数据默认0x030
    uint16_t CAN_Rx_ID;
    // 发数据默认0x02f
    uint16_t CAN_Tx_ID;
    // 发送缓存区
    uint8_t *Tx_Data;

    // 常量

    // 内部变量

    // 当前时刻的超级电容接收flag
    uint32_t Flag = 0;
    // 前一时刻的超级电容接收flag
    uint32_t Pre_Flag = 0;

    // 读变量

    // 超级电容状态
    Enum_RM24_Supercap_Status Supercap_Status = Supercap_24_Status_DISABLE;
    // 超级电容对外接口信息
    Struct_Supercap_24_Rx_Data Rx_Data;

    // 写变量

    // 比赛规则限制的功率
    uint16_t Power_Limit_Max = 40.0f;
    // 底盘缓冲能量
    uint16_t Chassis_Buffer_Energy = 0.0f;
    // 超级电容可以补偿的最大功率值
    uint16_t Power_Compensate_Max = 400.0f;

    // 是否开启超级电容
    Enum_Supercap_24_Data_Status Supercap_Enable_Status = Supercap_24_Data_Status_ENABLE;
    // 是否开启能量缓冲环
    Enum_Supercap_24_Data_Status Buffer_Energy_Loop_Status = Supercap_24_Data_Status_DISABLE;

    // 读写变量

    // 内部函数

    void Data_Process();

    void Output();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取超级电容在线状态
 *
 * @return uint8_t 超级电容在线状态
 */
inline Enum_RM24_Supercap_Status Class_Supercap_24::Get_Status()
{
    return (Supercap_Status);
}

/**
 * @brief 获取当前能量值
 *
 * @return uint8_t 当前能量值
 */
inline uint16_t Class_Supercap_24::Get_Now_Energy()
{
    return (Rx_Data.Now_Energy);
}

/**
 * @brief 获取当前状态, 指示是否可用
 *
 * @return Enum_Supercap_24_Energy_Status 当前状态, 指示是否可用
 */
inline Enum_Supercap_24_Energy_Status Class_Supercap_24::Get_Energy_Status()
{
    return (Rx_Data.Energy_Status);
}

/**
 * @brief 获取底盘功率
 *
 * @return float 底盘功率
 */
inline float Class_Supercap_24::Get_Chassis_Power()
{
    return (Rx_Data.Chassis_Power);
}

/**
 * @brief 设定比赛规则限制的功率
 *
 * @param __Power_Limit_Max 比赛规则限制的功率
 */
inline void Class_Supercap_24::Set_Power_Limit_Max(uint16_t __Power_Limit_Max)
{
    Power_Limit_Max = __Power_Limit_Max;
}

/**
 * @brief 设定底盘缓冲能量
 *
 * @param __Chassis_Buffer_Energy 底盘缓冲能量
 */
inline void Class_Supercap_24::Set_Chassis_Buffer_Energy(uint16_t __Chassis_Buffer_Energy)
{
    Chassis_Buffer_Energy = __Chassis_Buffer_Energy;
}

/**
 * @brief 设定超级电容可以补偿的最大功率值
 *
 * @param __Power_Compensate_Max 超级电容可以补偿的最大功率值
 */
inline void Class_Supercap_24::Set_Power_Compensate_Max(uint16_t __Power_Compensate_Max)
{
    Power_Compensate_Max = __Power_Compensate_Max;
}

/**
 * @brief 设定是否开启超级电容
 *
 * @param __Supercap_Enable_Status 是否开启超级电容
 */
inline void Class_Supercap_24::Set_Supercap_Enable_Status(Enum_Supercap_24_Data_Status __Supercap_Enable_Status)
{
    Supercap_Enable_Status = __Supercap_Enable_Status;
}

/**
 * @brief 是否开启能量缓冲环
 *
 * @param __Buffer_Energy_Loop_Status 是否开启能量缓冲环
 */
inline void Class_Supercap_24::Set_Buffer_Energy_Loop_Status(Enum_Supercap_24_Data_Status __Buffer_Energy_Loop_Status)
{
    Buffer_Energy_Loop_Status = __Buffer_Energy_Loop_Status;
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
