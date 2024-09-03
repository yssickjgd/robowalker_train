/**
 * @file crt_posture.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 战车姿态感知器
 * @version 0.1
 * @date 2024-06-13 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

#ifndef CRT_POSTURE_H
#define CRT_POSTURE_H

/* Includes ------------------------------------------------------------------*/

#include "3_Chariot/1_Module/Chassis/crt_chassis.h"
#include "3_Chariot/1_Module/Gimbal/crt_gimbal.h"
#include "2_Device/AHRS/AHRS_WHEELTEC/dvc_ahrs_wheeltec.h"
#include "2_Device/AHRS/AHRS_WIT/dvc_ahrs_wit.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Specialized, 姿态感知器类
 *
 */
class Class_Posture
{
public:

    // 底盘
    Class_Chassis *Chassis;

    // 云台
    Class_Gimbal *Gimbal;

    // 云台陀螺仪
    Class_AHRS_WIT AHRS_Gimbal;

    // 底盘陀螺仪
    Class_AHRS_WHEELTEC AHRS_Chassis;

    // 底盘在Odom旋转矩阵
    Eigen::Matrix3f Matrix_Chassis_Odom_Rotation;

    // 云台在Odom旋转矩阵
    Eigen::Matrix3f Matrix_Gimbal_Odom_Rotation;

    // 云台在底盘旋转矩阵
    Eigen::Matrix3f Matrix_Gimbal_Chassis_Rotation;

    void Init();

    inline float Get_Chassis_Velocity_X();

    inline float Get_Chassis_Velocity_Y();

    inline float Get_Chassis_Omega();

    inline float Get_Chassis_Odom_Angle_Yaw();

    inline float Get_Chassis_Odom_Angle_Pitch();

    inline float Get_Chassis_Odom_Angle_Roll();

    inline float Get_Gimbal_Velocity_X();

    inline float Get_Gimbal_Velocity_Y();

    inline float Get_Gimbal_Omega_Yaw();

    inline float Get_Gimbal_Omega_Pitch();

    inline float Get_Gimbal_Odom_Angle_Yaw();

    inline float Get_Gimbal_Odom_Angle_Pitch();

    inline float Get_Gimbal_Odom_Angle_Roll();

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_1ms_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 内部变量

    // 读变量

    // 底盘相对自身速度
    float Chassis_Velocity_X;
    float Chassis_Velocity_Y;
    float Chassis_Omega;

    // 底盘在Odom角度
    float Chassis_Odom_Angle_Yaw;
    float Chassis_Odom_Angle_Pitch;
    float Chassis_Odom_Angle_Roll;

    // 云台相对自身速度
    float Gimbal_Velocity_X;
    float Gimbal_Velocity_Y;

    // 云台相对自身角速度
    float Gimbal_Omega_Yaw;
    float Gimbal_Omega_Pitch;

    // 云台在Odom角度
    float Gimbal_Odom_Angle_Yaw;
    float Gimbal_Odom_Angle_Pitch;
    float Gimbal_Odom_Angle_Roll;

    // 写变量

    // 内部函数

};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取相对自身速度
 *
 * @return 底盘相对自身速度
 */
inline float Class_Posture::Get_Chassis_Velocity_X()
{
    return (Chassis_Velocity_X);
}

/**
 * @brief 获取相对自身速度
 *
 * @return 底盘相对自身速度
 */
inline float Class_Posture::Get_Chassis_Velocity_Y()
{
    return (Chassis_Velocity_Y);
}

/**
 * @brief 获取相对自身速度
 *
 * @return 底盘相对自身速度
 */
inline float Class_Posture::Get_Chassis_Omega()
{
    return (Chassis_Omega);
}

/**
 * @brief 获取底盘在Odom角度
 *
 * @return 底盘在Odom角度
 */
inline float Class_Posture::Get_Chassis_Odom_Angle_Yaw()
{
    return (Chassis_Odom_Angle_Yaw);
}

/**
 * @brief 获取底盘在Odom角度
 *
 * @return 底盘在Odom角度
 */
inline float Class_Posture::Get_Chassis_Odom_Angle_Pitch()
{
    return (Chassis_Odom_Angle_Pitch);
}

/**
 * @brief 获取底盘在Odom角度
 *
 * @return 底盘在Odom角度
 */
inline float Class_Posture::Get_Chassis_Odom_Angle_Roll()
{
    return (Chassis_Odom_Angle_Roll);
}

/**
 * @brief 获取云台在Odom速度
 *
 * @return 云台在Odom速度
 */
inline float Class_Posture::Get_Gimbal_Velocity_X()
{
    return (Gimbal_Velocity_X);
}

/**
 * @brief 获取云台在Odom速度
 *
 * @return 云台在Odom速度
 */
inline float Class_Posture::Get_Gimbal_Velocity_Y()
{
    return (Gimbal_Velocity_Y);
}

/**
 * @brief 获取云台相对自身角速度
 *
 * @return 云台相对自身角速度
 */
inline float Class_Posture::Get_Gimbal_Omega_Yaw()
{
    return (Gimbal_Omega_Yaw);
}

/**
 * @brief 获取云台相对自身角速度
 *
 * @return 云台相对自身角速度
 */
inline float Class_Posture::Get_Gimbal_Omega_Pitch()
{
    return (Gimbal_Omega_Pitch);
}

/**
 * @brief 获取云台在Odom角度
 *
 * @return 云台在Odom角度
 */
inline float Class_Posture::Get_Gimbal_Odom_Angle_Yaw()
{
    return (Gimbal_Odom_Angle_Yaw);
}

/**
 * @brief 获取云台在Odom角度
 *
 * @return 云台在Odom角度
 */
inline float Class_Posture::Get_Gimbal_Odom_Angle_Pitch()
{
    return (Gimbal_Odom_Angle_Pitch);
}

/**
 * @brief 获取云台在Odom角度
 *
 * @return 云台在Odom角度
 */
inline float Class_Posture::Get_Gimbal_Odom_Angle_Roll()
{
    return (Gimbal_Odom_Angle_Roll);
}


#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
