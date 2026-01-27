/**
 * @file dvc_powermeter.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 功率计
 * @version 0.1
 * @date 2026-01-26 0.1 新建
 *
 * @copyright USTC-RoboWalker (c) 2026
 *
 */

#ifndef DVC_POWERMETER_H
#define DVC_POWERMETER_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Driver/UART/drv_uart.h"
#include "1_Middleware/Driver/CAN/drv_can.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 功率计状态
 *
 */
enum Enum_Powermeter_Status
{
    Powermeter_Status_DISABLE = 0,
    Powermeter_Status_ENABLE,
};

/**
 * @brief 功率计源数据
 *
 */
struct Struct_Powermeter_UART_Rx_Data
{
    uint8_t Frame_Header;
    float Current;
    float Voltage;
    float Power;
    uint8_t Checksum;
} __attribute__((packed));

/**
 * @brief 功率计源数据
 *
 */
struct Struct_Powermeter_CAN_Rx_Data
{
    float Current;
    float Voltage;
} __attribute__((packed));

/**
 * @brief 功率计经过处理的数据
 *
 */
struct Struct_Powermeter_Rx_Data
{
    float Current;
    float Voltage;
    float Power;
};

/**
 * @brief Reusable, 功率计
 *
 */
class Class_Powermeter_UART
{
public:
    void Init(const UART_HandleTypeDef *huart, const uint8_t &__Frame_Header = 0xab);

    inline Enum_Powermeter_Status Get_Status() const;

    inline float Get_Current() const;

    inline float Get_Voltage() const;

    inline float Get_Power() const;

    void UART_RxCpltCallback(const uint8_t *Rx_Data, const uint16_t &Length);

    void TIM_100ms_Alive_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;
    // 数据包头标
    uint8_t Frame_Header;

    // 常量

    // 内部变量

    // 当前时刻的功率计接收flag
    uint32_t Flag = 0;
    // 前一时刻的功率计接收flag
    uint32_t Pre_Flag = 0;

    // 读变量

    // 功率计状态
    Enum_Powermeter_Status Powermeter_Status = Powermeter_Status_DISABLE;
    // 功率计接收数据
    Struct_Powermeter_Rx_Data Rx_Data;

    // 写变量

    // 读写变量

    // 内部函数

    void Data_Process();
};

/**
 * @brief Reusable, 功率计
 *
*/
class Class_Powermeter_CAN
{
public:
    void Init(const FDCAN_HandleTypeDef *hcan);

    inline Enum_Powermeter_Status Get_Status() const;

    inline float Get_Current() const;

    inline float Get_Voltage() const;

    inline float Get_Power() const;

    void CAN_RxCpltCallback();

    void TIM_100ms_Alive_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;

    // 常量

    // 内部变量

    // 当前时刻的功率计接收flag
    uint32_t Flag = 0;
    // 前一时刻的功率计接收flag
    uint32_t Pre_Flag = 0;

    // 读变量

    // 功率计状态
    Enum_Powermeter_Status Powermeter_Status = Powermeter_Status_DISABLE;
    // 功率计接收数据
    Struct_Powermeter_Rx_Data Rx_Data;

    // 写变量

    // 读写变量

    // 内部函数

    void Data_Process();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取功率计状态
 *
 * @return Enum_Powermeter_Status 功率计状态
 */
inline Enum_Powermeter_Status Class_Powermeter_UART::Get_Status() const
{
    return (Powermeter_Status);
}

/**
 * @brief 获取当前的电流
 *
 * @return float 当前的电流
 */
inline float Class_Powermeter_UART::Get_Current() const
{
    return (Rx_Data.Current);
}

/**
 * @brief 获取当前的电压
 *
 * @return float 当前的电压
 */
inline float Class_Powermeter_UART::Get_Voltage() const
{
    return (Rx_Data.Voltage);
}

/**
 * @brief 获取当前的功率
 *
 * @return float 当前的功率
 */
inline float Class_Powermeter_UART::Get_Power() const
{
    return (Rx_Data.Power);
}

/**
 * @brief 获取功率计状态
 *
 * @return Enum_Powermeter_Status 功率计状态
 */
inline Enum_Powermeter_Status Class_Powermeter_CAN::Get_Status() const
{
    return (Powermeter_Status);
}

/**
 * @brief 获取当前的电流
 *
 * @return float 当前的电流
 */
inline float Class_Powermeter_CAN::Get_Current() const
{
    return (Rx_Data.Current);
}

/**
 * @brief 获取当前的电压
 *
 * @return float 当前的电压
 */
inline float Class_Powermeter_CAN::Get_Voltage() const
{
    return (Rx_Data.Voltage);
}

/**
 * @brief 获取当前的功率
 *
 * @return float 当前的功率
 */
inline float Class_Powermeter_CAN::Get_Power() const
{
    return (Rx_Data.Power);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/