/**
 * @file crt_chassis.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 底盘电控
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-11-19 1.1 修改为四轮底盘
 * @date 2024-01-17 1.2 新增速度规划等算法, 将各个算法解耦合
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/**
 * @brief 轮组编号
 * 3[0] 2[3]
 * 4[1] 1[2]
 * 前x左y上z
 */

/* Includes ------------------------------------------------------------------*/

#include "crt_chassis.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 底盘初始化
 *
 * @param __Chassis_Control_Type 底盘控制方式, 默认舵轮方式
 * @param __Speed 底盘速度限制最大值
 */
void Class_Chassis::Init()
{
    // PID初始化

    // 底盘速度xPID, 输出摩擦力
    PID_Velocity_X.Init(600.0f, 0.0f, 0.0f, 0.0f, 150.0f, 3000.0f, 0.002f);

    // 底盘速度yPID, 输出摩擦力
    PID_Velocity_Y.Init(600.0f, 0.0f, 0.0f, 0.0f, 150.0f, 3000.0f, 0.002f);

    // 底盘角速度PID, 输出扭矩
    PID_Omega.Init(25.0f, 0.0f, 0.0f, 0.0f, 10.0f, 400.0f, 0.002f);

    // 舵向电机初始化

    // PID初始化
    Motor_Steer[0].PID_Angle.Init(10.0f, 0.0f, 0.0f, 0.0f, 6.0f * PI, 6.0f * PI, 0.002f);
    Motor_Steer[0].PID_Omega.Init(0.12f, 1.0f, 0.0f, 0.0f, 3.0f, 3.0f, 0.002f);
    // 电机初始化
    Motor_Steer[0].Init(&hcan2, Motor_DJI_ID_0x207, Motor_DJI_Control_Method_ANGLE, 1696, Motor_DJI_GM6020_Driver_Version_2023, Motor_DJI_Power_Limit_Status_ENABLE);

    // PID初始化
    Motor_Steer[1].PID_Angle.Init(10.0f, 0.0f, 0.0f, 0.0f, 6.0f * PI, 6.0f * PI, 0.002f);
    Motor_Steer[1].PID_Omega.Init(0.12f, 1.0f, 0.0f, 0.0f, 3.0f, 3.0f, 0.002f);
    // 电机初始化
    Motor_Steer[1].Init(&hcan2, Motor_DJI_ID_0x208, Motor_DJI_Control_Method_ANGLE, -3145, Motor_DJI_GM6020_Driver_Version_2023, Motor_DJI_Power_Limit_Status_ENABLE);

    // PID初始化
    Motor_Steer[2].PID_Angle.Init(10.0f, 0.0f, 0.0f, 0.0f, 6.0f * PI, 6.0f * PI, 0.002f);
    Motor_Steer[2].PID_Omega.Init(0.12f, 1.0f, 0.0f, 0.0f, 3.0f, 3.0f, 0.002f);
    // 电机初始化
    Motor_Steer[2].Init(&hcan2, Motor_DJI_ID_0x205, Motor_DJI_Control_Method_ANGLE, -1025, Motor_DJI_GM6020_Driver_Version_2023, Motor_DJI_Power_Limit_Status_ENABLE);

    // PID初始化
    Motor_Steer[3].PID_Angle.Init(10.0f, 0.0f, 0.0f, 0.0f, 6.0f * PI, 6.0f * PI, 0.002f);
    Motor_Steer[3].PID_Omega.Init(0.12f, 1.0f, 0.0f, 0.0f, 3.0f, 3.0f, 0.002f);
    // 电机初始化
    Motor_Steer[3].Init(&hcan2, Motor_DJI_ID_0x206, Motor_DJI_Control_Method_ANGLE, 3732, Motor_DJI_GM6020_Driver_Version_2023, Motor_DJI_Power_Limit_Status_ENABLE);

    // 轮向电机初始化

    // 电机初始化
    Motor_Wheel[0].Init(&hcan2, Motor_DJI_ID_0x203, Motor_DJI_Control_Method_CURRENT, 268.0f / 17.0f, Motor_DJI_Power_Limit_Status_ENABLE);

    // 电机初始化
    Motor_Wheel[1].Init(&hcan2, Motor_DJI_ID_0x204, Motor_DJI_Control_Method_CURRENT, 268.0f / 17.0f, Motor_DJI_Power_Limit_Status_ENABLE);

    // 电机初始化
    Motor_Wheel[2].Init(&hcan2, Motor_DJI_ID_0x201, Motor_DJI_Control_Method_CURRENT, 268.0f / 17.0f, Motor_DJI_Power_Limit_Status_ENABLE);

    // 电机初始化
    Motor_Wheel[3].Init(&hcan2, Motor_DJI_ID_0x202, Motor_DJI_Control_Method_CURRENT, 268.0f / 17.0f, Motor_DJI_Power_Limit_Status_ENABLE);
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Chassis::TIM_100ms_Alive_PeriodElapsedCallback()
{
    for (int i = 0; i < 4; i++)
    {
        Motor_Steer[i].TIM_100ms_Alive_PeriodElapsedCallback();
        Motor_Wheel[i].TIM_100ms_Alive_PeriodElapsedCallback();
    }
}

/**
 * @brief TIM定时器中断解算回调函数
 *
 */
void Class_Chassis::TIM_2ms_Resolution_PeriodElapsedCallback()
{
    Self_Resolution();
}

/**
 * @brief TIM定时器中断控制回调函数
 *
 */
void Class_Chassis::TIM_2ms_Control_PeriodElapsedCallback()
{
    Kinematics_Inverse_Resolution();

    Output_To_Dynamics();

    Dynamics_Inverse_Resolution();

    Output_To_Motor();
}

/**
 * @brief 自身解算
 *
 */
void Class_Chassis::Self_Resolution()
{
    // 根据电机编码器与陀螺仪计算速度和角度

    Now_Velocity_X = 0.0f;
    Now_Velocity_Y = 0.0f;
    Now_Omega = 0.0f;

    // 轮速计的计算方式

    for (int i = 0; i < 4; i++)
    {
        Now_Velocity_X += (Motor_Wheel[i].Get_Now_Omega() * arm_cos_f32(Motor_Steer[i].Get_Now_Angle()) * Wheel_Radius) / 4.0f;
        Now_Velocity_Y += (Motor_Wheel[i].Get_Now_Omega() * arm_sin_f32(Motor_Steer[i].Get_Now_Angle()) * Wheel_Radius) / 4.0f;
        Now_Omega += (Motor_Wheel[i].Get_Now_Omega() * arm_sin_f32(Motor_Steer[i].Get_Now_Angle() - Wheel_Azimuth[i]) * Wheel_Radius / Wheel_To_Core_Distance[i]) / 4.0f;
    }

    // 角度解算
    float pitch = -AHRS_Chassis->Get_Angle_Pitch();
    float roll = AHRS_Chassis->Get_Angle_Roll();
    if (isnan(pitch) == true || isnan(roll) == true)
    {
        Angle_Pitch = 0.0f;
        Angle_Roll = 0.0f;
    }
    else
    {
        Angle_Pitch = pitch;
        Angle_Roll = roll;
    }

    Slope_Direction_X = arm_sin_f32(pitch) * arm_cos_f32(roll);
    Slope_Direction_Y = -arm_sin_f32(roll);
    Slope_Direction_Z = arm_cos_f32(pitch) * arm_cos_f32(roll);

    // 计算功率值
    Now_Motor_Power = 0.0f;
    Now_Steer_Motor_Power = 0.0f;
    Now_Wheel_Motor_Power = 0.0f;
    for (int i = 0; i < 4; i++)
    {
        Now_Motor_Power += Motor_Steer[i].Get_Now_Power() + Motor_Wheel[i].Get_Now_Power();
        Now_Steer_Motor_Power += Motor_Steer[i].Get_Now_Power();
        Now_Wheel_Motor_Power += Motor_Wheel[i].Get_Now_Power();
    }
}

/**
 * @brief 运动学逆解算
 *
 */
void Class_Chassis::Kinematics_Inverse_Resolution()
{
    for (int i = 0; i < 4; i++)
    {
        float tmp_velocity_x, tmp_velocity_y, tmp_velocity_modulus;

        // 解算到每个轮组的具体线速度
        tmp_velocity_x = Target_Velocity_X - Target_Omega * Wheel_To_Core_Distance[i] * arm_sin_f32(Wheel_Azimuth[i]);
        tmp_velocity_y = Target_Velocity_Y + Target_Omega * Wheel_To_Core_Distance[i] * arm_cos_f32(Wheel_Azimuth[i]);
        arm_sqrt_f32(tmp_velocity_x * tmp_velocity_x + tmp_velocity_y * tmp_velocity_y, &tmp_velocity_modulus);

        // 根据线速度决定轮向电机角速度
        Target_Wheel_Omega[i] = tmp_velocity_modulus / Wheel_Radius;

        // 根据速度的xy分量分别决定舵向电机角度
        if (tmp_velocity_modulus == 0.0f)
        {
            // 排除除零问题
            Target_Steer_Angle[i] = Motor_Steer[i].Get_Now_Angle();
        }
        else
        {
            // 没有除零问题
            Target_Steer_Angle[i] = atan2f(tmp_velocity_y, tmp_velocity_x);
        }
    }

    _Steer_Motor_Kinematics_Nearest_Transposition();
}

/**
 * @brief 舵向电机依照轮向电机目标角速度就近转位
 *
 */
void Class_Chassis::_Steer_Motor_Kinematics_Nearest_Transposition()
{
    for (int i = 0; i < 4; i++)
    {
        float tmp_delta_angle = Math_Modulus_Normalization(Target_Steer_Angle[i] - Motor_Steer[i].Get_Now_Angle(), 2.0f * PI);

        // 根据转动角度范围决定是否需要就近转位
        if (-PI / 2.0f <= tmp_delta_angle && tmp_delta_angle <= PI / 2.0f)
        {
            // ±PI / 2之间无需反向就近转位
            Target_Steer_Angle[i] = tmp_delta_angle + Motor_Steer[i].Get_Now_Angle();
        }
        else
        {
            // 需要反转扣圈情况
            Target_Steer_Angle[i] = Math_Modulus_Normalization(tmp_delta_angle + PI, 2.0f * PI) + Motor_Steer[i].Get_Now_Angle();
            Target_Wheel_Omega[i] *= -1.0f;
        }
    }
}

/**
 * @brief 输出到动力学状态
 *
 */
void Class_Chassis::Output_To_Dynamics()
{
    switch (Chassis_Control_Type)
    {
    case (Chassis_Control_Type_DISABLE):
    {
        // 底盘失能
        for (int i = 0; i < 4; i++)
        {
            PID_Velocity_X.Set_Integral_Error(0.0f);
            PID_Velocity_Y.Set_Integral_Error(0.0f);
            PID_Omega.Set_Integral_Error(0.0f);
        }

        break;
    }
    case (Chassis_Control_Type_NORMAL):
    {

        PID_Velocity_X.Set_Target(Target_Velocity_X);
        PID_Velocity_X.Set_Now(Now_Velocity_X);
        PID_Velocity_X.TIM_Calculate_PeriodElapsedCallback();

        PID_Velocity_Y.Set_Target(Target_Velocity_Y);
        PID_Velocity_Y.Set_Now(Now_Velocity_Y);
        PID_Velocity_Y.TIM_Calculate_PeriodElapsedCallback();

        PID_Omega.Set_Target(Target_Omega);
        PID_Omega.Set_Now(Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        break;
    }
    }
}

/**
 * @brief 动力学逆解算
 *
 */
void Class_Chassis::Dynamics_Inverse_Resolution()
{
    float force_x, force_y, torque_omega;

    force_x = PID_Velocity_X.Get_Out();
    force_y = PID_Velocity_Y.Get_Out();
    torque_omega = PID_Omega.Get_Out();

    // 每个轮的扭力
    float tmp_force[4];
    for (int i = 0; i < 4; i++)
    {
        // 解算到每个轮组的具体摩擦力
        tmp_force[i] = force_x * arm_cos_f32(Motor_Steer[i].Get_Now_Angle()) + force_y * arm_sin_f32(Motor_Steer[i].Get_Now_Angle()) - torque_omega / Wheel_To_Core_Distance[i] * arm_sin_f32(Wheel_Azimuth[i] - Motor_Steer[i].Get_Now_Angle());
    }
    for (int i = 0; i < 4; i++)
    {
        // 摩擦力转换至扭矩
        Target_Wheel_Current[i] = tmp_force[i] * Wheel_Radius + Wheel_Speed_Limit_Factor * (Target_Wheel_Omega[i] - Motor_Wheel[i].Get_Now_Omega());
        // 动摩擦阻力前馈
        if (Target_Wheel_Omega[i] > Wheel_Resistance_Omega_Threshold)
        {
            Target_Wheel_Current[i] += Dynamic_Resistance_Wheel_Current[i];
        }
        else if (Target_Wheel_Omega[i] < -Wheel_Resistance_Omega_Threshold)
        {
            Target_Wheel_Current[i] -= Dynamic_Resistance_Wheel_Current[i];
        }
        else
        {
            Target_Wheel_Current[i] += Motor_Wheel[i].Get_Now_Omega() / Wheel_Resistance_Omega_Threshold * Dynamic_Resistance_Wheel_Current[i];
        }
    }

    // 根据斜坡与压力进行电流限幅防止贴地打滑
    // TODO
}

/**
 * @brief 输出到电机
 *
 */
void Class_Chassis::Output_To_Motor()
{
    switch (Chassis_Control_Type)
    {
    case (Chassis_Control_Type_DISABLE):
    {
        // 底盘失能
        for (int i = 0; i < 4; i++)
        {
            Motor_Steer[i].Set_Control_Method(Motor_DJI_Control_Method_CURRENT);
            Motor_Wheel[i].Set_Control_Method(Motor_DJI_Control_Method_CURRENT);

            Motor_Steer[i].PID_Angle.Set_Integral_Error(0.0f);
            Motor_Steer[i].PID_Omega.Set_Integral_Error(0.0f);

            Motor_Steer[i].Set_Target_Current(0.0f);
            Motor_Wheel[i].Set_Target_Current(0.0f);
        }

        break;
    }
    case (Chassis_Control_Type_NORMAL):
    {
        // 舵轮模型
        for (int i = 0; i < 4; i++)
        {
            Motor_Steer[i].Set_Control_Method(Motor_DJI_Control_Method_ANGLE);
            Motor_Wheel[i].Set_Control_Method(Motor_DJI_Control_Method_CURRENT);
        }

        for (int i = 0; i < 4; i++)
        {
            Motor_Steer[i].Set_Target_Angle(Target_Steer_Angle[i]);

            Motor_Wheel[i].Set_Target_Current(Target_Wheel_Current[i]);
        }

        break;
    }
    }

    for (int i = 0; i < 4; i++)
    {
        Motor_Steer[i].TIM_Calculate_PeriodElapsedCallback();
        Motor_Wheel[i].TIM_Calculate_PeriodElapsedCallback();
    }

    _Power_Limit_Control();

    for (int i = 0; i < 4; i++)
    {
        Motor_Wheel[i].TIM_Power_Limit_After_Calculate_PeriodElapsedCallback();
        Motor_Steer[i].TIM_Power_Limit_After_Calculate_PeriodElapsedCallback();
    }
}

/**
 * @brief 功率控制
 *
 */
void Class_Chassis::_Power_Limit_Control()
{
    float available_power = Power_Limit_Max;
    float consume_power = 0.0f;
    float steer_consume_power = 0.0f;
    float wheel_consume_power = 0.0f;
    float steer_power_single[4], wheel_power_single[4];

    for (int i = 0; i < 4; i++)
    {
        // 获取舵向电机功率估计值
        steer_power_single[i] = Motor_Steer[i].Get_Power_Estimate();
        // 正功计入消耗, 负功计入补偿
        if (steer_power_single[i] > 0)
        {
            consume_power += steer_power_single[i];
            steer_consume_power += steer_power_single[i];
        }
        else
        {
            available_power += -steer_power_single[i];
        }

        // 获取轮向电机功率估计值
        wheel_power_single[i] = Motor_Wheel[i].Get_Power_Estimate();
        // 正功计入消耗, 负功计入补偿
        if (wheel_power_single[i] > 0)
        {
            consume_power += wheel_power_single[i];
            wheel_consume_power += wheel_power_single[i];
        }
        else
        {
            available_power += -wheel_power_single[i];
        }
    }

    // 舵向电机功率控制

    Steer_Factor = __Steer_Power_Limit_Control(available_power * 0.6f, steer_consume_power);
    for (int i = 0; i < 4; i++)
    {
        if (steer_power_single[i] > 0)
        {
            Motor_Steer[i].Set_Power_Factor(Steer_Factor);
        }
        else
        {
            Motor_Steer[i].Set_Power_Factor(1.0f);
        }
    }

    // 轮向电机功率控制

    Wheel_Factor = __Wheel_Power_Limit_Control(available_power - Now_Steer_Motor_Power, wheel_consume_power);
    for (int i = 0; i < 4; i++)
    {
        if (wheel_power_single[i] > 0)
        {
            Motor_Wheel[i].Set_Power_Factor(Wheel_Factor);
        }
        else
        {
            Motor_Wheel[i].Set_Power_Factor(1.0f);
        }
    }

    // 零速度则待机
    if (Target_Velocity_X == 0.0f && Target_Velocity_Y == 0.0f && Target_Omega == 0.0f)
    {
        for (int i = 0; i < 4; i++)
        {
            Motor_Steer[i].PID_Angle.Set_Integral_Error(0.0f);
            Motor_Steer[i].PID_Omega.Set_Integral_Error(0.0f);
        }
    }
}

/**
 * @brief 舵向电机功率控制
 *
 * @param steer_available_power 舵向电机可用功率
 * @param steer_consume_power 舵向电机消耗功率
 * @return 功率因数
 */
float Class_Chassis::__Steer_Power_Limit_Control(float steer_available_power, float steer_consume_power)
{
    if (steer_consume_power <= steer_available_power)
    {
        // 无需功率控制
        return (1.0f);
    }
    else
    {
        // 功率分配因子计算
        return (steer_available_power / steer_consume_power);
    }
}

/**
 * @brief 轮向电机功率控制
 *
 * @param wheel_available_power 轮向电机可用功率
 * @param wheel_consume_power 轮向电机消耗功率
 * @return 功率因数
 */
float Class_Chassis::__Wheel_Power_Limit_Control(float wheel_available_power, float wheel_consume_power)
{
    if (wheel_consume_power <= wheel_available_power)
    {
        // 无需功率控制
        return (1.0f);
    }
    else
    {
        // 功率分配因子计算
        return (wheel_available_power / wheel_consume_power);
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
