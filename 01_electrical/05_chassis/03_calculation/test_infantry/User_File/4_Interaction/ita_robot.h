/**
 * @file ita_robot.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 人机交互控制逻辑
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-01-17 1.1 更名为ita_robot.h, 引入新功能
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

#ifndef ITA_ROBOT_H
#define ITA_ROBOT_H

/* Includes ------------------------------------------------------------------*/

#include "3_Chariot/1_Module/Chassis/crt_chassis.h"
#include "3_Chariot/1_Module/Gimbal/crt_gimbal.h"
#include "3_Chariot/1_Module/Booster/crt_booster.h"
#include "3_Chariot/2_Posture/Posture/crt_posture.h"
#include "2_Device/DR16/dvc_dr16.h"
#include "2_Device/Manifold/dvc_manifold.h"
#include "2_Device/Supercap/Supercap_24/dvc_supercap_24.h"
#include "2_Device/Referee/dvc_referee.h"
#include "1_Middleware/2_Algorithm/Timer/alg_timer.h"
#include "1_Middleware/2_Algorithm/Queue/alg_queue.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 长按左键状态类型
 *
 */
enum Enum_FSM_Press_Hold_Status
{
    FSM_Press_Hold_Status_STOP = 0,
    FSM_Press_Hold_Status_FREE,
    FSM_Press_Hold_Status_PRESSED,
    FSM_Press_Hold_Status_CLICK,
    FSM_Press_Hold_Status_HOLD,
};

/**
 * @brief 热量自检测状态类型
 *
 */
enum Enum_FSM_Heat_Detector_Status
{
    FSM_Heat_Detector_Status_CLOSE = 0,
    FSM_Heat_Detector_Status_OPEN,
};

/**
 * @brief 底盘类型
 *
 */
enum Enum_Robot_Chassis_Type
{
    Robot_Chassis_Type_POWER = 0,
    Robot_Chassis_Type_HP,
};

/**
 * @brief 小陀螺类型
 *
 */
enum Enum_Robot_Gyroscope_Type
{
    Robot_Gyroscope_Type_DISABLE = 0,
    Robot_Gyroscope_Type_CLOCKWISE,
    Robot_Gyroscope_Type_COUNTERCLOCKWISE,
};

/**
 * @brief 发射机构类型
 *
 */
enum Enum_Robot_Booster_Type
{
    Robot_Booster_Type_BURST = 0,
    Robot_Booster_Type_CD,
};

class Class_Robot;

/**
 * @brief Specialized, 长按左键有限自动机
 *
 */
class Class_FSM_Press_Hold : public Class_FSM
{
public:
    Class_Robot *Robot;

    uint32_t Get_Default_Hold_Time_Threshold();

    void Set_Hold_Time_Threshold(uint32_t __Hold_Time_Threshold);

    void TIM_1ms_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // 内部变量

    // 读变量

    // 默认长按时间阈值, 超出被认为长按
    uint32_t Default_Hold_Time_Threshold = 98;

    // 写变量

    // 长按时间阈值, 超出被认为长按
    uint32_t Hold_Time_Threshold = 98;

    // 读写变量

    // 内部函数
};

/**
 * @brief Specialized, 热量自检测有限自动机
 *
 */
class Class_FSM_Heat_Detector : public Class_FSM
{
public:
    Class_Robot *Robot;

    float Get_Now_Heat();

    uint32_t Get_Total_Ammo_Num();

    void TIM_1ms_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // 最大队列长度
    uint32_t Max_Queue_Size = 100;
    // 电流队列和阈值
    float Current_Queue_Sum_Threshold = 100000.0f;

    // 内部变量

    // 电流值队列
    Class_Queue<float, 100> Current_Queue;
    // 电流值队列和
    float Current_Queue_Sum = 0.0f;

    // 读变量

    // 当前热量
    float Now_Heat = 0.0f;
    // 累计子弹数
    uint32_t Total_Ammo_Num = 0;

    // 写变量

    // 读写变量

    // 内部函数
};

/**
 * @brief 控制对象
 *
 */
class Class_Robot
{
public:

    // 长按左键有限自动机
    Class_FSM_Press_Hold FSM_DR16_Left_Mouse_Press_Hold;

    friend class Class_FSM_Press_Hold;

    // 热量自检测逻辑
    Class_FSM_Heat_Detector FSM_Heat_Detector;

    friend class Class_FSM_Heat_Detector;

    // 云台角度接管定时器
    Class_Timer Timer_Turn_About;

    // 视觉切换目标定时器
    Class_Timer Timer_Switch_Target;

    // 能量机关切换回装甲板定时器
    Class_Timer Timer_Rune_To_Armor;

    // 底盘跟随PID
    Class_PID PID_Chassis_Follow;

    // 超级电容对底盘功率控制PID
    Class_PID PID_Supercap_Chassis_Power;

    // 裁判系统对底盘功率控制PID
    Class_PID PID_Referee_Chassis_Power;

    // 斜坡函数底盘速度x
    Class_Slope Slope_Speed_X;

    // 斜坡函数底盘速度y
    Class_Slope Slope_Speed_Y;

    // 斜坡函数底盘角速度
    Class_Slope Slope_Omega;

    // 遥控器
    Class_DR16 DR16;

    // 视觉Manifold
    Class_Manifold Manifold;

    // 超级电容
    Class_Supercap_24 Supercap;

    // 裁判系统
    Class_Referee Referee;

    // 底盘
    Class_Chassis Chassis;

    // 云台
    Class_Gimbal Gimbal;

    // 发射机构
    Class_Booster Booster;

    // 姿态感知器
    Class_Posture Posture;

    void Init();

    void Loop();

    void TIM_1000ms_Alive_PeriodElapsedCallback();

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_100ms_Calculate_Callback();

    void TIM_10ms_Calculate_PeriodElapsedCallback();

    void TIM_2ms_Calculate_PeriodElapsedCallback();

    void TIM_1ms_Calculate_Callback();

protected:
    // 初始化相关常量

    // 常量

    // DR16摇杆死区
    float DR16_Rocker_Dead_Zone = 0.03f;
    // DR16摇杆对云台yaw灵敏度系数(0.002 * PI表示yaw速度最大时为1rpm)
    float DR16_Rocker_Yaw_Resolution = 0.001f * PI;
    // DR16摇杆对云台pitch灵敏度系数(0.002 * PI表示pitch速度最大时为1rpm)
    float DR16_Rocker_Pitch_Resolution = 0.0005f * PI;
    // DR16鼠标云台yaw灵敏度系数
    float DR16_Mouse_Yaw_Angle_Resolution = 1.0f;
    // DR16鼠标云台pitch灵敏度系数
    float DR16_Mouse_Pitch_Angle_Resolution = 0.6f;
    // DR16滚轮调摩擦轮转速灵敏度系数
    float DR16_Mouse_Friction_Omega_Resolution = 500.0f;

    // 底盘解算方向对底盘速度的前馈
    float AHRS_Chassis_Omega_Feedforward = 0.07f;
    float Chassis_Omega_Feedforward = 0.10f;

    // 内部变量

    // 限制功率控制缓冲能量阈值
    uint16_t Restrict_Enable_Buffer_Energy = 20;
    uint16_t Restrict_Disable_Buffer_Energy = 55;

    // 小陀螺模式是否使能
    Enum_Robot_Gyroscope_Type Chassis_Gyroscope_Mode_Status = Robot_Gyroscope_Type_DISABLE;
    // 底盘跟随模式是否使能
    bool Chassis_Follow_Mode_Status = true;

    // 超级电容是否开启, 操作手控制, ctrl+q
    bool Supercap_Enable = true;
    // 超级电容加速模式是否使能, 操作手控制, shift
    bool Supercap_Accelerate_Status = false;
    // 超级电容是否爆发模式, 操作手控制, ctrl+w
    bool Supercap_Burst_Mode_Status = false;
    // 超级电容自动重启状态
    bool Supercap_Auto_Restart_Flag = false;

    // 自瞄模式是否使能
    bool Manifold_Autoaiming_Status = false;
    // 自瞄己方颜色
    Enum_Manifold_Enemy_Color Manifold_Self_Color = Manifold_Enemy_Color_RED;
    // 自瞄优先级
    Enum_Manifold_Aiming_Priority Manifold_Aiming_Priority = Manifold_Aiming_Priority_ARMOR;
    float Autoaiming_Chassis_Omega_Feedforward = 0.016f;

    // 底盘属性
    Enum_Robot_Chassis_Type Robot_Chassis_Type = Robot_Chassis_Type_HP;
    // 发射机构属性
    Enum_Robot_Booster_Type Robot_Booster_Type = Robot_Booster_Type_CD;
    // 机器人等级
    int32_t Robot_Level = 1;

    // UI初始化属性
    bool UI_Init_Status = false;

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数

    void Referee_Draw_Static_UI();

    void Referee_Draw_Dynamic_UI();

    void _Supercap_Control();

    void _Manifold_Control();

    void _Status_Control();

    void _Chassis_Control();

    void _Gimbal_Control();

    void _Booster_Control();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 设定长按时间阈值, 超出被认为长按
 *
 * @param __Hold_Time_Threshold 长按时间阈值, 超出被认为长按
 */
inline void Class_FSM_Press_Hold::Set_Hold_Time_Threshold(uint32_t __Hold_Time_Threshold)
{
    Hold_Time_Threshold = __Hold_Time_Threshold;
}

/**
 * @brief 设定长按时间阈值, 超出被认为长按
 *
 * @param __Hold_Time_Threshold 长按时间阈值, 超出被认为长按
 */
inline uint32_t Class_FSM_Press_Hold::Get_Default_Hold_Time_Threshold()
{
    return (Default_Hold_Time_Threshold);
}

/**
 * @brief 获取当前热量
 *
 * @return float 当前热量
 */
inline float Class_FSM_Heat_Detector::Get_Now_Heat()
{
    return (Now_Heat);
}

/**
 * @brief 获取累计子弹数
 *
 * @return float 累计子弹数
 */
inline uint32_t Class_FSM_Heat_Detector::Get_Total_Ammo_Num()
{
    return (Total_Ammo_Num);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
