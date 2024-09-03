/**
 * @file crt_booster_motor.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 云台电机
 * @version 0.1
 * @date 2024-07-01 新增
 *
 * @copyright USTC-RoboWalker (c) 2024
 *
 */

/**
 * @brief 云台陀螺仪对应关系
 * x pitch
 * z yaw
 */

#ifndef CRT_BOOSTER_MOTOR_H
#define CRT_BOOSTER_MOTOR_H

/* Includes ------------------------------------------------------------------*/

#include "2_Device/Motor/Motor_DJI/dvc_motor_dji.h"
#include "1_Middleware/2_Algorithm/Filter/alg_filter.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

class Class_Posture;

class Class_Gimbal_Pitch_Motor_DJI_GM6020;

/**
 * @brief Specialized, 摩擦轮电机, 加了滤波器
 *
 */
class Class_Booster_Friction_Motor_DJI_C620 : public Class_Motor_DJI_C620
{
public:
    Class_Filter_Fourier<5> Filter_Fourier_Omega;

    inline float Get_Now_Filter_Omega();

    void TIM_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // 内部变量

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数

    void PID_Calculate();

};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取当前滤波后的速度, rad/s
 *
 * @return float 当前滤波后的速度, rad/s
 */
inline float Class_Booster_Friction_Motor_DJI_C620::Get_Now_Filter_Omega()
{
    return (Filter_Fourier_Omega.Get_Out());
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
