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
char Serialplot_Variable_Assignment_List[][VOFA_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH] = {"q00", "q11", "r00", "r11",};

// 大疆电机
Class_Motor_DJI_GM6020 motor;

// Kalman滤波器
Class_Filter_Kalman filter_kalman;
// 相关矩阵
Class_Matrix_f32<2, 2> A;
Class_Matrix_f32<2, 1> B;
Class_Matrix_f32<2, 2> H;
Class_Matrix_f32<2, 2> Q;
Class_Matrix_f32<2, 2> R;
Class_Matrix_f32<2, 2> P;

// 全局初始化完成标志位
bool init_finished = false;

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief USB虚拟串口接收回调函数
 *
 * @param Buffer 接收缓冲区
 * @param Length 接收数据长度
 */
void Serial_USB_Call_Back(uint8_t *Buffer, uint16_t Length)
{
    Serialplot_USB.USB_RxCallback(Buffer, Length);
    int32_t index = Serialplot_USB.Get_Variable_Index();
    switch (index)
    {
    case (0):
    {
        filter_kalman.Matrix_Q[0][0] = Serialplot_USB.Get_Variable_Value();
        break;
    }
    case (1):
    {
        filter_kalman.Matrix_Q[1][1] = Serialplot_USB.Get_Variable_Value();
        break;
    }
    case (2):
    {
        filter_kalman.Matrix_R[0][0] = Serialplot_USB.Get_Variable_Value();
        break;
    }
    case (3):
    {
        filter_kalman.Matrix_R[1][1] = Serialplot_USB.Get_Variable_Value();
        break;
    }
    default:
    {
        break;
    }
    }
}

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
    case (0x205):
    {
        motor.CAN_RxCpltCallback();

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

        motor.TIM_100ms_Alive_PeriodElapsedCallback();
    }

    static uint32_t Counter;
    Counter++;
    motor.Set_Target_Torque(sinf(Counter * 0.001f * 20.0f) * 0.1f);
    motor.TIM_Calculate_PeriodElapsedCallback();
    TIM_1ms_CAN_PeriodElapsedCallback();

    filter_kalman.Vector_Z[0][0] = motor.Get_Now_Angle();
    filter_kalman.Vector_Z[1][0] = motor.Get_Now_Omega();
    filter_kalman.TIM_Predict_PeriodElapsedCallback();
    filter_kalman.TIM_Update_PeriodElapsedCallback();

    // Vofa+显示内容
    float now_angle_6020 = motor.Get_Now_Angle();
    float now_omega_6020 = motor.Get_Now_Omega();
    float now_filter_angle = filter_kalman.Vector_X[0][0];
    float now_filter_omega = filter_kalman.Vector_X[1][0];
    float p00 = filter_kalman.Matrix_P[0][0];
    float p01 = filter_kalman.Matrix_P[0][1];
    float p10 = filter_kalman.Matrix_P[1][0];
    float p11 = filter_kalman.Matrix_P[1][1];
    Serialplot_USB.Set_Data(8, &now_angle_6020, &now_omega_6020, &now_filter_angle, &now_filter_omega, &p00, &p01, &p10, &p11);
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
    USB_Init(Serial_USB_Call_Back);
    // 大疆电机的CAN
    CAN_Init(&hfdcan1, Can_Callback_Function);

    // Vofa初始化
    Serialplot_USB.Init(Serialplot_Checksum_8_ENABLE, 4, reinterpret_cast<const char **>(Serialplot_Variable_Assignment_List));

    // 大疆电机
    motor.Init(&hfdcan1, Motor_DJI_ID_0x205, Motor_DJI_Control_Method_TORQUE);

    // Kalman滤波器
    A[0][0] = 1.0f;
    A[0][1] = 0.001f;
    A[1][0] = 0.0f;
    A[1][1] = 1.0f;
    B[0][0] = 0.0f;
    B[1][0] = 0.0f;
    H[0][0] = 1.0f;
    H[0][1] = 0.0f;
    H[1][0] = 0.0f;
    H[1][1] = 1.0f;
    Q[0][0] = 0.1f;
    Q[0][1] = 0.0f;
    Q[1][0] = 0.0f;
    Q[1][1] = 0.1f;
    R[0][0] = 0.000767f;
    R[0][1] = 0.0f;
    R[1][0] = 0.0f;
    R[1][1] = 0.105f;
    P[0][0] = 1.0f;
    P[0][1] = 0.0f;
    P[1][0] = 0.0f;
    P[1][1] = 1.0f;
    filter_kalman.Init(A, B, H, Q, R, P);

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
