/**
 * @file alg_pid.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief PID算法
 * @version 0.1
 * @date 2022-05-03
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

#ifndef ALG_PID_H
#define ALG_PID_H

/* Includes ------------------------------------------------------------------*/

#include "drv_math.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 微分先行
 *
 */
enum Enum_PID_D_First
{
    PID_D_First_DISABLE = 0,
    PID_D_First_ENABLE,
};

/**
 * @brief Reusable, PID算法
 *
 */
class Class_PID
{
public:
    void Init(float __K_P, float __K_I, float __K_D, float __K_F = 0.0f, float __I_Out_Max = 0.0f, float __Out_Max = 0.0f, float __D_T = 0.001f, float __Dead_Zone = 0.0f, float __I_Variable_Speed_A = 0.0f, float __I_Variable_Speed_B = 0.0f, float __I_Separate_Threshold = 0.0f, Enum_PID_D_First __D_First = PID_D_First_DISABLE);

    float Get_Integral_Error();
    float Get_Out();

    void Set_K_P(float __K_P);
    void Set_K_I(float __K_I);
    void Set_K_D(float __K_D);
    void Set_K_F(float __K_F);
    void Set_I_Out_Max(float __I_Out_Max);
    void Set_Out_Max(float __Out_Max);
    void Set_I_Variable_Speed_A(float __Variable_Speed_I_A);
    void Set_I_Variable_Speed_B(float __Variable_Speed_I_B);
    void Set_I_Separate_Threshold(float __I_Separate_Threshold);
    void Set_Target(float __Target);
    void Set_Now(float __Now);
    void Set_Integral_Error(float __Integral_Error);

    void TIM_Adjust_PeriodElapsedCallback();

protected:
    //初始化相关常量

    // PID计时器周期, s
    float D_T;
    //死区, Error在其绝对值内不输出
    float Dead_Zone;
    //微分先行
    Enum_PID_D_First D_First;

    //常量

    //内部变量

    //之前的当前值
    float Pre_Now = 0.0f;
    //之前的目标值
    float Pre_Target = 0.0f;
    //之前的输出值
    float Pre_Out = 0.0f;
    //前向误差
    float Pre_Error = 0.0f;

    //读变量

    //输出值
    float Out = 0.0f;

    //写变量

    // PID的P
    float K_P = 0.0f;
    // PID的I
    float K_I = 0.0f;
    // PID的D
    float K_D = 0.0f;
    //前馈
    float K_F = 0.0f;

    //积分限幅, 0为不限制
    float I_Out_Max = 0;
    //输出限幅, 0为不限制
    float Out_Max = 0;

    //变速积分定速内段阈值, 0为不限制
    float I_Variable_Speed_A = 0.0f;
    //变速积分变速区间, 0为不限制
    float I_Variable_Speed_B = 0.0f;
    //积分分离阈值，需为正数, 0为不限制
    float I_Separate_Threshold = 0.0f;

    //目标值
    float Target = 0.0f;
    //当前值
    float Now = 0.0f;

    //读写变量

    //积分值
    float Integral_Error = 0.0f;

    //内部函数
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
