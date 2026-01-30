/**
 * @file tsk_config_and_callback.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 临时任务调度测试用函数, 后续用来存放个人定义的回调函数以及若干任务
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-01-17 1.1 调试到机器人层
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "tsk_config_and_callback.h"

#include "2_Device/Motor/Motor_DJI/dvc_motor_dji.h"
#include "2_Device/BSP/BMI088/bsp_bmi088.h"
#include "2_Device/Plotter/Vofa/dvc_vofa.h"
#include "2_Device/BSP/W25Q64JV/bsp_w25q64jv.h"
#include "2_Device/BSP/WS2812/bsp_ws2812.h"
#include "2_Device/BSP/Buzzer/bsp_buzzer.h"
#include "2_Device/BSP/Power/bsp_power.h"
#include "2_Device/BSP/Key/bsp_key.h"
#include "1_Middleware/Algorithm/Filter/Kalman/alg_filter_kalman.h"
#include "1_Middleware/Algorithm/Matrix/alg_matrix.h"
#include "1_Middleware/Driver/WDG/drv_wdg.h"
#include "1_Middleware/System/Timestamp/sys_timestamp.h"
#include "2_Device/Plotter/Serialplot/dvc_serialplot.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// 串口绘图
Class_Serialplot_USB Serialplot_USB;

// 大疆电机
Class_Motor_DJI_GM6020 motor_6020;
Class_Motor_DJI_C620 motor_3508;

// 全局初始化完成标志位
bool init_finished = false;

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief CAN接收回调函数
 *
 * @param Header 接收报文头
 * @param Buffer 接收数据缓冲区
 */
void Can_Callback_Function(FDCAN_RxHeaderTypeDef &Header, uint8_t *Buffer)
{
    // DJI电机专属

    // 根据ID分配数据
    switch (Header.Identifier)
    {
    case (0x201):
    {
        motor_3508.CAN_RxCpltCallback();

        break;
    }
    case (0x205):
    {
        motor_6020.CAN_RxCpltCallback();

        break;
    }
    }
}

/**
 * @brief 每3600s调用一次
 *
 */
void Task3600s_Callback()
{
    SYS_Timestamp.TIM_3600s_PeriodElapsedCallback();
}

/**
 * @brief 每1s调用一次
 *
 */
void Task1s_Callback()
{
}

/**
 * @brief 每1ms调用一次
 *
 */
void Task1ms_Callback()
{
    static int mod100 = 0;
    mod100++;
    if (mod100 == 100)
    {
        mod100 = 0;

        motor_3508.TIM_100ms_Alive_PeriodElapsedCallback();
        motor_6020.TIM_100ms_Alive_PeriodElapsedCallback();
    }

    static uint32_t Counter;
    Counter++;
    motor_3508.Set_Target_Omega(5.0f * motor_6020.Get_Now_Angle());
    motor_6020.Set_Target_Angle(0.0f);
    motor_3508.TIM_Calculate_PeriodElapsedCallback();
    motor_6020.TIM_Calculate_PeriodElapsedCallback();
    TIM_1ms_CAN_PeriodElapsedCallback();

    // Vofa+显示内容
    float target_omega_3508 = motor_3508.Get_Target_Omega();
    float now_omega_3508 = motor_3508.Get_Now_Omega();
    float target_angle_6020 = motor_6020.Get_Target_Angle();
    float now_angle_6020 = motor_6020.Get_Now_Angle();
    float target_omega_6020 = motor_6020.Get_Target_Omega();
    float now_omega_6020 = motor_6020.Get_Now_Omega();
    Serialplot_USB.Set_Data(6, &target_omega_3508, &now_omega_3508, &target_angle_6020, &now_angle_6020, &target_omega_6020, &now_omega_6020);
    Serialplot_USB.TIM_1ms_Write_PeriodElapsedCallback();

    TIM_1ms_IWDG_PeriodElapsedCallback();
}

/**
 * @brief 每125us调用一次
 *
 */
void Task125us_Callback()
{
}

/**
 * @brief 每10us调用一次
 *
 */
void Task10us_Callback()
{
}

/**
 * @brief 初始化任务
 *
 */
void Task_Init()
{
    SYS_Timestamp.Init(&htim5);
    // 串口绘图的USB
    USB_Init(nullptr);
    // 大疆电机的CAN
    CAN_Init(&hfdcan1, Can_Callback_Function);

    // Vofa初始化
    Serialplot_USB.Init();

    // 大疆电机
    motor_6020.Init(&hfdcan1, Motor_DJI_ID_0x205, Motor_DJI_Control_Method_ANGLE);
    motor_3508.PID_Omega.Init(0.4f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f);
    motor_3508.Init(&hfdcan1, Motor_DJI_ID_0x201, Motor_DJI_Control_Method_OMEGA);
    motor_6020.PID_Omega.Init(0.04f, 0.16f, 0.0f, 0.0f, 0.1f, 0.1f);
    motor_6020.PID_Angle.Init(12.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    motor_6020.Init(&hfdcan1, Motor_DJI_ID_0x205, Motor_DJI_Control_Method_ANGLE);

    // 定时器中断初始化
    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_Start_IT(&htim5);
    HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_Base_Start_IT(&htim7);
    HAL_TIM_Base_Start_IT(&htim8);

    // 标记初始化完成
    init_finished = true;
}

/**
 * @brief 前台循环任务
 *
 */
void Task_Loop()
{
}

/**
 * @brief GPIO中断回调函数
 *
 * @param GPIO_Pin 中断引脚
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (!init_finished)
    {
        return;
    }
}

/**
 * @brief 定时器中断回调函数
 *
 * @param htim
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (!init_finished)
    {
        return;
    }

    // 选择回调函数
    if (htim->Instance == TIM4)
    {
        Task10us_Callback();
    }
    else if (htim->Instance == TIM5)
    {
        Task3600s_Callback();
    }
    else if (htim->Instance == TIM6)
    {
        Task1s_Callback();
    }
    else if (htim->Instance == TIM7)
    {
        Task1ms_Callback();
    }
    else if (htim->Instance == TIM8)
    {
        Task125us_Callback();
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
