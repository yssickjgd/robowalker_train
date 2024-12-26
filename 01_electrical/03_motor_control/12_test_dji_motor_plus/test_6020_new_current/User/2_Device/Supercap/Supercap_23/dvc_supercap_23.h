/**
 * @file dvc_supercap_23.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 超级电容
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿, 由于哨兵无超级电容, 因此未启用
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

#ifndef DVC_SUPERCAP_23_H
#define DVC_SUPERCAP_23_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/1_Driver/CAN/drv_can.h"
#include "1_Middleware/1_Driver/Math/drv_math.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 超级电容状态
 *
 */
enum Enum_Supercap_23_Status
{
    Supercap_23_Status_DISABLE = 0,
    Supercap_23_Status_ENABLE,
};

/**
 * @brief 超级电容给控制板的源数据
 *
 */
struct Struct_Supercap_23_CAN_Rx_Data
{
    // 当前能量值
    int16_t Now_Energy_Reverse;
    // 当前功率
    int16_t Chassis_Power_Reverse;
} __attribute__((packed));

/**
 * @brief 超级电容给控制板的源数据
 *
 */
struct Struct_Supercap_23_Rx_Data
{
    // 当前能量值
    int16_t Now_Energy;
    // 当前功率
    float Chassis_Power;
} __attribute__((packed));

/**
 * @brief 控制板给超级电容的源数据
 *
 */
struct Struct_Supercap_23_CAN_Tx_Data
{
    uint16_t Power_Limit_Max;
} __attribute__((packed));

/**
 * @brief Specialized, 超级电容
 * 
 */
class Class_Supercap_23
{
public:
    void Init(CAN_HandleTypeDef *hcan, uint16_t __CAN_Rx_ID = 0x210, uint16_t __CAN_Tx_ID = 0x220);

    inline float Get_Power_Threshold();

    inline Enum_Supercap_23_Status Get_Status();

    inline uint16_t Get_Now_Energy();

    inline float Get_Chassis_Power();

    inline void Set_Power_Limit_Max(uint16_t __Power_Limit_Max);

    void CAN_RxCpltCallback(uint8_t *Rx_Data);

    void TIM_1000ms_Alive_PeriodElapsedCallback();

    void TIM_10ms_Send_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;
    // 数据绑定的CAN ID, 切记避开0x201~0x20b
    // 收数据默认0x220
    uint16_t CAN_Rx_ID;
    // 发数据默认0x210
    uint16_t CAN_Tx_ID;
    // 发送缓存区
    uint8_t *Tx_Data;

    // 常量

    // 过放保护能量阈值
    float Power_Threshold = 300.0f;

    // 内部变量

    // 当前时刻的超级电容接收flag
    uint32_t Flag = 0;
    // 前一时刻的超级电容接收flag
    uint32_t Pre_Flag = 0;

    // 读变量

    // 超级电容状态
    Enum_Supercap_23_Status Supercap_Status = Supercap_23_Status_DISABLE;
    // 超级电容对外接口信息
    Struct_Supercap_23_Rx_Data Rx_Data;

    // 写变量

    // 比赛规则限制的功率
    uint16_t Power_Limit_Max;

    // 读写变量

    // 内部函数

    void Data_Process();

    void Output();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取过放保护能量阈值
 *
 * @return float 过放保护能量阈值
 */
inline float Class_Supercap_23::Get_Power_Threshold()
{
    return (Power_Threshold);
}

/**
 * @brief 获取超级电容在线状态
 *
 * @return uint8_t 超级电容在线状态
 */
inline Enum_Supercap_23_Status Class_Supercap_23::Get_Status()
{
    return (Supercap_Status);
}

/**
 * @brief 获取当前能量值
 *
 * @return uint16_t 当前能量值
 */
inline uint16_t Class_Supercap_23::Get_Now_Energy()
{
    return (Rx_Data.Now_Energy);
}


/**
 * @brief 获取当前功率
 *
 * @return float 当前功率
 */
inline float Class_Supercap_23::Get_Chassis_Power()
{
    return (Rx_Data.Chassis_Power);
}

/**
 * @brief 设定比赛规则限制的功率
 *
 * @param __Power_Limit_Max 比赛规则限制的功率
 */
inline void Class_Supercap_23::Set_Power_Limit_Max(uint16_t __Power_Limit_Max)
{
    Power_Limit_Max = __Power_Limit_Max;
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
