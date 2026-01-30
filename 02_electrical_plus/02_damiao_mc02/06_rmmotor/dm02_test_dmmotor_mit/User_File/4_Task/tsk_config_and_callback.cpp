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

#include <algorithm>

#include "2_Device/BSP/Power/bsp_power.h"
#include "2_Device/Plotter/Serialplot/dvc_serialplot.h"
#include "1_Middleware/Driver/WDG/drv_wdg.h"
#include "1_Middleware/System/Timestamp/sys_timestamp.h"
#include "2_Device/Motor/Motor_DM/dvc_motor_dm.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// 全局初始化完成标志位
bool init_finished = false;
// 串口绘图模块
Class_Serialplot_USB Serialplot_USB;
// 电机模块
Class_Motor_DM_Normal Motor_4310;

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

void CAN1_Callback(FDCAN_RxHeaderTypeDef &Header, uint8_t *Buffer)
{
    if (!init_finished)
    {
        return;
    }

    switch (Header.Identifier)
    {
    case 0x00:
    {
        Motor_4310.CAN_RxCpltCallback();
    }
    break;
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
    // 检测模块存活
    static int16_t mod100 = 0;
    mod100++;
    if (mod100 >= 100)
    {
        mod100 = 0;
        Motor_4310.TIM_100ms_Alive_PeriodElapsedCallback();
    }

    static int counter = 0;
    counter++;
    Motor_4310.Set_K_P(5.0f);
    Motor_4310.Set_K_D(1.0f);
    Motor_4310.Set_Control_Angle(PI * sinf(5.0f * counter / 1000.0f));
    // 前馈角速度是角度的一阶导数
    Motor_4310.Set_Control_Omega(5.0f * PI * cosf(5.0f * counter / 1000.0f));
    // 前馈力矩是角度的二阶导数乘以转动惯量(经测试发现转动惯量约为0.003kg·m²)
    Motor_4310.Set_Control_Torque(-0.003f * 25.0f * PI * sinf(5.0f * counter / 1000.0f));

    float target_angle = Motor_4310.Get_Control_Angle();
    float now_angle = Motor_4310.Get_Now_Angle();
    float target_omega = Motor_4310.Get_Control_Omega();
    float now_omega = Motor_4310.Get_Now_Omega();
    float target_torque = Motor_4310.Get_Control_Torque();
    float now_torque = Motor_4310.Get_Now_Torque();
    Serialplot_USB.Set_Data(6, &target_angle, &now_angle, &target_omega, &now_omega, &target_torque, &now_torque);
    Serialplot_USB.TIM_1ms_Write_PeriodElapsedCallback();

    Motor_4310.TIM_Send_PeriodElapsedCallback();

    TIM_1ms_CAN_PeriodElapsedCallback();
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

    CAN_Init(&hfdcan1, CAN1_Callback);
    USB_Init(nullptr);

    BSP_Power.Init(false, false, true);
    Motor_4310.Init(&hfdcan1);
    Serialplot_USB.Init();

    // 标记初始化完成
    init_finished = true;

    // 喂狗
    IWDG_Independent_Feed();

    // 定时器中断初始化
    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_Start_IT(&htim5);
    HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_Base_Start_IT(&htim7);
    HAL_TIM_Base_Start_IT(&htim8);
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
