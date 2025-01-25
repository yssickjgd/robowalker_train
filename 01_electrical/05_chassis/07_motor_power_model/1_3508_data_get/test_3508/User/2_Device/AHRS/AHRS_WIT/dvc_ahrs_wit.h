/**
 * @file dvc_ahrs_wit.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief WIT姿态传感器
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-05-23 1.1 粘包处理
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

#ifndef DVC_AHRS_WIT_H
#define DVC_AHRS_WIT_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/1_Driver/Math/drv_math.h"
#include "1_Middleware/1_Driver/UART/drv_uart.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief WIT姿态传感器状态
 *
 */
enum Enum_AHRS_WIT_Status
{
    AHRS_WIT_Status_DISABLE = 0,
    AHRS_WIT_Status_ENABLE,
};

/**
 * @brief WIT姿态传感器接收数据类型
 *
 */
enum Enum_AHRS_WIT_Data_Type : uint8_t
{
    AHRS_WIT_Data_Type_TIME = 0x50,
    AHRS_WIT_Data_Type_ACCELERATE,
    AHRS_WIT_Data_Type_OMEGA,
    AHRS_WIT_Data_Type_ANGLE,
    AHRS_WIT_Data_Type_MAGNETIC,
    AHRS_WIT_Data_Type_PORT,
    AHRS_WIT_Data_Type_PRESSURE_ALTITUDE,
    AHRS_WIT_Data_Type_LONGITUDE_LATITUDE,
    AHRS_WIT_Data_Type_GROUND_SPEED,
    AHRS_WIT_Data_Type_QUATERNION,
    AHRS_WIT_Data_Type_GPS,
    AHRS_WIT_Data_Type_REGISTER,
};

/**
 * @brief WIT姿态传感器源数据
 * 
 */
struct Struct_AHRS_WIT_UART_Data
{
    uint8_t Frame_Header;
    Enum_AHRS_WIT_Data_Type Data_Type;
    uint8_t Data[8];
    uint8_t Checksum;
}__attribute__((packed));

/**
 * @brief WIT姿态传感器经过处理的数据, 加速度
 * 
 */
struct Struct_AHRS_WIT_Data_Accelerate
{
    float Accelerate_X;
    float Accelerate_Y;
    float Accelerate_Z;
    float Temperature;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 角速度
 * 
 */
struct Struct_AHRS_WIT_Data_Omega
{
    float Omega_X;
    float Omega_Y;
    float Omega_Z;
    float Voltage;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 角度
 * 
 */
struct Struct_AHRS_WIT_Data_Angle
{
    float Angle_Roll;
    float Angle_Pitch;
    float Angle_Yaw;
    uint16_t Version;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 四元数
 *
 */
struct Struct_AHRS_WIT_Data_Quaternion
{
    float Q_0;
    float Q_1;
    float Q_2;
    float Q_3;
};

/**
 * @brief Generic, WIT姿态传感器
 * 
 */
class Class_AHRS_WIT
{
public:
    void Init(UART_HandleTypeDef *huart, uint16_t __Frame_Header = 0x55);

    inline Enum_AHRS_WIT_Status Get_Status();

    inline float Get_Accelerate_X();

    inline float Get_Accelerate_Y();

    inline float Get_Accelerate_Z();

    inline float Get_Omega_X();

    inline float Get_Omega_Y();

    inline float Get_Omega_Z();

    inline float Get_Angle_Roll();

    inline float Get_Angle_Pitch();

    inline float Get_Angle_Yaw();

    inline float Get_Q_0();

    inline float Get_Q_1();

    inline float Get_Q_2();

    inline float Get_Q_3();

    void UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length);

    void TIM_100ms_Alive_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;
    // 数据包头标
    uint8_t Frame_Header;

    // 常量

    // 内部变量

    // 当前时刻的WIT姿态传感器接收flag
    uint32_t Flag = 0;
    // 前一时刻的WIT姿态传感器接收flag
    uint32_t Pre_Flag = 0;

    // 读变量

    // WIT姿态传感器状态
    Enum_AHRS_WIT_Status WIT_AHRS_Status = AHRS_WIT_Status_DISABLE;

    // WIT姿态传感器对外接口信息
    Struct_AHRS_WIT_Data_Accelerate Data_Accelerate;
    Struct_AHRS_WIT_Data_Omega Data_Omega;
    Struct_AHRS_WIT_Data_Angle Data_Angle;
    Struct_AHRS_WIT_Data_Quaternion Data_Quaternion;

    // 写变量

    // 读写变量

    // 内部函数

    void Data_Process(uint16_t Length);
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取WIT姿态传感器状态
 * 
 * @return Enum_WIT_Status WIT姿态传感器状态
 */
inline Enum_AHRS_WIT_Status Class_AHRS_WIT::Get_Status()
{
    return (WIT_AHRS_Status);
}

/**
 * @brief 获取加速度x
 * 
 * @return float 加速度x
 */
inline float Class_AHRS_WIT::Get_Accelerate_X()
{
    return (Data_Accelerate.Accelerate_X);
}

/**
 * @brief 获取加速度y
 * 
 * @return float 加速度y
 */
inline float Class_AHRS_WIT::Get_Accelerate_Y()
{
    return (Data_Accelerate.Accelerate_Y);
}

/**
 * @brief 获取加速度z
 * 
 * @return float 加速度z
 */
inline float Class_AHRS_WIT::Get_Accelerate_Z()
{
    return (Data_Accelerate.Accelerate_Z);
}

/**
 * @brief 获取角速度x
 * 
 * @return float 角速度x
 */
inline float Class_AHRS_WIT::Get_Omega_X()
{
    return (Data_Omega.Omega_X);
}

/**
 * @brief 获取角速度y
 * 
 * @return float 角速度y
 */
inline float Class_AHRS_WIT::Get_Omega_Y()
{
    return (Data_Omega.Omega_Y);
}

/**
 * @brief 获取角速度z
 * 
 * @return float 角速度z
 */
inline float Class_AHRS_WIT::Get_Omega_Z()
{
    return (Data_Omega.Omega_Z);
}

/**
 * @brief 获取滚转角
 * 
 * @return float 滚转角
 */
inline float Class_AHRS_WIT::Get_Angle_Roll()
{
    return (Data_Angle.Angle_Roll);
}

/**
 * @brief 获取俯仰角
 * 
 * @return float 俯仰角
 */
inline float Class_AHRS_WIT::Get_Angle_Pitch()
{
    return (Data_Angle.Angle_Pitch);
}

/**
 * @brief 获取航向角
 * 
 * @return float 航向角
 */
inline float Class_AHRS_WIT::Get_Angle_Yaw()
{
    return (Data_Angle.Angle_Yaw);
}

/**
 * @brief 获取四元数0
 * 
 * @return float 四元数0
 */
inline float Class_AHRS_WIT::Get_Q_0()
{
    return (Data_Quaternion.Q_0);
}

/**
 * @brief 获取四元数1
 * 
 * @return float 四元数1
 */
inline float Class_AHRS_WIT::Get_Q_1()
{
    return (Data_Quaternion.Q_1);
}

/**
 * @brief 获取四元数2
 * 
 * @return float 四元数2
 */
inline float Class_AHRS_WIT::Get_Q_2()
{
    return (Data_Quaternion.Q_2);
}

/**
 * @brief 获取四元数3
 * 
 * @return float 四元数3
 */
inline float Class_AHRS_WIT::Get_Q_3()
{
    return (Data_Quaternion.Q_3);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
