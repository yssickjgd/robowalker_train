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

/**
 * @brief 注意, 每个类的对象分为专属对象Specialized, 同类可复用对象Reusable以及通用对象Generic
 *
 * 专属对象:
 * 单对单来独打独
 * 比如交互类的底盘对象, 只需要交互对象调用且全局只有一个, 这样看来, 底盘就是交互类的专属对象
 * 这种对象直接封装在上层类里面, 初始化在上层类里面, 调用在上层类里面
 *
 * 同类可复用对象:
 * 各调各的
 * 比如电机的对象, 底盘可以调用, 云台可以调用, 而两者调用的是不同的对象, 这种就是同类可复用对象
 * 电机的pid对象也算同类可复用对象, 它们都在底盘类里初始化
 * 这种对象直接封装在上层类里面, 初始化在最近的一个上层专属对象的类里面, 调用在上层类里面
 *
 * 通用对象:
 * 多个调用同一个
 * 比如底盘陀螺仪对象, 底盘类要调用它做小陀螺, 云台要调用它做方位感知, 因此底盘陀螺仪是通用对象.
 * 这种对象以指针形式进行指定, 初始化在包含所有调用它的上层的类里面, 调用在上层类里面
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "tsk_config_and_callback.h"

#include "4_Interaction/ita_robot.h"
#include "2_Device/Serialplot/dvc_serialplot.h"
#include "1_Middleware/1_Driver/BSP/drv_djiboarda.h"
#include "1_Middleware/1_Driver/TIM/drv_tim.h"
#include "1_Middleware/1_Driver/WDG/drv_wdg.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// 全局初始化完成标志位
bool init_finished = false;
uint32_t flag = 0;

// 机器人战车!!!
Class_Robot robot;

// 串口绘图
Class_Serialplot serialplot;
static char Serialplot_Variable_Assignment_List[][SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH] = {
        // 电机调PID
        "pa",
        "ia",
        "da",
        "po",
        "io",
        "do",};

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief CAN1回调函数
 *
 * @param CAN_RxMessage CAN1收到的消息
 */
void Device_CAN1_Callback(Struct_CAN_Rx_Buffer *CAN_RxMessage)
{
    switch (CAN_RxMessage->Header.StdId)
    {
    case (0x202):
    {
        robot.Booster.Motor_Driver.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x203):
    {
        robot.Booster.Motor_Friction_Right.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x204):
    {
        robot.Booster.Motor_Friction_Left.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x205):
    {
        robot.Gimbal.Motor_Yaw.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x206):
    {
        robot.Gimbal.Motor_Pitch.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    }
}

/**
 * @brief CAN2回调函数
 *
 * @param CAN_RxMessage CAN2收到的消息
 */
void Device_CAN2_Callback(Struct_CAN_Rx_Buffer *CAN_RxMessage)
{
    switch (CAN_RxMessage->Header.StdId)
    {
    case (0x030):
    {
        robot.Supercap.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x201):
    {
        robot.Chassis.Motor_Wheel[2].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x202):
    {
        robot.Chassis.Motor_Wheel[3].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x203):
    {
        robot.Chassis.Motor_Wheel[0].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x204):
    {
        robot.Chassis.Motor_Wheel[1].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x205):
    {
        robot.Chassis.Motor_Steer[2].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x206):
    {
        robot.Chassis.Motor_Steer[3].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x207):
    {
        robot.Chassis.Motor_Steer[0].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x208):
    {
        robot.Chassis.Motor_Steer[1].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    }
}

/**
 * @brief UART1遥控器回调函数
 *
 * @param Buffer UART1收到的消息
 * @param Length 长度
 */
void DR16_UART1_Callback(uint8_t *Buffer, uint16_t Length)
{
    robot.DR16.UART_RxCpltCallback(Buffer, Length);
}

/**
 * @brief UART2串口绘图回调函数
 *
 * @param Buffer UART2收到的消息
 * @param Length 长度
 */
void Serialplot_UART2_Callback(uint8_t *Buffer, uint16_t Length)
{
    serialplot.UART_RxCpltCallback(Buffer, Length);
    // 电机调PID
    switch (serialplot.Get_Variable_Index())
    {
    case (0):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Angle.Set_K_P(serialplot.Get_Variable_Value());

        break;
    }
    case (1):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Angle.Set_K_I(serialplot.Get_Variable_Value());

        break;
    }
    case (2):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Angle.Set_K_D(serialplot.Get_Variable_Value());

        break;
    }
    case (3):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Omega.Set_K_P(serialplot.Get_Variable_Value());

        break;
    }
    case (4):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Omega.Set_K_I(serialplot.Get_Variable_Value());

        break;
    }
    case (5):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Omega.Set_K_D(serialplot.Get_Variable_Value());

        break;
    }
    }
}

/**
 * @brief UART3妙算回调函数
 *
 * @param Buffer UART3收到的消息
 * @param Length 长度
 */
void Manifold_UART3_Callback(uint8_t *Buffer, uint16_t Length)
{
    robot.Manifold.UART_RxCpltCallback(Buffer, Length);
}

/**
 * @brief UART6裁判系统回调函数
 *
 * @param Buffer UART6收到的消息
 * @param Length 长度
 */
void Referee_UART6_Callback(uint8_t *Buffer, uint16_t Length)
{
    robot.Referee.UART_RxCpltCallback(Buffer, Length);
}

/**
 * @brief UART7底盘姿态传感器回调函数
 *
 * @param Buffer UART7收到的消息
 * @param Length 长度
 */
void Chassis_AHRS_UART7_Callback(uint8_t *Buffer, uint16_t Length)
{
    robot.Posture.AHRS_Chassis.UART_RxCpltCallback(Buffer, Length);
}

/**
 * @brief UART8云台姿态传感器回调函数
 *
 * @param Buffer UART8收到的消息
 * @param Length 长度
 */
void Gimbal_AHRS_UART8_Callback(uint8_t *Buffer, uint16_t Length)
{
    robot.Posture.AHRS_Gimbal.UART_RxCpltCallback(Buffer, Length);
}

/**
 * @brief TIM4任务回调函数
 *
 */
void Task100us_TIM4_Callback()
{

}

/**
 * @brief TIM5任务回调函数
 *
 */
void Task1ms_TIM5_Callback()
{
    // 模块保持存活

    static int alive_mod100 = 0;
    alive_mod100++;
    if (alive_mod100 == 100)
    {
        alive_mod100 = 0;

        robot.TIM_100ms_Alive_PeriodElapsedCallback();
    }

    static int alive_mod1000 = 0;
    alive_mod1000++;
    if (alive_mod1000 == 1000)
    {
        alive_mod1000 = 0;

        robot.TIM_1000ms_Alive_PeriodElapsedCallback();
    }

    // 交互层回调函数

    static int interaction_mod100 = 0;
    interaction_mod100++;
    if (interaction_mod100 == 100)
    {
        interaction_mod100 = 0;

        robot.TIM_100ms_Calculate_Callback();
    }

    static int interaction_mod10 = 0;
    interaction_mod10++;
    if (interaction_mod10 == 10)
    {
        interaction_mod10 = 0;

        robot.TIM_10ms_Calculate_PeriodElapsedCallback();
    }

    static int interaction_mod2 = 0;
    interaction_mod2++;
    if (interaction_mod2 == 2)
    {
        interaction_mod2 = 0;

        robot.TIM_2ms_Calculate_PeriodElapsedCallback();
    }

    robot.TIM_1ms_Calculate_Callback();

    // 设备层回调函数

    // 模块在线情况调试
    // float t0 = robot.Chassis.Motor_Wheel[0].Get_Status();
    // float t1 = robot.Chassis.Motor_Wheel[1].Get_Status();
    // float t2 = robot.Chassis.Motor_Wheel[2].Get_Status();
    // float t3 = robot.Chassis.Motor_Wheel[3].Get_Status();
    // float s0 = robot.Chassis.Motor_Steer[0].Get_Status();
    // float s1 = robot.Chassis.Motor_Steer[1].Get_Status();
    // float s2 = robot.Chassis.Motor_Steer[2].Get_Status();
    // float s3 = robot.Chassis.Motor_Steer[3].Get_Status();
    // float yaw = robot.Gimbal.Motor_Yaw.Get_Status();
    // float pitch = robot.Gimbal.Motor_Pitch.Get_Status();
    // float driver = robot.Booster.Motor_Driver.Get_Status();
    // float frictionr = robot.Booster.Motor_Friction_Right.Get_Status();
    // float frictionl = robot.Booster.Motor_Friction_Left.Get_Status();
    // float ahrschassis = robot.Posture.AHRS_Chassis.Get_Status();
    // float ahrsgimbal = robot.Posture.AHRS_Gimbal.Get_Status();
    // float supercap = robot.Supercap.Get_Status();
    // float vision = robot.Manifold.Get_Status();
    // serialplot.Set_Data(1, &vision);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 调PID
    // float ta = robot.Gimbal.Motor_Yaw.Get_Target_Angle() - robot.Gimbal.Motor_Yaw.Get_Now_Angle();
    // float na = robot.Gimbal.Motor_Yaw.Get_Now_Angle();
    // float to = robot.Gimbal.Motor_Yaw.Get_Target_Omega() + robot.Gimbal.Motor_Yaw.Get_Feedforward_Omega();
    // float nom = robot.Gimbal.Motor_Yaw.Get_Now_Omega();
    // float noa = robot.Gimbal.Motor_Yaw.Get_Now_AHRS_Omega();
    // float tc = robot.Gimbal.Motor_Yaw.Get_Target_Current();
    // float nc = robot.Gimbal.Motor_Yaw.Get_Now_Current();
    // serialplot.Set_Data(7, &ta, &na, &to, &nom, &noa, &nc, &tc);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 底盘舵向电机角度标定
    // float motor0 = robot.Chassis.Motor_Steer[0].Get_Now_Angle();
    // float motor1 = robot.Chassis.Motor_Steer[1].Get_Now_Angle();
    // float motor2 = robot.Chassis.Motor_Steer[2].Get_Now_Angle();
    // float motor3 = robot.Chassis.Motor_Steer[3].Get_Now_Angle();
    // float vx = robot.Posture.Get_Chassis_Velocity_X();
    // float vy = robot.Posture.Get_Chassis_Velocity_Y();
    // float omega = robot.Posture.Get_Chassis_Omega();
    // serialplot.Set_Data(7, &motor0, &motor1, &motor2, &motor3, &vx, &vy, &omega);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 底盘陀螺仪标定
    // float yaw = robot.Posture.Get_Chassis_Odom_Angle_Yaw();
    // float pitch = robot.Posture.Get_Chassis_Odom_Angle_Pitch();
    // float roll = robot.Posture.Get_Chassis_Odom_Angle_Roll();
    // float omegaz = robot.Posture.AHRS_Chassis.Get_Omega_Z();
    // float omegay = robot.Posture.AHRS_Chassis.Get_Omega_Y();
    // float omegax = robot.Posture.AHRS_Chassis.Get_Omega_X();
    // float slope_x = -arm_sin_f32(pitch) * arm_cos_f32(roll);
    // float slope_y = arm_sin_f32(roll);
    // float slope_z = arm_cos_f32(pitch);
    // float modulus = slope_x * slope_x + slope_y * slope_y + slope_z * slope_z;
    // serialplot.Set_Data(10, &yaw, &pitch, &roll, &omegaz, &omegay, &omegax, &slope_x, &slope_y, &slope_z, &modulus);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 底盘速度解算
    // float vx = robot.Chassis.Get_Target_Velocity_X();
    // float vy = robot.Chassis.Get_Target_Velocity_Y();
    // float noa = robot.Chassis.Get_Now_AHRS_Omega();
    // float nom = robot.Chassis.Get_Now_Omega();
    // float wit = -robot.Gimbal.Motor_Yaw.Get_Now_AHRS_Omega();
    // serialplot.Set_Data(5, &vx, &vy, &noa, &nom, &wit);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 底盘电机运动姿态
    // float tx = robot.Chassis.Get_Target_Velocity_X();
    // float ty = robot.Chassis.Get_Target_Velocity_Y();
    // float to = robot.Chassis.Get_Target_Omega();
    // float nx = robot.Chassis.Get_Now_Velocity_X();
    // float ny = robot.Chassis.Get_Now_Velocity_Y();
    // float no = robot.Chassis.Get_Now_Omega();
    // serialplot.Set_Data(6, &tx, &ty, &to, &nx, &ny, &no);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 云台电机角度标定
    // float motoryaw = robot.Gimbal.Motor_Yaw.Get_Now_Angle();
    // float motorpitch = robot.Gimbal.Motor_Pitch.Get_Now_Angle();
    // float omegax = robot.Gimbal.AHRS_Gimbal->Get_Omega_X();
    // float omegay = robot.Gimbal.AHRS_Gimbal->Get_Omega_Y();
    // float omegaz = robot.Gimbal.AHRS_Gimbal->Get_Omega_Z();
    // serialplot.Set_Data(5, &motoryaw, &motorpitch, &omegax, &omegay, &omegaz);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 云台自解算标定
    // float yawangle = robot.Gimbal.Get_Now_Yaw_Angle();
    // float pitchangle = robot.Gimbal.Get_Now_Pitch_Angle();
    // serialplot.Set_Data(2, &yawangle, &pitchangle);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 视觉调试
    // float vision = robot.Manifold.Get_Status();
    // float shoot = robot.Manifold.Get_Shoot_Flag();
    // float target_yaw_angle = robot.Manifold.Get_Target_Gimbal_Yaw();
    // float target_yaw_inc = robot.Manifold.Get_Gimbal_Yaw_Angle_Increment();
    // float target_yaw_omega = robot.Manifold.Get_Gimbal_Yaw_Omega_FeedForward();
    // float target_pitch_angle = robot.Manifold.Get_Target_Gimbal_Pitch();
    // float target_pitch_inc = robot.Manifold.Get_Gimbal_Pitch_Angle_Increment();
    // float target_pitch_omega = robot.Manifold.Get_Gimbal_Pitch_Omega_FeedForward();
    // float enemy = robot.Manifold.Get_Enemy_ID();
    // float Confidence_Level = robot.Manifold.Get_Confidence_Level();
    // serialplot.Set_Data(10, &vision, &shoot, &target_yaw_angle, &target_yaw_inc, &target_yaw_omega, &target_pitch_angle, &target_pitch_inc, &target_pitch_omega, &enemy, &Confidence_Level);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 超级电容标定
    // float energy = robot.Supercap.Get_Now_Energy();
    // float power = robot.Referee.Get_Chassis_Power();
    // float referee_source = robot.Referee.Get_Self_PM01_Chassis_Status();
    // serialplot.Set_Data(3, &energy, &power, &referee_source);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 功率控制调PID
    // float wheel0 = robot.Chassis.Motor_Wheel[0].Power_Factor;
    // float wheel1 = robot.Chassis.Motor_Wheel[1].Power_Factor;
    // float wheel2 = robot.Chassis.Motor_Wheel[2].Power_Factor;
    // float wheel3 = robot.Chassis.Motor_Wheel[3].Power_Factor;
    // float omega = robot.AHRS_Chassis.Get_Omega_Yaw();
    // serialplot.Set_Data(5, &wheel0, &wheel1, &wheel2, &wheel3, &omega);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 功率控制调试
    // float power_chassis = robot.Chassis.Get_Now_Motor_Power();
    // float power_max = robot.Referee.Get_Self_Chassis_Power_Max();
    // float referee_power = robot.Referee.Get_Chassis_Power();
    // float supercap_power = robot.Supercap.Get_Chassis_Power();
    // float nowomega = robot.Chassis.Get_Now_AHRS_Omega();
    // float targetomega = robot.Chassis.Get_Target_Omega();
    // serialplot.Set_Data(6, &power_chassis, &power_max, &referee_power, &supercap_power, &nowomega, &targetomega);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 姿态感知器调试
    // float c00 = robot.Posture.Matrix_Chassis_Odom_Rotation(0, 0);
    // float c01 = robot.Posture.Matrix_Chassis_Odom_Rotation(0, 1);
    // float c02 = robot.Posture.Matrix_Chassis_Odom_Rotation(0, 2);
    // float c10 = robot.Posture.Matrix_Chassis_Odom_Rotation(1, 0);
    // float c11 = robot.Posture.Matrix_Chassis_Odom_Rotation(1, 1);
    // float c12 = robot.Posture.Matrix_Chassis_Odom_Rotation(1, 2);
    // float c20 = robot.Posture.Matrix_Chassis_Odom_Rotation(2, 0);
    // float c21 = robot.Posture.Matrix_Chassis_Odom_Rotation(2, 1);
    // float c22 = robot.Posture.Matrix_Chassis_Odom_Rotation(2, 2);
    // serialplot.Set_Data(9, &c00, &c01, &c02, &c10, &c11, &c12, &c20, &c21, &c22);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 发射机构调试
    // float fsm_status = robot.FSM_Heat_Detector.Get_Now_Status_Serial();
    // float fsm_ammo = robot.FSM_Heat_Detector.Get_Total_Ammo_Num();
    // float fsm_heat = robot.FSM_Heat_Detector.Get_Now_Heat();
    // float referee_heat = robot.Referee.Get_Booster_17mm_1_Heat();
    // float current = robot.Booster.Motor_Friction_Left.Get_Now_Current();
    // float omega = robot.Booster.Motor_Friction_Left.Get_Target_Omega();
    // serialplot.Set_Data(6, &fsm_status, &fsm_ammo, &fsm_heat, &referee_heat, &current, &omega);
    // serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 临时调试
    float floatflag = flag;
    float structorigin = (float)(sizeof(Struct_Referee_Data_Interaction_Graphic_Config));
    serialplot.Set_Data(2, &floatflag, &structorigin);
    serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 驱动层回调函数

    TIM_1ms_CAN_PeriodElapsedCallback();
    TIM_1ms_UART_PeriodElapsedCallback();
    TIM_1ms_IWDG_PeriodElapsedCallback();
    flag++;
}

/**
 * @brief 初始化任务
 *
 */
void Task_Init()
{
    // 驱动层初始化

    // 点俩灯, 开24V
    BSP_Init(BSP_LED_R_ON | BSP_LED_G_ON);
    // CAN总线初始化
    CAN_Init(&hcan1, Device_CAN1_Callback);
    CAN_Init(&hcan2, Device_CAN2_Callback);
    // UART初始化
    UART_Init(&huart1, DR16_UART1_Callback, 36);
    UART_Init(&huart2, Serialplot_UART2_Callback, SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH);
    UART_Init(&huart3, Manifold_UART3_Callback, 128);
    UART_Init(&huart6, Referee_UART6_Callback, 512);
    UART_Init(&huart7, Chassis_AHRS_UART7_Callback, 128);
    UART_Init(&huart8, Gimbal_AHRS_UART8_Callback, 128);
    // 定时器初始化
    TIM_Init(&htim4, Task100us_TIM4_Callback);
    TIM_Init(&htim5, Task1ms_TIM5_Callback);
    // 喂狗
    IWDG_Independent_Feed();

    // 设备层初始化

    // 串口绘图初始化
    serialplot.Init(&huart2, 9, (char **) Serialplot_Variable_Assignment_List);

    // 战车层初始化

    // 交互层初始化

    // 机器人战车初始化
    robot.Init();

    // 使能调度时钟
    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_Start_IT(&htim5);
    // 标记初始化完成
    init_finished = true;

    // 等待系统
    HAL_Delay(2000);
}

/**
 * @brief 前台循环任务
 *
 */
void Task_Loop()
{
    robot.Loop();
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
