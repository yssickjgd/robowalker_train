/**
 * @file dvc_manifold.h
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

/**
 * @brief 坐标定义
 * 前x左y上z, 右手系决定旋转, ypr旋转顺序
 */

#ifndef DVC_MANIFOLD_H
#define DVC_MANIFOLD_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/1_Driver/Math/drv_math.h"
#include "1_Middleware/1_Driver/UART/drv_uart.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 视觉Manifold状态
 *
 */
enum Enum_Manifold_Status
{
    Manifold_Status_DISABLE = 0,
    Manifold_Status_ENABLE,
};

/**
 * @brief 视觉Manifold自家颜色
 *
 */
enum Enum_Manifold_Enemy_Color : uint8_t
{
    Manifold_Enemy_Color_RED = 0,
    Manifold_Enemy_Color_BLUE,
};

/**
 * @brief 敌方机器人ID
 *
 */
enum Enum_Manifold_Enemy_ID : uint8_t
{
    Manifold_Enemy_ID_NONE_0 = 0,
    Manifold_Enemy_ID_HERO_1,
    Manifold_Enemy_ID_ENGINEER_2,
    Manifold_Enemy_ID_INFANTRY_3,
    Manifold_Enemy_ID_INFANTRY_4,
    Manifold_Enemy_ID_INFANTRY_5,
    Manifold_Enemy_ID_SENTRY_7,
    Manifold_Enemy_ID_OUTPOST,
    Manifold_Enemy_ID_RUNE,
    Manifold_Enemy_ID_UNKNOWN,
};

/**
 * @brief 自瞄算法优先级
 *
 */
enum Enum_Manifold_Aiming_Priority : uint8_t
{
    Manifold_Aiming_Priority_ARMOR = 0,
    Manifold_Aiming_Priority_RUNE,
};

/**
 * @brief 视觉Manifold给控制板的源数据
 *
 */
struct Struct_Manifold_UART_Rx_Data
{
    uint16_t Frame_Header;
    uint8_t Reserved;
    uint8_t Shoot_Flag;
    float Gimbal_Pitch_Angle_Increment;
    float Gimbal_Yaw_Angle_Increment;
    float Gimbal_Pitch_Omega_FeedForward;
    float Gimbal_Yaw_Omega_FeedForward;
    Enum_Manifold_Enemy_ID Enemy_ID;
    uint16_t Confidence_Level;
}__attribute__((packed));

/**
 * @brief 控制板给视觉Manifold的源数据
 *
 */
struct Struct_Manifold_UART_Tx_Data
{
    uint16_t Frame_Header;
    Enum_Manifold_Enemy_Color Enemy_Color;
    Enum_Manifold_Aiming_Priority Aiming_Priority;
    float Velocity_X;
    float Velocity_Y;
}__attribute__((packed));

/**
 * @brief Specialized, 单独自瞄接口
 *
 */
class Class_Manifold
{
public:
    void Init(UART_HandleTypeDef *__huart, uint16_t __Frame_Header = 0xcdab);

    inline Enum_Manifold_Status Get_Status();

    inline uint8_t Get_Shoot_Flag();

    inline float Get_Gimbal_Pitch_Angle_Increment();

    inline float Get_Gimbal_Yaw_Angle_Increment();

    inline float Get_Gimbal_Pitch_Omega_FeedForward();

    inline float Get_Gimbal_Yaw_Omega_FeedForward();

    inline Enum_Manifold_Enemy_ID Get_Enemy_ID();

    inline uint16_t Get_Confidence_Level();

    inline float Get_Target_Gimbal_Yaw();

    inline float Get_Target_Gimbal_Pitch();

    inline Enum_Manifold_Enemy_Color Get_Enemy_Color();

    inline void Set_Aiming_Priority(Enum_Manifold_Aiming_Priority __Aiming_Priority);

    inline void Set_Velocity_X(float __Velocity_X);

    inline void Set_Velocity_Y(float __Velocity_Y);

    inline void Set_Enemy_Color(Enum_Manifold_Enemy_Color __Self_Color);

    inline void Set_Now_Gimbal_Yaw(float __Now_Gimbal_Yaw);

    inline void Set_Now_Gimbal_Pitch(float __Now_Gimbal_Pitch);

    void UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length);

    void TIM_1000ms_Alive_PeriodElapsedCallback();

    void TIM_10ms_Send_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;
    // 数据包头标
    uint16_t Frame_Header;

    // 常量

    // 内部变量

    // 当前时刻的视觉Manifold接收flag
    uint32_t Flag = 0;
    // 前一时刻的视觉Manifold接收flag
    uint32_t Pre_Flag = 0;
    // 视觉Manifold角度增量最大值
    float Gimbal_Pitch_Angle_Increment_Max = PI / 6.0f;
    float Gimbal_Yaw_Angle_Increment_Max = PI / 3.0f;

    // 云台角度偏差值
    float Gimbal_Pitch_Angle_Offset = 0.004f;
    float Gimbal_Yaw_Angle_Offset = 0.0f;

    // 读变量

    // 视觉Manifold状态
    Enum_Manifold_Status Manifold_Status = Manifold_Status_DISABLE;
    // 视觉Manifold对外接口信息
    Struct_Manifold_UART_Rx_Data Rx_Data;
    // 目标云台角度
    float Target_Gimbal_Pitch = 0.0f;
    float Target_Gimbal_Yaw = 0.0f;

    // 写变量

    // 视觉Manifold对外接口信息
    Struct_Manifold_UART_Tx_Data Tx_Data;

    // 当前云台角度, 用于获取绝对角度
    float Now_Gimbal_Pitch = 0.0f;
    float Now_Gimbal_Yaw = 0.0f;

    // 读写变量

    // 内部函数

    void Data_Process(uint16_t Length);

    void Output();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取Manifold在线状态
 *
 * @return Enum_Manifold_Status Manifold在线状态
 */
inline Enum_Manifold_Status Class_Manifold::Get_Status()
{
    return (Manifold_Status);
}

/**
 * @brief 获取发射状态
 *
 * @return Enum_Manifold_Status 发射状态
 */
inline uint8_t Class_Manifold::Get_Shoot_Flag()
{
    return (Rx_Data.Shoot_Flag);
}

/**
 * @brief 获取云台Pitch角度增量
 *
 * @return float 云台Pitch角度增量
 */
inline float Class_Manifold::Get_Gimbal_Pitch_Angle_Increment()
{
    return (Rx_Data.Gimbal_Pitch_Angle_Increment);
}

/**
 * @brief 获取云台Yaw角度增量
 *
 * @return float 云台Yaw角度增量
 */
inline float Class_Manifold::Get_Gimbal_Yaw_Angle_Increment()
{
    return (Rx_Data.Gimbal_Yaw_Angle_Increment);
}

/**
 * @brief 获取云台Pitch角速度前馈
 *
 * @return float 云台Pitch角速度前馈
 */
inline float Class_Manifold::Get_Gimbal_Pitch_Omega_FeedForward()
{
    return (Rx_Data.Gimbal_Pitch_Omega_FeedForward);
}

/**
 * @brief 获取云台Yaw角速度前馈
 *
 * @return float 云台Yaw角速度前馈
 */
inline float Class_Manifold::Get_Gimbal_Yaw_Omega_FeedForward()
{
    return (Rx_Data.Gimbal_Yaw_Omega_FeedForward);
}

/**
 * @brief 获取瞄准的敌方机器人信息
 *
 * @return Enum_Manifold_Enemy_ID 瞄准的敌方机器人信息
 */
inline Enum_Manifold_Enemy_ID Class_Manifold::Get_Enemy_ID()
{
    return (Rx_Data.Enemy_ID);
}

/**
 * @brief 获取算法置信度
 *
 * @return uint16_t 算法置信度
 */
inline uint16_t Class_Manifold::Get_Confidence_Level()
{
    return (Rx_Data.Confidence_Level);
}

/**
 * @brief 获取目标云台角度
 *
 * @return float 目标云台角度
 */
inline float Class_Manifold::Get_Target_Gimbal_Yaw()
{
    return (Target_Gimbal_Yaw);
}

/**
 * @brief 获取目标云台角度
 *
 * @return float 目标云台角度
 */
inline float Class_Manifold::Get_Target_Gimbal_Pitch()
{
    return (Target_Gimbal_Pitch);
}

/**
 * @brief 获取敌方颜色
 *
 * @return Enum_Manifold_Enemy_Color 敌方颜色
 */
inline Enum_Manifold_Enemy_Color Class_Manifold::Get_Enemy_Color()
{
    return (Tx_Data.Enemy_Color);
}

/**
 * @brief 设定自瞄算法优先级
 *
 * @param __Aiming_Priority 自瞄算法优先级
 */
inline void Class_Manifold::Set_Aiming_Priority(Enum_Manifold_Aiming_Priority __Aiming_Priority)
{
    Tx_Data.Aiming_Priority = __Aiming_Priority;
}

/**
 * @brief 设定速度vx
 *
 * @param __Velocity_X 速度vx
 */
inline void Class_Manifold::Set_Velocity_X(float __Velocity_X)
{
    Tx_Data.Velocity_X = __Velocity_X;
}

/**
 * @brief 设定速度vy
 *
 * @param __Velocity_Y 速度vy
 */
inline void Class_Manifold::Set_Velocity_Y(float __Velocity_Y)
{
    Tx_Data.Velocity_Y = __Velocity_Y;
}

/**
 * @brief 设定敌方颜色
 *
 * @param __Enemy_Color 敌方颜色
 */
inline void Class_Manifold::Set_Enemy_Color(Enum_Manifold_Enemy_Color __Enemy_Color)
{
    Tx_Data.Enemy_Color = __Enemy_Color;
}

/**
 * @brief 设定设定当前云台角度, 用于获取绝对角度
 *
 * @param __Now_Gimbal_Yaw 设定当前云台角度, 用于获取绝对角度
 */
inline void Class_Manifold::Set_Now_Gimbal_Yaw(float __Now_Gimbal_Yaw)
{
    Now_Gimbal_Yaw = __Now_Gimbal_Yaw;
}

/**
 * @brief 设定当前云台角度, 用于获取绝对角度
 *
 * @param __Now_Gimbal_Pitch 设定当前云台角度, 用于获取绝对角度
 */
inline void Class_Manifold::Set_Now_Gimbal_Pitch(float __Now_Gimbal_Pitch)
{
    Now_Gimbal_Pitch = __Now_Gimbal_Pitch;
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
