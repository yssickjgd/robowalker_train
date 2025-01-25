/**
 * @file dvc_powermeter.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 自制功率计
 * @version 0.1
 * @date 2025-01-25 0.1 新建
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef DVC_POWERMETER_H
#define DVC_POWERMETER_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/1_Driver/UART/drv_uart.h"
#include "1_Middleware/1_Driver/Math/drv_math.h"

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
struct Struct_Powermeter_UART_Data
{
    uint8_t Frame_Header;
    float Current;
    float Voltage;
    float Power;
    uint8_t Checksum;
} __attribute__((packed));

/**
 * @brief 功率计经过处理的数据
 *
 */
struct Struct_Powermeter_Data
{
    float Current;
    float Voltage;
    float Power;
};

/**
 * @brief Specialized, 功率计
 *
 */
class Class_Powermeter
{
public:
    void Init(UART_HandleTypeDef *huart, uint8_t __Frame_Header = 0xab);

    inline Enum_Powermeter_Status Get_Status();

    inline float Get_Current();

    inline float Get_Voltage();

    inline float Get_Power();

    void UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length);

protected:
    // 初始化相关常量

    // 绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;
    // 数据包头标
    uint8_t Frame_Header;

    // 常量

    // 内部变量

    // 读变量

    // 功率计状态
    Enum_Powermeter_Status Powermeter_Status = Powermeter_Status_DISABLE;
    // 功率计对外接口信息
    Struct_Powermeter_Data Data;

    // 写变量

    // 读写变量

    // 内部函数

    void Data_Process(uint16_t Length);
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取功率计在线状态
 *
 * @return Enum_Manifold_Status Manifold在线状态
 */
inline Enum_Powermeter_Status Class_Powermeter::Get_Status()
{
    return (Powermeter_Status);
}

/**
 * @brief 获取功率计电流值
 *
 * @return float 功率计电流值
 */
inline float Class_Powermeter::Get_Current()
{
    return (Data.Current);
}

/**
 * @brief 获取功率计电压值
 *
 * @return float 功率计电压值
 */
inline float Class_Powermeter::Get_Voltage()
{
    return (Data.Voltage);
}

/**
 * @brief 获取功率计功率值
 *
 * @return float 功率计功率值
 */
inline float Class_Powermeter::Get_Power()
{
    return (Data.Power);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
