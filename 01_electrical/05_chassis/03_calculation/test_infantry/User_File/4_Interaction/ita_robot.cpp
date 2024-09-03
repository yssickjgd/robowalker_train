/**
 * @file ita_robot.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 人机交互控制逻辑
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-01-17 1.1 更名为ita_robot.h, 引入新功能
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "ita_robot.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// 功率优先功率打表
static const float chassis_power_first_power[10] = {60.0f,
                                                    65.0f,
                                                    70.0f,
                                                    75.0f,
                                                    80.0f,
                                                    85.0f,
                                                    90.0f,
                                                    95.0f,
                                                    100.0f,
                                                    100.0f,};

// 血量优先功率打表
static const float chassis_hp_first_power[10] = {45.0f,
                                                 50.0f,
                                                 55.0f,
                                                 60.0f,
                                                 65.0f,
                                                 70.0f,
                                                 75.0f,
                                                 80.0f,
                                                 90.0f,
                                                 100.0f,};

// 爆发优先热量打表
static const float booster_burst_first_heat_max[10] = {200.0f,
                                                       250.0f,
                                                       300.0f,
                                                       350.0f,
                                                       400.0f,
                                                       450.0f,
                                                       500.0f,
                                                       550.0f,
                                                       600.0f,
                                                       650.0f,};

// 爆发优先冷却打表
static const float booster_burst_first_heat_cd[10] = {10.0f,
                                                      15.0f,
                                                      20.0f,
                                                      25.0f,
                                                      30.0f,
                                                      35.0f,
                                                      40.0f,
                                                      45.0f,
                                                      50.0f,
                                                      60.0f,};

// 冷却优先热量打表
static const float booster_cd_first_heat_max[10] = {50.0f,
                                                    85.0f,
                                                    120.0f,
                                                    155.0f,
                                                    190.0f,
                                                    225.0f,
                                                    260.0f,
                                                    295.0f,
                                                    330.0f,
                                                    400.0f,};

// 冷却优先冷却打表
static const float booster_cd_first_heat_cd[10] = {40.0f,
                                                   45.0f,
                                                   50.0f,
                                                   55.0f,
                                                   60.0f,
                                                   65.0f,
                                                   70.0f,
                                                   75.0f,
                                                   80.0f,
                                                   80.0f,};

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 长按左键有限自动机
 *
 */
void Class_FSM_Press_Hold::TIM_1ms_Calculate_PeriodElapsedCallback()
{
    Status[Now_Status_Serial].Count_Time++;

    // 自己接着编写状态转移函数
    switch (Now_Status_Serial)
    {
    case (FSM_Press_Hold_Status_STOP):
    {
        // 停机状态

        if (Robot->DR16.Get_Mouse_Left_Key() == DR16_Key_Status_TRIG_FREE_PRESSED)
        {
            // 停机->按下状态
            Set_Status(FSM_Press_Hold_Status_PRESSED);
        }

        break;
    }
    case (FSM_Press_Hold_Status_FREE):
    {
        // 放开状态

        if (Robot->DR16.Get_Mouse_Left_Key() == DR16_Key_Status_TRIG_FREE_PRESSED)
        {
            // 松开->按下状态
            Set_Status(FSM_Press_Hold_Status_PRESSED);
        }

        break;
    }
    case (FSM_Press_Hold_Status_PRESSED):
    {
        // 按下状态

        if (Robot->DR16.Get_Mouse_Left_Key() == DR16_Key_Status_TRIG_PRESSED_FREE)
        {
            // 短时间长按->单点状态

            Set_Status(FSM_Press_Hold_Status_CLICK);
        }
        if (Status[Now_Status_Serial].Count_Time >= Hold_Time_Threshold)
        {
            // 长时间按下->长按状态
            Set_Status(FSM_Press_Hold_Status_HOLD);
        }

        break;
    }
    case (FSM_Press_Hold_Status_CLICK):
    {
        // 单点状态

        Set_Status(FSM_Press_Hold_Status_FREE);

        break;
    }
    case (FSM_Press_Hold_Status_HOLD):
    {
        // 长按状态
        Hold_Time_Threshold = Default_Hold_Time_Threshold;

        if (Robot->DR16.Get_Mouse_Left_Key() == DR16_Key_Status_FREE)
        {
            Set_Status(FSM_Press_Hold_Status_FREE);
        }

        break;
    }
    }
}

/**
 * @brief 热量自检测逻辑
 *
 */
void Class_FSM_Heat_Detector::TIM_1ms_Calculate_PeriodElapsedCallback()
{
    Status[Now_Status_Serial].Count_Time++;

    //自己接着编写状态转移函数
    switch (Now_Status_Serial)
    {
    case (FSM_Heat_Detector_Status_CLOSE):
    {
        // 停机状态

        if ((Robot->Booster.Motor_Friction_Left.Get_Status() == Motor_DJI_Status_ENABLE && Robot->Booster.Motor_Friction_Right.Get_Status() == Motor_DJI_Status_ENABLE) && (Robot->Booster.Motor_Friction_Left.Get_Target_Omega() > 0.0f && Robot->Booster.Motor_Friction_Right.Get_Target_Omega() < 0.0f) && (Robot->Booster.Motor_Friction_Left.Get_Now_Filter_Omega() >= Robot->Booster.Motor_Friction_Left.Get_Target_Omega() * 0.99f && Robot->Booster.Motor_Friction_Right.Get_Now_Filter_Omega() <= Robot->Booster.Motor_Friction_Right.Get_Target_Omega() * 0.99f))
        {
            // 摩擦轮达到了目标速度且电机存活->开机状态
            Set_Status(FSM_Heat_Detector_Status_OPEN);
        }

        break;
    }
    case (FSM_Heat_Detector_Status_OPEN):
    {
        // 开机状态

        float now_current = Robot->Booster.Motor_Friction_Left.Get_Now_Current() - Robot->Booster.Motor_Friction_Right.Get_Now_Current();
        Current_Queue_Sum += now_current;
        Current_Queue.Push(now_current);

        // 计算窗口内电流和
        if (Current_Queue.Get_Length() > Max_Queue_Size)
        {
            Current_Queue_Sum -= Current_Queue.Get_Front();
            Current_Queue.Pop();
        }

        // 如果超过了判定阈值则认为是打出子弹
        if (Current_Queue_Sum > Current_Queue_Sum_Threshold)
        {
            Current_Queue.Clear();
            Current_Queue_Sum = 0.0f;
            Now_Heat += 10.0f;
            Total_Ammo_Num++;
        }

        // 计算冷却
        Now_Heat -= (float) (Robot->Referee.Get_Self_Booster_Heat_CD()) * 0.001f;
        if (Now_Heat < 0.0f)
        {
            Now_Heat = 0.0f;
        }

        if ((Robot->Booster.Motor_Friction_Left.Get_Status() == Motor_DJI_Status_DISABLE && Robot->Booster.Motor_Friction_Right.Get_Status() == Motor_DJI_Status_DISABLE) || (Robot->Booster.Motor_Friction_Left.Get_Target_Omega() == 0.0f && Robot->Booster.Motor_Friction_Right.Get_Target_Omega() == 0.0f))
        {
            // 电机掉线->关机状态

            Current_Queue.Clear();
            Current_Queue_Sum = 0.0f;
            Now_Heat = 0.0f;

            Set_Status(FSM_Heat_Detector_Status_CLOSE);
        }

        break;
    }
    }
}

/**
 * @brief 控制交互端初始化
 *
 */
void Class_Robot::Init()
{
    // 未按过状态, 松开状态, 按下状态, 长按状态
    FSM_DR16_Left_Mouse_Press_Hold.Robot = this;
    FSM_DR16_Left_Mouse_Press_Hold.Init(4, FSM_Press_Hold_Status_STOP);

    // 热量自检测逻辑初始化
    FSM_Heat_Detector.Robot = this;
    FSM_Heat_Detector.Init(2, FSM_Heat_Detector_Status_CLOSE);

    // 一键调头定时器初始化
    Timer_Turn_About.Init(3000);

    // 视觉切换目标定时器初始化
    Timer_Switch_Target.Init(1);

    // 能量机关切换回装甲板定时器初始化
    Timer_Rune_To_Armor.Init(1);

    // 底盘跟随PID初始化, 纯P控制
    PID_Chassis_Follow.Init(3.0f, 0.0f, 0.0f, 0.0f, 4.0f * PI, 4.0f * PI);

    // 底盘超级电容功率控制PID初始化
    PID_Supercap_Chassis_Power.Init(0.10f, 1.5f, 0.0f, 0.0f, 120.0f, 120.0f, 0.1f, 0.0f, 0.0f, 0.0f, 45.0f);

    // 底盘裁判系统功率控制PID初始化
    PID_Referee_Chassis_Power.Init(0.05f, 1.5f, 0.0f, 0.0f, 120.0f, 120.0f, 0.1f, 0.0f, 0.0f, 0.0f, 45.0f);

    // 底盘斜坡函数初始化
    Slope_Speed_X.Init(2.5f / 1000.0f, 3.0f / 1000.0f);
    Slope_Speed_Y.Init(2.5f / 1000.0f, 5.0f / 1000.0f);
    Slope_Omega.Init(4.0f * PI / 1000.0f, 4.0f * PI / 1000.0f);

    // 遥控器初始化
    DR16.Init(&huart1);

    // 视觉Manifold初始化
    Manifold.Init(&huart3);

    // 超级电容初始化
    Supercap.Init(&hcan2);

    // 裁判系统
    Referee.Init(&huart6);

    // 底盘初始化
    Chassis.AHRS_Chassis = &(Posture.AHRS_Chassis);
    Chassis.Init();

    // 云台初始化
    Gimbal.AHRS_Gimbal = &(Posture.AHRS_Gimbal);
    Gimbal.AHRS_Chassis = &(Posture.AHRS_Chassis);
    Gimbal.Init();

    // 发射机构初始化
    Booster.Init();

    // 姿态感知器初始化
    Posture.Chassis = &Chassis;
    Posture.Gimbal = &Gimbal;
    Posture.Init();
}

/**
 * @brief 控制交互端循环任务
 *
 */
void Class_Robot::Loop()
{
    if (UI_Init_Status == false)
    {
        Referee.Set_Referee_UI_Change_Flag_Clear();
        Referee_Draw_Static_UI();
        UI_Init_Status = true;
    }
    else
    {
        Referee_Draw_Dynamic_UI();
    }
}

/**
 * @brief TIM定时器中断定期检测模块是否存活
 *
 */
void Class_Robot::TIM_1000ms_Alive_PeriodElapsedCallback()
{
    Referee.TIM_1000ms_Alive_PeriodElapsedCallback();
    Manifold.TIM_1000ms_Alive_PeriodElapsedCallback();
    Supercap.TIM_1000ms_Alive_PeriodElapsedCallback();
}

/**
 * @brief TIM定时器中断定期检测模块是否存活
 *
 */
void Class_Robot::TIM_100ms_Alive_PeriodElapsedCallback()
{
    DR16.TIM_100ms_Alive_PeriodElapsedCallback();

    Chassis.TIM_100ms_Alive_PeriodElapsedCallback();
    Gimbal.TIM_100ms_Alive_PeriodElapsedCallback();
    Booster.TIM_100ms_Alive_PeriodElapsedCallback();

    Posture.TIM_100ms_Alive_PeriodElapsedCallback();
}

/**
 * @brief 定时器计算函数
 *
 */
void Class_Robot::TIM_100ms_Calculate_Callback()
{
    // 底盘功率控制PID
    if (Supercap.Get_Status() == Supercap_24_Status_ENABLE)
    {
        if (Referee.Get_Status() == Referee_Status_DISABLE || Referee.Get_Referee_Trust_Status() == Referee_Data_Status_DISABLE)
        {
            // 裁判系统掉线或远端信息不可信
            if (Robot_Chassis_Type == Robot_Chassis_Type_POWER)
            {
                PID_Supercap_Chassis_Power.Set_Target(chassis_power_first_power[Robot_Level - 1]);
            }
            else if (Robot_Chassis_Type == Robot_Chassis_Type_HP)
            {
                PID_Supercap_Chassis_Power.Set_Target(chassis_hp_first_power[Robot_Level - 1]);
            }
        }
        else
        {
            // 裁判系统可信

            PID_Supercap_Chassis_Power.Set_Target(Referee.Get_Self_Chassis_Power_Max());
        }
        PID_Supercap_Chassis_Power.Set_Now(Supercap.Get_Chassis_Power());
        PID_Supercap_Chassis_Power.TIM_Calculate_PeriodElapsedCallback();
        PID_Referee_Chassis_Power.Set_Integral_Error(0.0f);
    }
    else if (Referee.Get_Status() == Referee_Status_ENABLE && Referee.Get_Referee_Trust_Status() == Referee_Data_Status_ENABLE)
    {
        PID_Referee_Chassis_Power.Set_Target(Referee.Get_Self_Chassis_Power_Max());
        PID_Referee_Chassis_Power.Set_Now(Referee.Get_Chassis_Power());
        PID_Referee_Chassis_Power.TIM_Calculate_PeriodElapsedCallback();
        PID_Supercap_Chassis_Power.Set_Integral_Error(0.0f);
    }
    else
    {
        PID_Supercap_Chassis_Power.Set_Integral_Error(0.0f);
        PID_Referee_Chassis_Power.Set_Integral_Error(0.0f);
    }
}

/**
 * @brief 定时器计算函数
 *
 */
void Class_Robot::TIM_10ms_Calculate_PeriodElapsedCallback()
{
    _Manifold_Control();

    Manifold.TIM_10ms_Send_PeriodElapsedCallback();

    Supercap.TIM_10ms_Send_PeriodElapsedCallback();
}

/**
 * @brief 定时器计算函数
 *
 */
void Class_Robot::TIM_2ms_Calculate_PeriodElapsedCallback()
{
    Chassis.TIM_2ms_Resolution_PeriodElapsedCallback();
    Chassis.TIM_2ms_Control_PeriodElapsedCallback();
}

/**
 * @brief 定时器计算函数
 *
 */
void Class_Robot::TIM_1ms_Calculate_Callback()
{
    // 一键调头定时器
    Timer_Turn_About.TIM_1ms_Calculate_PeriodElapsedCallback();
    // 视觉切换目标定时器
    Timer_Switch_Target.TIM_1ms_Calculate_PeriodElapsedCallback();
    // 能量机关切换回装甲板定时器
    Timer_Rune_To_Armor.TIM_1ms_Calculate_PeriodElapsedCallback();
    // 长按左键有限自动机
    FSM_DR16_Left_Mouse_Press_Hold.TIM_1ms_Calculate_PeriodElapsedCallback();

    // 遥控器处理上升沿下降沿
    DR16.TIM_1ms_Calculate_PeriodElapsedCallback();

    _Status_Control();
    _Supercap_Control();
    _Chassis_Control();
    _Gimbal_Control();
    _Booster_Control();

    Gimbal.TIM_1ms_Resolution_PeriodElapsedCallback();
    Gimbal.TIM_1ms_Control_PeriodElapsedCallback();
    Booster.TIM_1ms_Calculate_PeriodElapsedCallback();
    Posture.TIM_1ms_Calculate_PeriodElapsedCallback();
}

/**
 * @brief 裁判系统画静态UI
 *
 */
void Class_Robot::Referee_Draw_Static_UI()
{
    // 底盘警戒线左
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_chassis_warning_left_line = Referee.Set_Referee_UI_Line(0, 0, Referee_Data_Interaction_Graphic_Color_ORANGE, 5, 400, 0, 660, 540);
    // 底盘警戒线右
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_chassis_warning_right_line = Referee.Set_Referee_UI_Line(0, 1, Referee_Data_Interaction_Graphic_Color_ORANGE, 5, 1550, 0, 1290, 540);
    // 弹道竖直线
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_booster_aiming_straight_line = Referee.Set_Referee_UI_Line(0, 2, Referee_Data_Interaction_Graphic_Color_MAIN, 2, 975, 350, 975, 500);
    // 近战准星直线
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_booster_near_line  = Referee.Set_Referee_UI_Line(0, 3, Referee_Data_Interaction_Graphic_Color_MAIN, 2, 975 - 140, 400, 975 + 140, 400);
    // 中距离准星矩形
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_booster_middle_rectangle = Referee.Set_Referee_UI_Rectangle(0, 4, Referee_Data_Interaction_Graphic_Color_MAIN, 2, 960, 540, 990, 500);
    // 发送当前五个图形
    Referee.UART_Send_Interaction_UI_Graphic_5(graphic_chassis_warning_left_line, graphic_chassis_warning_right_line, graphic_booster_aiming_straight_line, graphic_booster_near_line , graphic_booster_middle_rectangle);
    // 准备发送下一组
    HAL_Delay(34);

    // 云台航向指示直线
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_gimbal_vision_line = Referee.Set_Referee_UI_Line(1, 0, Referee_Data_Interaction_Graphic_Color_MAIN, 4, 960, 100, 960, 150);
    // 裁判系统备用状态标记圆
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_referee_status_circle = Referee.Set_Referee_UI_Circle(2, 1, Referee_Data_Interaction_Graphic_Color_WHITE, 5, 1440 + 7 / 2 + 15, 375, 15);
    // 裁判系统备用颜色标记圆
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_referee_color_status_circle = Referee.Set_Referee_UI_Circle(2, 2, Referee_Data_Interaction_Graphic_Color_WHITE, 5, 1440 + 7 / 2 + 15 * 6, 375, 15);
    // 裁判系统备用底盘标记圆
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_referee_chassis_status_circle = Referee.Set_Referee_UI_Circle(2, 3, Referee_Data_Interaction_Graphic_Color_WHITE, 5, 1440 + 7 / 2 + 15 * 11, 375, 15);
    // 裁判系统备用发射机构标记圆
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_referee_booster_status_circle = Referee.Set_Referee_UI_Circle(2, 4, Referee_Data_Interaction_Graphic_Color_WHITE, 5, 1440 + 7 / 2 + 15 * 15, 375, 15);
    // 发送当前五个图形
    Referee.UART_Send_Interaction_UI_Graphic_5(graphic_gimbal_vision_line, graphic_referee_status_circle, graphic_referee_color_status_circle, graphic_referee_chassis_status_circle, graphic_referee_booster_status_circle);
    // 准备发送下一组
    HAL_Delay(34);

    // 裁判系统备用字符串
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_ref_color_chs_bst_lv_string = Referee.Set_Referee_UI_String(2, 0, Referee_Data_Interaction_Graphic_Color_ORANGE, 3, 1440, 350, 15, 30);
    // 发送当前字符串
    Referee.UART_Send_Interaction_UI_Graphic_String(graphic_ref_color_chs_bst_lv_string, "ref_color_chs_bst_lv");
    // 准备发送下一组
    HAL_Delay(34);
}

/**
 * @brief 裁判系统画动态UI
 *
 */
void Class_Robot::Referee_Draw_Dynamic_UI()
{
    // 底盘速度模式字符串
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_chassis_speed_mode_string;
    // 发送当前字符串
    if(Supercap_Accelerate_Status == true && Supercap_Burst_Mode_Status == true)
    {
        graphic_chassis_speed_mode_string = Referee.Set_Referee_UI_String(3, 0, Referee_Data_Interaction_Graphic_Color_GREEN, 6, 1680, 640, 40, 30);
        Referee.UART_Send_Interaction_UI_Graphic_String(graphic_chassis_speed_mode_string, "burst");
    }
    else if(Supercap_Accelerate_Status == true && Supercap_Burst_Mode_Status == false)
    {
        graphic_chassis_speed_mode_string = Referee.Set_Referee_UI_String(3, 0, Referee_Data_Interaction_Graphic_Color_GREEN, 6, 1680, 640, 40, 30);
        Referee.UART_Send_Interaction_UI_Graphic_String(graphic_chassis_speed_mode_string, "accel");
    }
    else
    {
        graphic_chassis_speed_mode_string = Referee.Set_Referee_UI_String(3, 0, Referee_Data_Interaction_Graphic_Color_ORANGE, 4, 1680, 640, 40, 30);
        Referee.UART_Send_Interaction_UI_Graphic_String(graphic_chassis_speed_mode_string, "normal");
    }
    // 准备发送下一组
    HAL_Delay(34);

    // 底盘姿态模式字符串
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_chassis_posture_mode_string;
    // 发送当前字符串
    if(Chassis_Gyroscope_Mode_Status == Robot_Gyroscope_Type_CLOCKWISE || Chassis_Gyroscope_Mode_Status == Robot_Gyroscope_Type_COUNTERCLOCKWISE)
    {
        // 小陀螺特殊模式
        graphic_chassis_posture_mode_string = Referee.Set_Referee_UI_String(3, 1, Referee_Data_Interaction_Graphic_Color_GREEN, 6, 1680, 560, 40, 30);
        Referee.UART_Send_Interaction_UI_Graphic_String(graphic_chassis_posture_mode_string, "gyro");
    }
    else
    {
        // 否则底盘跟随
        graphic_chassis_posture_mode_string = Referee.Set_Referee_UI_String(3, 1, Referee_Data_Interaction_Graphic_Color_ORANGE, 4, 1680, 560, 40, 30);
        Referee.UART_Send_Interaction_UI_Graphic_String(graphic_chassis_posture_mode_string, "follow");
    }
    // 准备发送下一组
    HAL_Delay(34);

    // 自瞄识别模式字符串
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_autoaiming_mode_string;
    // 发送当前字符串
    if(Manifold_Aiming_Priority == Manifold_Aiming_Priority_RUNE)
    {
        // 能量机关特殊模式
        graphic_autoaiming_mode_string = Referee.Set_Referee_UI_String(7, 1, Referee_Data_Interaction_Graphic_Color_GREEN, 6, 1680, 480, 40, 30);
        Referee.UART_Send_Interaction_UI_Graphic_String(graphic_autoaiming_mode_string, "rune");
    }
    else
    {
        // 否则装甲板
        graphic_autoaiming_mode_string = Referee.Set_Referee_UI_String(7, 1, Referee_Data_Interaction_Graphic_Color_ORANGE, 4, 1680, 480, 40, 30);
        Referee.UART_Send_Interaction_UI_Graphic_String(graphic_autoaiming_mode_string, "armor");
    }
    // 准备发送下一组
    HAL_Delay(34);

    // 底盘功率标记整数
    bool tmp_chassis_alive_flag = true;
    for(int i = 0; i < 4; i++)
    {
        // 底盘掉线
        if(Chassis.Motor_Steer[i].Get_Status() == Motor_DJI_Status_DISABLE || Chassis.Motor_Wheel[i].Get_Status() == Motor_DJI_Status_DISABLE)
        {
            tmp_chassis_alive_flag = false;
            break;
        }
    }
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_chassis_power_integer;
    if(tmp_chassis_alive_flag == false)
    {
        // 底盘掉线
        graphic_chassis_power_integer = Referee.Set_Referee_UI_Integer(3, 2, Referee_Data_Interaction_Graphic_Color_PURPLE, 5, 1720, 750, 50, Referee.Get_Self_Chassis_Power_Max());
    }
    else
    {
        // 底盘在线
        graphic_chassis_power_integer = Referee.Set_Referee_UI_Integer(3, 2, Referee_Data_Interaction_Graphic_Color_GREEN, 5, 1720, 750, 50, Referee.Get_Self_Chassis_Power_Max());
    }
    // 底盘方向指示
    float delta_angle = Gimbal.Get_Now_Yaw_Angle();
    float tmp_x = 50 * arm_cos_f32(PI / 4.0f - delta_angle);
    float tmp_y = 50 * arm_sin_f32(PI / 4.0f - delta_angle);
    // 底盘方向指示线前
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_chassis_status_line_front;
    graphic_chassis_status_line_front = Referee.Set_Referee_UI_Line(3, 3, Referee_Data_Interaction_Graphic_Color_WHITE, 3, uint32_t(960 + tmp_x), uint32_t(100 + tmp_y), uint32_t(960 - tmp_y), uint32_t(100 + tmp_x));
    // 底盘方向指示线后
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_chassis_status_line_back;
    graphic_chassis_status_line_back = Referee.Set_Referee_UI_Line(3, 4, Referee_Data_Interaction_Graphic_Color_MAIN, 3, uint32_t(960 - tmp_x), uint32_t(100 - tmp_y), uint32_t(960 + tmp_y), uint32_t(100 - tmp_x));
    // 底盘方向指示线左
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_chassis_status_line_left;
    graphic_chassis_status_line_left = Referee.Set_Referee_UI_Line(3, 5, Referee_Data_Interaction_Graphic_Color_WHITE, 3, uint32_t(960 - tmp_x), uint32_t(100 - tmp_y), uint32_t(960 - tmp_y), uint32_t(100 + tmp_x));
    // 底盘方向指示线右
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_chassis_status_line_right;
    graphic_chassis_status_line_right = Referee.Set_Referee_UI_Line(3, 6, Referee_Data_Interaction_Graphic_Color_WHITE, 3, uint32_t(960 + tmp_x), uint32_t(100 + tmp_y), uint32_t(960 + tmp_y), uint32_t(100 - tmp_x));
    Referee.UART_Send_Interaction_UI_Graphic_5(graphic_chassis_power_integer, graphic_chassis_status_line_front, graphic_chassis_status_line_back, graphic_chassis_status_line_left, graphic_chassis_status_line_right);
    HAL_Delay(34);

    // 裁判系统备用状态标记圆
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_referee_status_circle;
    if (Referee.Get_Status() == Referee_Status_DISABLE || Referee.Get_Referee_Trust_Status() == Referee_Data_Status_DISABLE)
    {
        // 裁判系统踢出
        graphic_referee_status_circle = Referee.Set_Referee_UI_Circle(5, 0, Referee_Data_Interaction_Graphic_Color_BLACK, 15, 1440 + 7 / 2 + 15, 375, 7);
    }
    else
    {
        // 裁判系统在线
        graphic_referee_status_circle = Referee.Set_Referee_UI_Circle(5, 0, Referee_Data_Interaction_Graphic_Color_MAIN, 15, 1440 + 7 / 2 + 15, 375, 7);
    }
    // 裁判系统备用颜色标记圆
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_referee_color_status_circle;
    if (Manifold_Self_Color == Manifold_Enemy_Color_RED)
    {
        // 自家设定红色
        graphic_referee_color_status_circle = Referee.Set_Referee_UI_Circle(5, 1, Referee_Data_Interaction_Graphic_Color_PINK, 15, 1440 + 7 / 2 + 15 * 6, 375, 7);
    }
    else if (Manifold_Self_Color == Manifold_Enemy_Color_BLUE)
    {
        // 自家设定蓝色
        graphic_referee_color_status_circle = Referee.Set_Referee_UI_Circle(5, 1, Referee_Data_Interaction_Graphic_Color_CYAN, 15, 1440 + 7 / 2 + 15 * 6, 375, 7);
    }
    // 裁判系统备用底盘标记圆
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_referee_chassis_status_circle;
    if (Robot_Chassis_Type == Robot_Chassis_Type_HP)
    {
        // 血量优先
        graphic_referee_chassis_status_circle = Referee.Set_Referee_UI_Circle(5, 2, Referee_Data_Interaction_Graphic_Color_CYAN, 15, 1440 + 7 / 2 + 15 * 11, 375, 7);
    }
    else if (Robot_Chassis_Type == Robot_Chassis_Type_POWER)
    {
        // 功率优先
        graphic_referee_chassis_status_circle = Referee.Set_Referee_UI_Circle(5, 2, Referee_Data_Interaction_Graphic_Color_PINK, 15, 1440 + 7 / 2 + 15 * 11, 375, 7);
    }
    // 裁判系统备用发射机构标记圆
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_referee_booster_status_circle;
    if (Robot_Booster_Type == Robot_Booster_Type_CD)
    {
        // 冷却优先
        graphic_referee_booster_status_circle = Referee.Set_Referee_UI_Circle(5, 3, Referee_Data_Interaction_Graphic_Color_CYAN, 15, 1440 + 7 / 2 + 15 * 15, 375, 7);
    }
    else if (Robot_Booster_Type == Robot_Booster_Type_BURST)
    {
        // 爆发优先
        graphic_referee_booster_status_circle = Referee.Set_Referee_UI_Circle(5, 3, Referee_Data_Interaction_Graphic_Color_PINK, 15, 1440 + 7 / 2 + 15 * 15, 375, 7);
    }
    // 裁判系统备用等级整数
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_referee_level_status_integer = Referee.Set_Referee_UI_Integer(5, 4, Referee_Data_Interaction_Graphic_Color_GREEN, 3, 1440 + 15 * 18, 375, 15, Robot_Level);
    Referee.UART_Send_Interaction_UI_Graphic_5(graphic_referee_status_circle, graphic_referee_color_status_circle, graphic_referee_chassis_status_circle, graphic_referee_booster_status_circle, graphic_referee_level_status_integer);
    // 准备发送下一组
    HAL_Delay(34);

    // 超级电容放电状态矩形
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_supercap_working_status_rectangle;
    if (Supercap_Enable == false)
    {
        // 操作手踢出超级电容
        graphic_supercap_working_status_rectangle = Referee.Set_Referee_UI_Rectangle(6, 0, Referee_Data_Interaction_Graphic_Color_BLACK, 10, 760, 210, 760 + 1900 / 5, 180);
    }
    else if (Supercap.Get_Status() == false)
    {
        // 超级电容掉线
        graphic_supercap_working_status_rectangle = Referee.Set_Referee_UI_Rectangle(6, 0, Referee_Data_Interaction_Graphic_Color_PURPLE, 10, 760, 210, 760 + 1900 / 5, 180);
    }
    else if (Supercap_Accelerate_Status == true)
    {
        if(Supercap_Burst_Mode_Status == false)
        {
            // 加速模式
            graphic_supercap_working_status_rectangle = Referee.Set_Referee_UI_Rectangle(6, 0, Referee_Data_Interaction_Graphic_Color_YELLOW, 10, 760, 210, 760 + 1900 / 5, 180);
        }
        else if (Supercap_Burst_Mode_Status == true)
        {
            // 爆发模式
            graphic_supercap_working_status_rectangle = Referee.Set_Referee_UI_Rectangle(6, 0, Referee_Data_Interaction_Graphic_Color_ORANGE, 10, 760, 210, 760 + 1900 / 5, 180);
        }
    }
    else
    {
        // 常规工作模式
        graphic_supercap_working_status_rectangle = Referee.Set_Referee_UI_Rectangle(6, 0, Referee_Data_Interaction_Graphic_Color_WHITE, 10, 760, 210, 760 + 1900 / 5, 180);
    }
    // 超级电容能量矩形
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_supercap_energy_rectangle;
    if (Supercap.Get_Energy_Status() == Supercap_24_Energy_Status_NORMAL)
    {
        // 正常电量
        graphic_supercap_energy_rectangle = Referee.Set_Referee_UI_Rectangle(6, 1, Referee_Data_Interaction_Graphic_Color_GREEN, 20, 765, 195, 765 + Supercap.Get_Now_Energy() / 5, 195);
    }
    else if (Supercap.Get_Energy_Status() == Supercap_24_Energy_Status_WARNING)
    {
        // 警告电量
        graphic_supercap_energy_rectangle = Referee.Set_Referee_UI_Rectangle(6, 1, Referee_Data_Interaction_Graphic_Color_YELLOW, 20, 765, 195, 765 + Supercap.Get_Now_Energy() / 5, 195);
    }
    else
    {
        // 低电量
        graphic_supercap_energy_rectangle = Referee.Set_Referee_UI_Rectangle(6, 1, Referee_Data_Interaction_Graphic_Color_PURPLE, 20, 765, 195, 765 + Supercap.Get_Now_Energy() / 5, 195);
    }
    Referee.UART_Send_Interaction_UI_Graphic_2(graphic_supercap_working_status_rectangle, graphic_supercap_energy_rectangle);
    // 准备发送下一组
    HAL_Delay(34);

    // 超级电容电量整型
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_supercap_energy_integer;
    if (Supercap.Get_Energy_Status() == Supercap_24_Energy_Status_NORMAL)
    {
        // 正常电量
        graphic_supercap_energy_integer = Referee.Set_Referee_UI_Integer(6, 2, Referee_Data_Interaction_Graphic_Color_GREEN, 3, 765, 155, 30, Supercap.Get_Now_Energy());
    }
    else if (Supercap.Get_Energy_Status() == Supercap_24_Energy_Status_WARNING)
    {
        // 警告电量
        graphic_supercap_energy_integer = Referee.Set_Referee_UI_Integer(6, 2, Referee_Data_Interaction_Graphic_Color_YELLOW, 3, 765, 155, 30, Supercap.Get_Now_Energy());
    }
    else
    {
        // 低电量
        graphic_supercap_energy_integer = Referee.Set_Referee_UI_Integer(6, 2, Referee_Data_Interaction_Graphic_Color_PURPLE, 3, 765, 155, 30, Supercap.Get_Now_Energy());
    }
    // 云台灯条yaw角圆弧

    // 云台Pitch角浮点数
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_pitch_float;
    if(Gimbal.Motor_Yaw.Get_Status() == Motor_DJI_Status_DISABLE || Gimbal.Motor_Pitch.Get_Status() == Motor_DJI_Status_DISABLE)
    {
        // 云台掉线
        graphic_pitch_float = Referee.Set_Referee_UI_Float(4, 1, Referee_Data_Interaction_Graphic_Color_PURPLE, 5, 50, 780, 40, Gimbal.Get_Now_Pitch_Angle() / DEG_TO_RAD);
    }
    else
    {
        // 云台在线
        graphic_pitch_float = Referee.Set_Referee_UI_Float(4, 1, Referee_Data_Interaction_Graphic_Color_GREEN, 5, 50, 780, 40, Gimbal.Get_Now_Pitch_Angle() / DEG_TO_RAD);
    }
    // 自瞄方框
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_autoaiming_range_rectangle;
    if (Manifold.Get_Status() == Manifold_Status_DISABLE)
    {
        // 自瞄掉线
        graphic_autoaiming_range_rectangle = Referee.Set_Referee_UI_Rectangle(7, 0, Referee_Data_Interaction_Graphic_Color_PURPLE, 1, 700, 340, 1200, 750);
    }
    else if (Timer_Switch_Target.Get_Now_Status() == Timer_Status_RESET || Timer_Switch_Target.Get_Now_Status() == Timer_Status_WAIT)
    {
        if(Manifold_Autoaiming_Status == false)
        {
            // 抓到目标但未跟随
            graphic_autoaiming_range_rectangle = Referee.Set_Referee_UI_Rectangle(7, 0, Referee_Data_Interaction_Graphic_Color_YELLOW, 3, 700, 340, 1200, 750);
        }
        else
        {
            // 抓到且跟随目标
            graphic_autoaiming_range_rectangle = Referee.Set_Referee_UI_Rectangle(7, 0, Referee_Data_Interaction_Graphic_Color_GREEN, 3, 700, 340, 1200, 750);
        }
    }
    else
    {
        // 未抓到目标
        graphic_autoaiming_range_rectangle = Referee.Set_Referee_UI_Rectangle(7, 0, Referee_Data_Interaction_Graphic_Color_WHITE, 1, 700, 340, 1200, 750);
    }
    // 摩擦轮转速整型
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_friction_omega_integer;
    if(Booster.Motor_Friction_Right.Get_Status() == Motor_DJI_Status_DISABLE || Booster.Motor_Friction_Left.Get_Status() == Motor_DJI_Status_DISABLE || Booster.Motor_Driver.Get_Status() == Motor_DJI_Status_DISABLE)
    {
        // 发射机构掉线
        graphic_friction_omega_integer = Referee.Set_Referee_UI_Integer(8, 0, Referee_Data_Interaction_Graphic_Color_PURPLE, 5, 50, 850, 50, Booster.Get_Friction_Omega());
    }
    else if (FSM_Heat_Detector.Get_Now_Status_Serial() == FSM_Heat_Detector_Status_CLOSE)
    {
        // 发射机构在线但未启用
        graphic_friction_omega_integer = Referee.Set_Referee_UI_Integer(8, 0, Referee_Data_Interaction_Graphic_Color_WHITE, 5, 50, 850, 50, Booster.Get_Friction_Omega());
    }
    else
    {
        // 发射机构启用
        graphic_friction_omega_integer = Referee.Set_Referee_UI_Integer(8, 0, Referee_Data_Interaction_Graphic_Color_GREEN, 5, 50, 850, 50, Booster.Get_Friction_Omega());
    }
    Referee.UART_Send_Interaction_UI_Graphic_5(graphic_supercap_energy_integer, graphic_pitch_float, graphic_pitch_float, graphic_autoaiming_range_rectangle, graphic_friction_omega_integer);
    // 准备发送下一组
    HAL_Delay(34);

    // 系统警告信息
    Struct_Referee_Data_Interaction_Graphic_Config *graphic_competition_warning = Referee.Set_Referee_UI_String(9, 0, Referee_Data_Interaction_Graphic_Color_ORANGE, 4, 960 - 14 * 30 - 20, 800, 30, 30);
    // 发送当前字符串
    if (Referee.Get_Self_PM01_Booster_Status() == Referee_Data_Status_DISABLE)
    {
        // 发射机构未激活
        Referee.UART_Send_Interaction_UI_Graphic_String(graphic_competition_warning, "_____booster_deactivated_____");
    }
    else if (Referee.Get_17mm_Remaining() < 20)
    {
        // 缺子弹
        Referee.UART_Send_Interaction_UI_Graphic_String(graphic_competition_warning, "_________lack_bullet_________");
    }
    else if (Chassis_Gyroscope_Mode_Status != Robot_Gyroscope_Type_DISABLE && Chassis.Get_Now_Omega() < 0.5f)
    {
        // 小陀螺堵墙
        Referee.UART_Send_Interaction_UI_Graphic_String(graphic_competition_warning, "____gyro_fail,_leave wall____");
    }
    else
    {
        // 没有警告
        Referee.UART_Send_Interaction_UI_Graphic_String(graphic_competition_warning, " ");
    }
    // 准备发送下一组
    HAL_Delay(34);
}

/**
 * @brief 状态控制逻辑
 *
 */
void Class_Robot::_Status_Control()
{
    // 判断遥控器状态是否正常, 不在线或急停直接断控
    if (DR16.Get_Status() == DR16_Status_DISABLE || DR16.Get_Left_Switch() == DR16_Switch_Status_DOWN)
    {
        return;
    }
    else
    {
        // 中间竞赛模式

        // 整车状态控制
        if (DR16.Get_Keyboard_Key_Ctrl() == DR16_Key_Status_FREE)
        {
            // 没有按下Ctrl

            // 小陀螺与底盘跟随模式模式
            if (DR16.Get_Keyboard_Key_Q() == DR16_Key_Status_PRESSED)
            {
                Chassis_Gyroscope_Mode_Status = Robot_Gyroscope_Type_COUNTERCLOCKWISE;
                Chassis_Follow_Mode_Status = false;
            }
            else if (DR16.Get_Keyboard_Key_E() == DR16_Key_Status_PRESSED)
            {
                Chassis_Gyroscope_Mode_Status = Robot_Gyroscope_Type_CLOCKWISE;
                Chassis_Follow_Mode_Status = false;
            }
            else
            {
                Chassis_Gyroscope_Mode_Status = Robot_Gyroscope_Type_DISABLE;
            }
            if (DR16.Get_Keyboard_Key_Q() == DR16_Key_Status_TRIG_PRESSED_FREE || DR16.Get_Keyboard_Key_E() == DR16_Key_Status_TRIG_PRESSED_FREE)
            {
                Chassis_Follow_Mode_Status = true;
            }

            // 超级电容模式
            if (DR16.Get_Keyboard_Key_Shift() == DR16_Key_Status_PRESSED)
            {
                Supercap_Accelerate_Status = true;
                Supercap_Burst_Mode_Status = false;
            }
            else if (DR16.Get_Keyboard_Key_B() == DR16_Key_Status_PRESSED)
            {
                Supercap_Accelerate_Status = true;
                Supercap_Burst_Mode_Status = true;
            }
            else
            {
                Supercap_Accelerate_Status = false;
            }

            // 自瞄模式
            if (DR16.Get_Mouse_Right_Key() == DR16_Key_Status_PRESSED)
            {
                Manifold_Autoaiming_Status = true;
            }
            else
            {
                Manifold_Autoaiming_Status = false;
            }

            // 能量机关
            if(DR16.Get_Keyboard_Key_V() == DR16_Key_Status_TRIG_FREE_PRESSED)
            {
                if(Manifold_Aiming_Priority == Manifold_Aiming_Priority_RUNE)
                {
                    Manifold_Aiming_Priority = Manifold_Aiming_Priority_ARMOR;
                    FSM_DR16_Left_Mouse_Press_Hold.Set_Hold_Time_Threshold(FSM_DR16_Left_Mouse_Press_Hold.Get_Default_Hold_Time_Threshold());
                }
                else
                {
                    Manifold_Aiming_Priority = Manifold_Aiming_Priority_RUNE;
                    Timer_Rune_To_Armor.Set_Delay(30000);
                    FSM_DR16_Left_Mouse_Press_Hold.Set_Hold_Time_Threshold(1000);
                }
            }
            if(FSM_DR16_Left_Mouse_Press_Hold.Get_Now_Status_Serial() == FSM_Press_Hold_Status_HOLD)
            {
                Manifold_Aiming_Priority = Manifold_Aiming_Priority_ARMOR;
            }
        }
        else if (DR16.Get_Keyboard_Key_Ctrl() == DR16_Key_Status_PRESSED)
        {
            // 按下了Ctrl

            if (DR16.Get_Keyboard_Key_Shift() == DR16_Key_Status_FREE)
            {
                // 没有按下Shift

            }
            else if (DR16.Get_Keyboard_Key_Shift() == DR16_Key_Status_PRESSED)
            {
                // 按下了Shift

                // 整车初始化
                if (DR16.Get_Keyboard_Key_Z() == DR16_Key_Status_TRIG_PRESSED_FREE)
                {
                    HAL_NVIC_SystemReset();
                }

                // 是否启用超级电容
                if (DR16.Get_Keyboard_Key_C() == DR16_Key_Status_TRIG_FREE_PRESSED)
                {
                    Math_Boolean_Logical_Not(&Supercap_Enable);
                }

                // 设置自瞄己方颜色
                if (DR16.Get_Keyboard_Key_Q() == DR16_Key_Status_TRIG_FREE_PRESSED)
                {
                    Referee.Set_Referee_Trust_Status(Referee_Data_Status_DISABLE);
                    if(Manifold_Self_Color == Manifold_Enemy_Color_RED)
                    {
                        Manifold_Self_Color = Manifold_Enemy_Color_BLUE;
                    }
                    else if(Manifold_Self_Color == Manifold_Enemy_Color_BLUE)
                    {
                        Manifold_Self_Color = Manifold_Enemy_Color_RED;
                    }
                }

                // 踢出裁判系统
                if (DR16.Get_Keyboard_Key_X() == DR16_Key_Status_TRIG_FREE_PRESSED)
                {
                    if (Referee.Get_Referee_Trust_Status() == Referee_Data_Status_ENABLE)
                    {
                        Referee.Set_Referee_Trust_Status(Referee_Data_Status_DISABLE);
                    }
                    else if (Referee.Get_Referee_Trust_Status() == Referee_Data_Status_DISABLE)
                    {
                        Referee.Set_Referee_Trust_Status(Referee_Data_Status_ENABLE);
                    }
                }

                // 设置底盘属性
                if (DR16.Get_Keyboard_Key_A() == DR16_Key_Status_TRIG_FREE_PRESSED)
                {
                    Referee.Set_Referee_Trust_Status(Referee_Data_Status_DISABLE);
                    if(Robot_Chassis_Type == Robot_Chassis_Type_HP)
                    {
                        Robot_Chassis_Type = Robot_Chassis_Type_POWER;
                    }
                    else if(Robot_Chassis_Type == Robot_Chassis_Type_POWER)
                    {
                        Robot_Chassis_Type = Robot_Chassis_Type_HP;
                    }
                }

                // 设置发射机构属性
                if (DR16.Get_Keyboard_Key_S() == DR16_Key_Status_TRIG_FREE_PRESSED)
                {
                    Referee.Set_Referee_Trust_Status(Referee_Data_Status_DISABLE);
                    if(Robot_Booster_Type == Robot_Booster_Type_CD)
                    {
                        Robot_Booster_Type = Robot_Booster_Type_BURST;
                    }
                    else if(Robot_Booster_Type == Robot_Booster_Type_BURST)
                    {
                        Robot_Booster_Type = Robot_Booster_Type_CD;
                    }
                }

                // 设置等级
                if (DR16.Get_Keyboard_Key_E() == DR16_Key_Status_TRIG_FREE_PRESSED)
                {
                    Referee.Set_Referee_Trust_Status(Referee_Data_Status_DISABLE);
                    Robot_Level++;
                    Math_Constrain(&Robot_Level, (int32_t) 1, (int32_t) 10);
                }
                else if (DR16.Get_Keyboard_Key_D() == DR16_Key_Status_TRIG_FREE_PRESSED)
                {
                    Referee.Set_Referee_Trust_Status(Referee_Data_Status_DISABLE);
                    Robot_Level--;
                    Math_Constrain(&Robot_Level, (int32_t) 1, (int32_t) 10);
                }

                // 刷新UI
                if (DR16.Get_Keyboard_Key_R() == DR16_Key_Status_TRIG_FREE_PRESSED)
                {
                    UI_Init_Status = false;
                }
            }
        }
    }

    if (DR16.Get_Left_Switch() == DR16_Switch_Status_UP)
    {
        Manifold_Autoaiming_Status = true;
    }

    if(Referee.Get_Status() == Referee_Status_ENABLE && Referee.Get_Referee_Trust_Status() == Referee_Data_Status_ENABLE)
    {
        if(Referee.Get_Self_ID() == Referee_Data_Robots_ID_RED_INFANTRY_3 || Referee.Get_Self_ID() == Referee_Data_Robots_ID_RED_INFANTRY_4 || Referee.Get_Self_ID() == Referee_Data_Robots_ID_RED_INFANTRY_5)
        {
            Manifold_Self_Color = Manifold_Enemy_Color_RED;
        }
        else if(Referee.Get_Self_ID() == Referee_Data_Robots_ID_BLUE_INFANTRY_3 || Referee.Get_Self_ID() == Referee_Data_Robots_ID_BLUE_INFANTRY_4 || Referee.Get_Self_ID() == Referee_Data_Robots_ID_BLUE_INFANTRY_5)
        {
            Manifold_Self_Color = Manifold_Enemy_Color_BLUE;
        }

        Robot_Level = Referee.Get_Self_Level();
    }
    if(Timer_Rune_To_Armor.Get_Now_Status() == Timer_Status_TIMEOUT)
    {
        Manifold_Aiming_Priority = Manifold_Aiming_Priority_ARMOR;
    }
}

/**
 * @brief 视觉Manifold控制逻辑
 *
 */
void Class_Robot::_Manifold_Control()
{
    if(Manifold.Get_Enemy_ID() != Manifold_Enemy_ID_NONE_0)
    {
        Timer_Switch_Target.Set_Delay(100);
    }

    Manifold.Set_Aiming_Priority(Manifold_Aiming_Priority);
    Manifold.Set_Velocity_X(Posture.Get_Gimbal_Velocity_X());
    Manifold.Set_Velocity_Y(Posture.Get_Gimbal_Velocity_Y());
    if(Referee.Get_Status() == Referee_Status_DISABLE || Referee.Get_Referee_Trust_Status() == Referee_Data_Status_DISABLE)
    {
        Manifold.Set_Enemy_Color(static_cast<Enum_Manifold_Enemy_Color>(!Manifold_Self_Color));
    }
    else
    {
        if(Referee.Get_Self_ID() == Referee_Data_Robots_ID_RED_INFANTRY_3 || Referee.Get_Self_ID() == Referee_Data_Robots_ID_RED_INFANTRY_4 || Referee.Get_Self_ID() == Referee_Data_Robots_ID_RED_INFANTRY_5)
        {
            Manifold.Set_Enemy_Color(Manifold_Enemy_Color_BLUE);
        }
        else if(Referee.Get_Self_ID() == Referee_Data_Robots_ID_BLUE_INFANTRY_3 || Referee.Get_Self_ID() == Referee_Data_Robots_ID_BLUE_INFANTRY_4 || Referee.Get_Self_ID() == Referee_Data_Robots_ID_BLUE_INFANTRY_5)
        {
            Manifold.Set_Enemy_Color(Manifold_Enemy_Color_RED);
        }
    }

    Manifold.Set_Now_Gimbal_Yaw(Gimbal.Get_Now_Yaw_Angle());
    Manifold.Set_Now_Gimbal_Pitch(Gimbal.Get_Now_Pitch_Angle());
}

/**
 * @brief 超级电容控制逻辑
 *
 */
void Class_Robot::_Supercap_Control()
{
    // 根据裁判系统是否在线与可信决定功率
    if (Referee.Get_Status() == Referee_Status_DISABLE || Referee.Get_Referee_Trust_Status() == Referee_Data_Status_DISABLE)
    {
        if (Robot_Chassis_Type == Robot_Chassis_Type_POWER)
        {
            Supercap.Set_Power_Limit_Max(chassis_power_first_power[Robot_Level - 1] * 0.8f);
        }
        else if (Robot_Chassis_Type == Robot_Chassis_Type_HP)
        {
            Supercap.Set_Power_Limit_Max(chassis_hp_first_power[Robot_Level - 1] * 0.8f);
        }
    }
    else
    {
        Supercap.Set_Power_Limit_Max(Referee.Get_Self_Chassis_Power_Max() * 0.8f);
    }

    Supercap.Set_Buffer_Energy_Loop_Status(Supercap_24_Data_Status_ENABLE);
    Supercap.Set_Chassis_Buffer_Energy(Referee.Get_Chassis_Energy_Buffer());
    Supercap.Set_Power_Compensate_Max(300.0f);

    // 超级电容开启状态控制

    Enum_Supercap_24_Data_Status tmp_status = Supercap_24_Data_Status_ENABLE;

    if (DR16.Get_Status() == DR16_Status_DISABLE || DR16.Get_Left_Switch() == DR16_Switch_Status_DOWN)
    {
        // 遥控器断掉则失能超级电容
        Supercap_Auto_Restart_Flag = false;
        tmp_status = Supercap_24_Data_Status_DISABLE;
    }
    else if (Supercap_Enable == false)
    {
        // 操作手手动关闭则失能超级电容
        Supercap_Auto_Restart_Flag = false;
        tmp_status = Supercap_24_Data_Status_DISABLE;
    }
    else if (Gimbal.Motor_Yaw.Get_Status() == Motor_DJI_Status_DISABLE && Gimbal.Motor_Pitch.Get_Status() == Motor_DJI_Status_DISABLE)
    {
        // 云台掉电则失能超级电容
        Supercap_Auto_Restart_Flag = false;
        tmp_status = Supercap_24_Data_Status_DISABLE;
    }
    else if(Referee.Get_Status() == Referee_Status_ENABLE && Referee.Get_Referee_Trust_Status() == Referee_Data_Status_ENABLE && Referee.Get_Self_PM01_Chassis_Status() == Referee_Data_Status_DISABLE)
    {
        // 底盘裁判系统掉电则失能超级电容
        Supercap_Auto_Restart_Flag = false;
        tmp_status = Supercap_24_Data_Status_DISABLE;
    }
    else
    {
        // 缓冲能量耗尽后超级电容的控制
        if (Referee.Get_Chassis_Energy_Buffer() <= Restrict_Enable_Buffer_Energy)
        {
            // 如果仅仅因为缓冲能量即将耗尽, 则失能超级电容, 同时准备下次开启
            Supercap_Auto_Restart_Flag = true;
            tmp_status = Supercap_24_Data_Status_DISABLE;
        }
        else if (Referee.Get_Chassis_Energy_Buffer() >= Restrict_Disable_Buffer_Energy && Supercap_Auto_Restart_Flag == true)
        {
            // 缓冲能量恢复满, 且是仅仅因为缓冲能量即将耗尽而失能, 则开启超级电容
            Supercap_Auto_Restart_Flag = false;
            tmp_status = Supercap_24_Data_Status_ENABLE;
        }
        else if (Supercap_Auto_Restart_Flag == true)
        {
            // 自动重启期间, 超级电容关闭
            tmp_status = Supercap_24_Data_Status_DISABLE;
        }
    }
    Supercap.Set_Supercap_Enable_Status(tmp_status);
}

/**
 * @brief 底盘控制逻辑
 *
 */
void Class_Robot::_Chassis_Control()
{
    // 根据裁判系统是否在线与可信决定功率
    float tmp_chassis_power_limit_max = 0.0f;
    if (Referee.Get_Status() == Referee_Status_DISABLE || Referee.Get_Referee_Trust_Status() == Referee_Data_Status_DISABLE)
    {
        // 裁判系统掉线或远端信息不可信
        if (Robot_Chassis_Type == Robot_Chassis_Type_POWER)
        {
            tmp_chassis_power_limit_max = chassis_power_first_power[Robot_Level - 1];
        }
        else if (Robot_Chassis_Type == Robot_Chassis_Type_HP)
        {
            tmp_chassis_power_limit_max = chassis_hp_first_power[Robot_Level - 1];
        }
    }
    else
    {
        // 裁判系统可信
        tmp_chassis_power_limit_max = Referee.Get_Self_Chassis_Power_Max();
    }

    // 根据超级电容决定功率
    if (Supercap.Get_Status() == Supercap_24_Status_ENABLE && Supercap_Enable == true)
    {
        // 超级电容在线且启用
        // 有电
        if(Supercap.Get_Energy_Status() == Supercap_24_Energy_Status_NORMAL)
        {
            if (Supercap_Accelerate_Status == true)
            {
                // 主动加速
                if (Supercap_Burst_Mode_Status == true)
                {
                    // 爆发加速
                    tmp_chassis_power_limit_max += 250.0f;
                }
                else
                {
                    // 一般加速
                    tmp_chassis_power_limit_max += 75.0f;
                }
            }
        }
    }
    if (Supercap.Get_Status() == Supercap_24_Status_ENABLE)
    {
        tmp_chassis_power_limit_max += PID_Referee_Chassis_Power.Get_Out();
    }
    else
    {
        tmp_chassis_power_limit_max += PID_Referee_Chassis_Power.Get_Out();
    }
    Chassis.Set_Power_Limit_Max(tmp_chassis_power_limit_max);

    // 速度上限与加减速规划控制
    float tmp_chassis_velocity_max, tmp_chassis_omega_max;
    if (Supercap_Accelerate_Status == true)
    {
        tmp_chassis_velocity_max = 4.0f;
        tmp_chassis_omega_max = 6.0f * PI;
        Slope_Speed_X.Set_Increase_Value(5.0f / 1000.0f);
    }
    else
    {
        tmp_chassis_velocity_max = 3.0f;
        tmp_chassis_omega_max = 4.0f * PI;
        Slope_Speed_X.Set_Increase_Value(3.0f / 1000.0f);
    }

    // 判断遥控器状态是否正常, 不在线或急停直接断控
    if (DR16.Get_Status() == DR16_Status_DISABLE || DR16.Get_Left_Switch() == DR16_Switch_Status_DOWN)
    {
        Chassis.Set_Chassis_Control_Type(Chassis_Control_Type_DISABLE);
        return;
    }
    else
    {
        // 竞赛模式
        Chassis.Set_Chassis_Control_Type(Chassis_Control_Type_NORMAL);

        // 底盘速度期望值
        float tmp_expect_direction_velocity_x, tmp_expect_direction_velocity_y, tmp_expect_direction_omega;
        // 底盘速度经过斜坡函数的值
        float tmp_planning_chassis_velocity_x, tmp_planning_chassis_velocity_y, tmp_planning_chassis_omega;
        // 底盘速度传参值
        float tmp_chassis_velocity_x, tmp_chassis_velocity_y, tmp_chassis_omega;

        // 遥控器摇杆值
        float dr16_left_x = DR16.Get_Left_X();
        float dr16_left_y = DR16.Get_Left_Y();
        float dr16_yaw = DR16.Get_Yaw();
        // 排除死区
        dr16_left_x = Math_Abs(dr16_left_x) > DR16_Rocker_Dead_Zone ? dr16_left_x : 0.0f;
        dr16_left_y = Math_Abs(dr16_left_y) > DR16_Rocker_Dead_Zone ? dr16_left_y : 0.0f;
        dr16_yaw = Math_Abs(dr16_yaw) > DR16_Rocker_Dead_Zone ? dr16_yaw : 0.0f;

        // 摇杆控制

        tmp_expect_direction_velocity_x = dr16_left_y * tmp_chassis_velocity_max;
        tmp_expect_direction_velocity_y = -dr16_left_x * tmp_chassis_velocity_max;
        tmp_expect_direction_omega = dr16_yaw * tmp_chassis_omega_max;

        // 键鼠控制

        if (DR16.Get_Keyboard_Key_Ctrl() == DR16_Key_Status_FREE)
        {
            // 没有按下Ctrl

            if (DR16.Get_Keyboard_Key_W() == DR16_Key_Status_PRESSED)
            {
                tmp_expect_direction_velocity_x += tmp_chassis_velocity_max;
            }
            if (DR16.Get_Keyboard_Key_S() == DR16_Key_Status_PRESSED)
            {
                tmp_expect_direction_velocity_x -= tmp_chassis_velocity_max;
            }
            if (DR16.Get_Keyboard_Key_A() == DR16_Key_Status_PRESSED)
            {
                tmp_expect_direction_velocity_y += tmp_chassis_velocity_max;
            }
            if (DR16.Get_Keyboard_Key_D() == DR16_Key_Status_PRESSED)
            {
                tmp_expect_direction_velocity_y -= tmp_chassis_velocity_max;
            }
        }

        // 小陀螺状态控制

        if (Chassis_Gyroscope_Mode_Status == Robot_Gyroscope_Type_CLOCKWISE)
        {
            tmp_expect_direction_omega -= tmp_chassis_omega_max;
        }
        else if (Chassis_Gyroscope_Mode_Status == Robot_Gyroscope_Type_COUNTERCLOCKWISE)
        {
            tmp_expect_direction_omega += tmp_chassis_omega_max;
        }

        // 非小陀螺则底盘跟随
        if (Chassis_Gyroscope_Mode_Status == false && dr16_yaw == 0.0f && Chassis_Follow_Mode_Status == true)
        {
            // 非底盘特殊形态, 则底盘跟随
            PID_Chassis_Follow.Set_Target(0.0f);
            PID_Chassis_Follow.Set_Now(0.0f - Math_Modulus_Normalization(Gimbal.Get_Now_Yaw_Angle(), 2.0f * PI));
            PID_Chassis_Follow.TIM_Calculate_PeriodElapsedCallback();
            tmp_expect_direction_omega += PID_Chassis_Follow.Get_Out();
        }

        // 速度斜坡函数

        // 底盘相对于云台角度
        float cos_yaw, sin_yaw;
        cos_yaw = arm_cos_f32(-Gimbal.Get_Now_Yaw_Angle());
        sin_yaw = arm_sin_f32(-Gimbal.Get_Now_Yaw_Angle());
        // 底盘相对于云台运动方向
        float chassis_now_vx, chassis_now_vy;
        chassis_now_vx = Chassis.Get_Now_Velocity_X() * cos_yaw + Chassis.Get_Now_Velocity_Y() * (-sin_yaw);
        chassis_now_vy = Chassis.Get_Now_Velocity_X() * sin_yaw + Chassis.Get_Now_Velocity_Y() * cos_yaw;
        // 规划速度
        Slope_Speed_X.Set_Now_Real(chassis_now_vx);
        Slope_Speed_Y.Set_Now_Real(chassis_now_vy);
        Slope_Speed_X.Set_Target(tmp_expect_direction_velocity_x);
        Slope_Speed_Y.Set_Target(tmp_expect_direction_velocity_y);
        Slope_Speed_X.TIM_Calculate_PeriodElapsedCallback();
        Slope_Speed_Y.TIM_Calculate_PeriodElapsedCallback();
        // 保存规划后结果
        tmp_planning_chassis_velocity_x = Slope_Speed_X.Get_Out();
        tmp_planning_chassis_velocity_y = Slope_Speed_Y.Get_Out();

        // 小陀螺模式前馈适配平动速度

        // 云台相对于底盘角度加前馈三角函数
        float cos_yaw_feedforward, sin_yaw_feedforward;
        if (Posture.AHRS_Chassis.Get_Status() == AHRS_WHEELTEC_Status_ENABLE)
        {
            cos_yaw_feedforward = arm_cos_f32(Gimbal.Get_Now_Yaw_Angle() - Posture.Get_Chassis_Omega() * AHRS_Chassis_Omega_Feedforward);
            sin_yaw_feedforward = arm_sin_f32(Gimbal.Get_Now_Yaw_Angle() - Posture.Get_Chassis_Omega() * AHRS_Chassis_Omega_Feedforward);
        }
        else
        {
            cos_yaw_feedforward = arm_cos_f32(Gimbal.Get_Now_Yaw_Angle() - Posture.Get_Chassis_Omega() * Chassis_Omega_Feedforward);
            sin_yaw_feedforward = arm_sin_f32(Gimbal.Get_Now_Yaw_Angle() - Posture.Get_Chassis_Omega() * Chassis_Omega_Feedforward);
        }
        // 确保第一人称直线, 乘上旋转矩阵
        tmp_chassis_velocity_x = tmp_planning_chassis_velocity_x * cos_yaw_feedforward + tmp_planning_chassis_velocity_y * (-sin_yaw_feedforward);
        tmp_chassis_velocity_y = tmp_planning_chassis_velocity_x * sin_yaw_feedforward + tmp_planning_chassis_velocity_y * cos_yaw_feedforward;

        // 角速度斜坡函数
        Slope_Omega.Set_Target(tmp_expect_direction_omega);
        Slope_Omega.Set_Now_Real(Posture.Get_Chassis_Omega());
        Slope_Omega.TIM_Calculate_PeriodElapsedCallback();
        tmp_planning_chassis_omega = Slope_Omega.Get_Out();

        tmp_chassis_omega = tmp_planning_chassis_omega;

        Chassis.Set_Target_Velocity_X(tmp_chassis_velocity_x);
        Chassis.Set_Target_Velocity_Y(tmp_chassis_velocity_y);
        Chassis.Set_Target_Omega(tmp_chassis_omega);
    }
}

/**
 * @brief 云台控制逻辑
 *
 */
void Class_Robot::_Gimbal_Control()
{
    // 判断遥控器状态是否正常, 不在线或急停直接断控
    if (DR16.Get_Status() == DR16_Status_DISABLE || DR16.Get_Left_Switch() == DR16_Switch_Status_DOWN)
    {
        Gimbal.Set_Gimbal_Control_Type(Gimbal_Control_Type_DISABLE);
        return;
    }
    else
    {
        // 竞赛模式
        Gimbal.Set_Gimbal_Control_Type(Gimbal_Control_Type_ANGLE);

        // 云台角度传参值
        float tmp_gimbal_yaw = Gimbal.Get_Target_Yaw_Angle();
        float tmp_gimbal_pitch = Gimbal.Get_Target_Pitch_Angle();
        // 云台前馈值
        float tmp_gimbal_yaw_feedforward_omega = 0.0f;
        float tmp_gimbal_pitch_feedforward_omega = 0.0f;
        float tmp_gimbal_pitch_feedforward_current = 0.0f;

        // 遥控器摇杆值
        float dr16_right_x = DR16.Get_Right_X();
        float dr16_right_y = DR16.Get_Right_Y();
        // 排除死区
        dr16_right_x = Math_Abs(dr16_right_x) > DR16_Rocker_Dead_Zone ? dr16_right_x : 0.0f;
        dr16_right_y = Math_Abs(dr16_right_y) > DR16_Rocker_Dead_Zone ? dr16_right_y : 0.0f;

        // 摇杆控制

        tmp_gimbal_yaw -= dr16_right_x * DR16_Rocker_Yaw_Resolution;
        tmp_gimbal_yaw_feedforward_omega -= dr16_right_x * DR16_Rocker_Yaw_Resolution * 1000.0f;
        tmp_gimbal_pitch -= dr16_right_y * DR16_Rocker_Pitch_Resolution;
        tmp_gimbal_pitch_feedforward_omega += dr16_right_y * DR16_Rocker_Pitch_Resolution * 1000.0f;

        // 键鼠控制

        if (DR16.Get_Keyboard_Key_Ctrl() == DR16_Key_Status_FREE)
        {
            // 没有按下Ctrl

            tmp_gimbal_yaw -= DR16.Get_Mouse_X() * DR16_Mouse_Yaw_Angle_Resolution;
            tmp_gimbal_yaw_feedforward_omega -= DR16.Get_Mouse_X() * DR16_Mouse_Yaw_Angle_Resolution * 1000.0f;
            tmp_gimbal_pitch += DR16.Get_Mouse_Y() * DR16_Mouse_Pitch_Angle_Resolution;
            tmp_gimbal_pitch_feedforward_omega -= DR16.Get_Mouse_Y() * DR16_Mouse_Pitch_Angle_Resolution * 1000.0f;

            if (DR16.Get_Keyboard_Key_G() == DR16_Key_Status_TRIG_FREE_PRESSED)
            {
                Timer_Turn_About.Set_Delay(1000);
                tmp_gimbal_yaw += PI;
            }
            if (DR16.Get_Keyboard_Key_Z() == DR16_Key_Status_TRIG_FREE_PRESSED)
            {
                Timer_Turn_About.Set_Delay(500);
                tmp_gimbal_yaw += PI / 4.0f;
            }
            if (DR16.Get_Keyboard_Key_C() == DR16_Key_Status_TRIG_FREE_PRESSED)
            {
                Timer_Turn_About.Set_Delay(500);
                tmp_gimbal_yaw -= PI / 4.0f;
            }
        }

        // 视觉控制

        if (Manifold_Autoaiming_Status == true && Manifold.Get_Status() == Manifold_Status_ENABLE)
        {
            if(Timer_Switch_Target.Get_Now_Status() == Timer_Status_RESET || Timer_Switch_Target.Get_Now_Status() == Timer_Status_WAIT)
            {
                tmp_gimbal_yaw = Manifold.Get_Target_Gimbal_Yaw() - Posture.Get_Chassis_Omega() * Autoaiming_Chassis_Omega_Feedforward;
                tmp_gimbal_yaw_feedforward_omega = Manifold.Get_Gimbal_Yaw_Omega_FeedForward();
                tmp_gimbal_pitch = Manifold.Get_Target_Gimbal_Pitch();
                tmp_gimbal_pitch_feedforward_omega = 0.0f;
            }
        }

        // 底盘旋转适配

        tmp_gimbal_yaw -= Posture.Get_Chassis_Omega() * 0.001f;
        tmp_gimbal_yaw_feedforward_omega -= Posture.Get_Chassis_Omega();

        // 重力补偿拟合
        // tmp_gimbal_pitch_feedforward_current += -0.4643f * Posture.Get_Gimbal_Odom_Angle_Pitch() * Posture.Get_Gimbal_Odom_Angle_Pitch() * Posture.Get_Gimbal_Odom_Angle_Pitch() - 0.254f * Posture.Get_Gimbal_Odom_Angle_Pitch() * Posture.Get_Gimbal_Odom_Angle_Pitch() + 0.8796f * Posture.Get_Gimbal_Odom_Angle_Pitch() + 0.3594f;

        if ((Manifold_Autoaiming_Status == true && Manifold.Get_Status() == Manifold_Status_ENABLE && Manifold.Get_Enemy_ID() != Manifold_Enemy_ID_NONE_0) || Timer_Turn_About.Get_Now_Status() == Timer_Status_WAIT)
        {
            Gimbal.Set_Target_Yaw_Angle(tmp_gimbal_yaw);
        }
        else
        {
            Gimbal.Set_Target_Yaw_Angle(Gimbal.Get_Now_Yaw_Angle() - Posture.Get_Chassis_Omega() * 0.001f);
        }
        Gimbal.Set_Target_Pitch_Angle(tmp_gimbal_pitch);
        Gimbal.Motor_Yaw.Set_Feedforward_Omega(tmp_gimbal_yaw_feedforward_omega);
        Gimbal.Motor_Pitch.Set_Feedforward_Omega(tmp_gimbal_pitch_feedforward_omega);
        Gimbal.Motor_Pitch.Set_Feedforward_Current(tmp_gimbal_pitch_feedforward_current);
    }
}

/**
 * @brief 发射机构控制逻辑
 *
 */
void Class_Robot::_Booster_Control()
{
    // 热量自检测状态机
    FSM_Heat_Detector.TIM_1ms_Calculate_PeriodElapsedCallback();

    // 根据裁判系统是否在线与可信决定热量
    if (Referee.Get_Status() == Referee_Status_DISABLE || Referee.Get_Referee_Trust_Status() == Referee_Data_Status_DISABLE)
    {
        // 裁判系统掉线或远端信息不可信

        if (Robot_Booster_Type == Robot_Booster_Type_BURST)
        {
            Booster.Set_Heat_Limit_Max(booster_burst_first_heat_max[Robot_Level - 1]);
            Booster.Set_Heat_CD(booster_burst_first_heat_cd[Robot_Level - 1]);
        }
        else if (Robot_Booster_Type == Robot_Booster_Type_CD)
        {
            Booster.Set_Heat_Limit_Max(booster_cd_first_heat_max[Robot_Level - 1]);
            Booster.Set_Heat_CD(booster_cd_first_heat_cd[Robot_Level - 1]);
        }
        Booster.Set_Now_Heat(FSM_Heat_Detector.Get_Now_Heat());
    }
    else
    {
        // 裁判系统可信
        Booster.Set_Heat_Limit_Max(Referee.Get_Self_Booster_Heat_Max());
        Booster.Set_Heat_CD(Referee.Get_Self_Booster_Heat_CD());
        Booster.Set_Now_Heat(Referee.Get_Booster_17mm_1_Heat());
    }

    // 判断遥控器状态是否正常, 不在线或急停直接断控
    if (DR16.Get_Status() == DR16_Status_DISABLE || DR16.Get_Left_Switch() == DR16_Switch_Status_DOWN)
    {
        Booster.Set_Booster_Control_Type(Booster_Control_Type_DISABLE);
        return;
    }
    else
    {
        // 竞赛模式

        // 摇杆控制与键鼠控制

        if (DR16.Get_Keyboard_Key_Ctrl() == DR16_Key_Status_FREE)
        {
            // 没有按下Ctrl

            // 发射逻辑控制
            if(Manifold_Autoaiming_Status == false || Manifold.Get_Status() == Manifold_Status_DISABLE || (Manifold_Autoaiming_Status == true && (Timer_Switch_Target.Get_Now_Status() == Timer_Status_RESET || Timer_Switch_Target.Get_Now_Status() == Timer_Status_WAIT)))
            {
                if (DR16.Get_Right_Switch() == DR16_Switch_Status_TRIG_MIDDLE_UP || DR16.Get_Mouse_Left_Key() == DR16_Key_Status_TRIG_FREE_PRESSED)
                {
                    // 单发逻辑
                    Booster.Set_Booster_Control_Type(Booster_Control_Type_SPOT);
                }
                else if (DR16.Get_Right_Switch() == DR16_Switch_Status_TRIG_MIDDLE_DOWN || FSM_DR16_Left_Mouse_Press_Hold.Get_Now_Status_Serial() == FSM_Press_Hold_Status_HOLD)
                {
                    // 连发逻辑
                    Booster.Set_Booster_Control_Type(Booster_Control_Type_AUTO);

                    if (Referee.Get_Status() == Referee_Status_ENABLE && Referee.Get_Referee_Trust_Status() == Referee_Data_Status_ENABLE)
                    {
                        float tmp_frequency = Referee.Get_Self_Booster_Heat_Max() / 10.0f;
                        Math_Constrain(&tmp_frequency, 5.0f, 18.0f);
                        Booster.Set_Max_Ammo_Shoot_Frequency(tmp_frequency);
                    }
                    else
                    {
                        Booster.Set_Max_Ammo_Shoot_Frequency(15.0f);
                    }
                }
                else if (DR16.Get_Right_Switch() == DR16_Switch_Status_TRIG_DOWN_MIDDLE || FSM_DR16_Left_Mouse_Press_Hold.Get_Now_Status_Serial() == FSM_Press_Hold_Status_FREE)
                {
                    // 连发暂停
                    Booster.Set_Booster_Control_Type(Booster_Control_Type_CEASEFIRE);
                }
            }

            // 关闭摩擦轮
            if (DR16.Get_Keyboard_Key_R() == DR16_Key_Status_TRIG_FREE_PRESSED)
            {
                FSM_DR16_Left_Mouse_Press_Hold.Set_Status(FSM_Press_Hold_Status_STOP);
                Booster.Set_Booster_Control_Type(Booster_Control_Type_DISABLE);
            }
        }
        else if (DR16.Get_Keyboard_Key_Ctrl() == DR16_Key_Status_PRESSED)
        {
            // 按下了Ctrl

            float tmp_friction_omega = Booster.Get_Friction_Omega();

            if(DR16.Get_Keyboard_Key_Z() == DR16_Key_Status_PRESSED)
            {
                tmp_friction_omega += DR16.Get_Mouse_Z() * DR16_Mouse_Friction_Omega_Resolution;
            }

            Booster.Set_Friction_Omega(tmp_friction_omega);
        }
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
