/**
 * @file dvc_motor_dji.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 大疆电机配置与操作
 * @version 1.2
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-01-01 1.1 官方6020驱动更新, 适配电压控制与电流控制
 * @date 2024-03-07 1.2 新增功率控制接口与相关函数, 24赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_motor_dji.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 分配CAN发送缓冲区
 *
 * @param hcan CAN编号
 * @param __CAN_ID CAN ID
 * @param __DJI_Motor_Driver_Version 大疆驱动版本, 当且仅当当前被分配电机为6020, 且是电流驱动新版本时选2023, 否则都是default
 * @return uint8_t* 缓冲区指针
 */
uint8_t *allocate_tx_data(CAN_HandleTypeDef *hcan, Enum_Motor_DJI_ID __CAN_ID, Enum_Motor_DJI_GM6020_Driver_Version __DJI_Motor_Driver_Version = Motor_DJI_GM6020_Driver_Version_DEFAULT)
{
    uint8_t *tmp_tx_data_ptr;
    if (hcan == &hcan1)
    {
        switch (__CAN_ID)
        {
        case (Motor_DJI_ID_0x201):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[0]);

            break;
        }
        case (Motor_DJI_ID_0x202):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[2]);

            break;
        }
        case (Motor_DJI_ID_0x203):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[4]);

            break;
        }
        case (Motor_DJI_ID_0x204):
        {
            tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[6]);

            break;
        }
        case (Motor_DJI_ID_0x205):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[0]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN1_0x1fe_Tx_Data[0]);
            }

            break;
        }
        case (Motor_DJI_ID_0x206):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[2]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN1_0x1fe_Tx_Data[2]);
            }

            break;
        }
        case (Motor_DJI_ID_0x207):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[4]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN1_0x1fe_Tx_Data[4]);
            }

            break;
        }
        case (Motor_DJI_ID_0x208):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[6]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN1_0x1fe_Tx_Data[6]);
            }

            break;
        }
        case (Motor_DJI_ID_0x209):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[0]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN1_0x2fe_Tx_Data[0]);
            }

            break;
        }
        case (Motor_DJI_ID_0x20A):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[2]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN1_0x2fe_Tx_Data[2]);
            }

            break;
        }
        case (Motor_DJI_ID_0x20B):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[4]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN1_0x2fe_Tx_Data[4]);
            }

            break;
        }
        }
    }
    else if (hcan == &hcan2)
    {
        switch (__CAN_ID)
        {
        case (Motor_DJI_ID_0x201):
        {
            tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[0]);

            break;
        }
        case (Motor_DJI_ID_0x202):
        {
            tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[2]);

            break;
        }
        case (Motor_DJI_ID_0x203):
        {
            tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[4]);

            break;
        }
        case (Motor_DJI_ID_0x204):
        {
            tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[6]);

            break;
        }
        case (Motor_DJI_ID_0x205):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[0]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN2_0x1fe_Tx_Data[0]);
            }

            break;
        }
        case (Motor_DJI_ID_0x206):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[2]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN2_0x1fe_Tx_Data[2]);
            }

            break;
        }
        case (Motor_DJI_ID_0x207):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[4]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN2_0x1fe_Tx_Data[4]);
            }

            break;
        }
        case (Motor_DJI_ID_0x208):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[6]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN2_0x1fe_Tx_Data[6]);
            }

            break;
        }
        case (Motor_DJI_ID_0x209):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[0]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN2_0x2fe_Tx_Data[0]);
            }

            break;
        }
        case (Motor_DJI_ID_0x20A):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[2]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN2_0x2fe_Tx_Data[2]);
            }

            break;
        }
        case (Motor_DJI_ID_0x20B):
        {
            if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
            {
                tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[4]);
            }
            else if (__DJI_Motor_Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
            {
                tmp_tx_data_ptr = &(CAN2_0x2fe_Tx_Data[4]);
            }

            break;
        }
        }
    }
    return (tmp_tx_data_ptr);
}

/**
 * @brief 估计功率值
 *
 * @param K_0 电机建模系数
 * @param K_1 电机建模系数
 * @param K_2 电机建模系数
 * @param A 电机建模系数
 * @param Current 电流
 * @param Omega 角速度
 * @return
 */
float power_calculate(float K_0, float K_1, float K_2, float A, float Current, float Omega)
{
    return (K_0 * Current * Omega + K_1 * Omega * Omega + K_2 * Current * Current + A);
}

/**
 * @brief 电机初始化
 *
 * @param hcan 绑定的CAN总线
 * @param __CAN_Rx_ID 绑定的CAN ID
 * @param __Motor_DJI_Control_Method 电机控制方式, 默认角度
 * @param __Encoder_Offset 编码器偏移, 默认0
 * @param __Driver_Version 6020电机驱动方式, 默认旧版电压控制
 * @param __Power_Limit_Status 是否开启功率控制
 * @param __Voltage_Max 最大速度, 需根据不同负载测量后赋值, 也就开环输出用得到, 不过我感觉应该没有奇葩喜欢开环输出这玩意
 * @param __Current_Max 最大电流
 */
void Class_Motor_DJI_GM6020::Init(CAN_HandleTypeDef *hcan, Enum_Motor_DJI_ID __CAN_Rx_ID, Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method, int32_t __Encoder_Offset, Enum_Motor_DJI_GM6020_Driver_Version __Driver_Version, Enum_Motor_DJI_Power_Limit_Status __Power_Limit_Status, float __Voltage_Max, float __Current_Max)
{
    if (hcan->Instance == CAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == CAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    CAN_Rx_ID = __CAN_Rx_ID;
    Motor_DJI_Control_Method = __Motor_DJI_Control_Method;
    Encoder_Offset = __Encoder_Offset;
    Driver_Version = __Driver_Version;
    Power_Limit_Status = __Power_Limit_Status;
    Voltage_Max = __Voltage_Max;
    Current_Max = __Current_Max;
    Tx_Data = allocate_tx_data(hcan, __CAN_Rx_ID, __Driver_Version);
}

/**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Motor_DJI_GM6020::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    // 滑动窗口, 判断电机是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Motor_DJI_GM6020::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Motor_DJI_Status = Motor_DJI_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
        PID_Current.Set_Integral_Error(0.0f);
    }
    else
    {
        // 电机保持连接
        Motor_DJI_Status = Motor_DJI_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Motor_DJI_GM6020::TIM_Calculate_PeriodElapsedCallback()
{
    PID_Calculate();

    if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
    {
        float tmp_value = Target_Voltage + Feedforward_Voltage;
        Math_Constrain(&tmp_value, -Voltage_Max, Voltage_Max);
        Out = tmp_value * Voltage_To_Out;
    }
    else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
    {
        float tmp_value = Target_Current + Feedforward_Current;
        Math_Constrain(&tmp_value, -Current_Max, Current_Max);
        Out = tmp_value * Current_To_Out;
    }

    Output();

    if (Power_Limit_Status == Motor_DJI_Power_Limit_Status_DISABLE)
    {
        Feedforward_Voltage = 0.0f;
        Feedforward_Current = 0.0f;
        Feedforward_Omega = 0.0f;
    }
}

/**
 * @brief TIM定时器中断功率控制善后计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Motor_DJI_GM6020::TIM_Power_Limit_After_Calculate_PeriodElapsedCallback()
{
    if (Power_Limit_Status == Motor_DJI_Power_Limit_Status_ENABLE)
    {
        Power_Limit_Control();
    }

    if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
    {
        float tmp_value = Target_Voltage + Feedforward_Voltage;
        Math_Constrain(&tmp_value, -Voltage_Max, Voltage_Max);
        Out = tmp_value * Voltage_To_Out;
    }
    else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
    {
        float tmp_value = Target_Current + Feedforward_Current;
        Math_Constrain(&tmp_value, -Current_Max, Current_Max);
        Out = tmp_value * Current_To_Out;
    }

    Output();

    Feedforward_Voltage = 0.0f;
    Feedforward_Current = 0.0f;
    Feedforward_Omega = 0.0f;
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Motor_DJI_GM6020::Data_Process()
{
    // 数据处理过程
    int16_t delta_encoder;
    uint16_t tmp_encoder;
    int16_t tmp_omega, tmp_current;
    Struct_Motor_DJI_CAN_Rx_Data *tmp_buffer = (Struct_Motor_DJI_CAN_Rx_Data *) CAN_Manage_Object->Rx_Buffer.Data;

    // 处理大小端
    Math_Endian_Reverse_16((void *) &tmp_buffer->Encoder_Reverse, (void *) &tmp_encoder);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Omega_Reverse, (void *) &tmp_omega);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Current_Reverse, (void *) &tmp_current);

    // 计算圈数与总编码器值
    delta_encoder = tmp_encoder - Rx_Data.Pre_Encoder;
    if (delta_encoder < -Encoder_Num_Per_Round / 2)
    {
        // 正方向转过了一圈
        Rx_Data.Total_Round++;
    }
    else if (delta_encoder > Encoder_Num_Per_Round / 2)
    {
        // 反方向转过了一圈
        Rx_Data.Total_Round--;
    }
    Rx_Data.Total_Encoder = Rx_Data.Total_Round * Encoder_Num_Per_Round + tmp_encoder + Encoder_Offset;

    // 计算电机本身信息
    Rx_Data.Now_Angle = (float) Rx_Data.Total_Encoder / (float) Encoder_Num_Per_Round * 2.0f * PI;
    Rx_Data.Now_Omega = (float) tmp_omega * RPM_TO_RADPS;
    Rx_Data.Now_Current = tmp_current / Current_To_Out;
    Rx_Data.Now_Temperature = tmp_buffer->Temperature + CELSIUS_TO_KELVIN;
    Rx_Data.Now_Power = power_calculate(Power_K_0, Power_K_1, Power_K_2, Power_A, Rx_Data.Now_Current, Rx_Data.Now_Omega);

    // 存储预备信息
    Rx_Data.Pre_Encoder = tmp_encoder;
}

/**
 * @brief 计算PID
 *
 */
void Class_Motor_DJI_GM6020::PID_Calculate()
{
    switch (Motor_DJI_Control_Method)
    {
    case (Motor_DJI_Control_Method_VOLTAGE):
    {
        if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
        {

        }
        else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
        {
            Target_Voltage = 0.0f;
            Target_Current = 0.0f;
        }

        break;
    }
    case (Motor_DJI_Control_Method_CURRENT):
    {
        if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
        {
            PID_Current.Set_Target(Target_Current + Feedforward_Current);
            PID_Current.Set_Now(Rx_Data.Now_Current);
            PID_Current.TIM_Calculate_PeriodElapsedCallback();

            Target_Voltage = PID_Current.Get_Out();
        }
        else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
        {

        }

        break;
    }
    case (Motor_DJI_Control_Method_OMEGA):
    {
        if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
        {
            PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
            PID_Omega.Set_Now(Rx_Data.Now_Omega);
            PID_Omega.TIM_Calculate_PeriodElapsedCallback();

            Target_Current = PID_Omega.Get_Out();

            PID_Current.Set_Target(Target_Current + Feedforward_Current);
            PID_Current.Set_Now(Rx_Data.Now_Current);
            PID_Current.TIM_Calculate_PeriodElapsedCallback();

            Target_Voltage = PID_Current.Get_Out();
        }
        else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
        {
            PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
            PID_Omega.Set_Now(Rx_Data.Now_Omega);
            PID_Omega.TIM_Calculate_PeriodElapsedCallback();

            Target_Current = PID_Omega.Get_Out();
        }

        break;
    }
    case (Motor_DJI_Control_Method_ANGLE):
    {
        if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
        {
            PID_Angle.Set_Target(Target_Angle);
            PID_Angle.Set_Now(Rx_Data.Now_Angle);
            PID_Angle.TIM_Calculate_PeriodElapsedCallback();

            Target_Omega = PID_Angle.Get_Out();

            PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
            PID_Omega.Set_Now(Rx_Data.Now_Omega);
            PID_Omega.TIM_Calculate_PeriodElapsedCallback();

            Target_Current = PID_Omega.Get_Out();

            PID_Current.Set_Target(Target_Current + Feedforward_Current);
            PID_Current.Set_Now(Rx_Data.Now_Current);
            PID_Current.TIM_Calculate_PeriodElapsedCallback();

            Target_Voltage = PID_Current.Get_Out();
        }
        else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
        {
            PID_Angle.Set_Target(Target_Angle);
            PID_Angle.Set_Now(Rx_Data.Now_Angle);
            PID_Angle.TIM_Calculate_PeriodElapsedCallback();

            Target_Omega = PID_Angle.Get_Out();

            PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
            PID_Omega.Set_Now(Rx_Data.Now_Omega);
            PID_Omega.TIM_Calculate_PeriodElapsedCallback();

            Target_Current = PID_Omega.Get_Out();
        }

        break;
    }
    default:
    {
        if (Driver_Version == Motor_DJI_GM6020_Driver_Version_DEFAULT)
        {
            Target_Voltage = 0.0f;
        }
        else if (Driver_Version == Motor_DJI_GM6020_Driver_Version_2023)
        {
            Target_Current = 0.0f;
        }

        break;
    }
    }
}

/**
 * @brief 功率控制算法, 修改电流目标值
 *
 */
void Class_Motor_DJI_GM6020::Power_Limit_Control()
{
    // 计算功率估计值
    Power_Estimate = power_calculate(Power_K_0, Power_K_1, Power_K_2, Power_A, Target_Current, Rx_Data.Now_Omega);

    // 若功率为正则考虑功率控制限制
    if (Power_Estimate > 0.0f)
    {
        if (Power_Factor >= 1.0f)
        {
            // 无需功率控制
        }
        else
        {
            // 需要功率控制

            // 根据功率估计公式解一元二次方程求电流值
            float a = Power_K_2;
            float b = Power_K_0 * Rx_Data.Now_Omega;
            float c = Power_A + Power_K_1 * Rx_Data.Now_Omega * Rx_Data.Now_Omega - Power_Factor * Power_Estimate;
            float delta, h;
            delta = b * b - 4 * a * c;
            if (delta < 0.0f)
            {
                // 无解
                Target_Current = 0.0f;
            }
            else
            {
                arm_sqrt_f32(delta, &h);
                float result_1, result_2;
                result_1 = (-b + h) / (2.0f * a);
                result_2 = (-b - h) / (2.0f * a);

                // 两个潜在的可行电流值, 取绝对值最小的那个
                if ((result_1 > 0.0f && result_2 < 0.0f) || (result_1 < 0.0f && result_2 > 0.0f))
                {
                    if ((Target_Current > 0.0f && result_1 > 0.0f) || (Target_Current < 0.0f && result_1 < 0.0f))
                    {
                        Target_Current = result_1;
                    }
                    else
                    {
                        Target_Current = result_2;
                    }
                }
                else
                {
                    if (Math_Abs(result_1) < Math_Abs(result_2))
                    {
                        Target_Current = result_1;
                    }
                    else
                    {
                        Target_Current = result_2;
                    }
                }
            }
        }
    }
}

/**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
void Class_Motor_DJI_GM6020::Output()
{
    Tx_Data[0] = (int16_t) Out >> 8;
    Tx_Data[1] = (int16_t) Out;
}

/**
 * @brief 电机初始化
 *
 * @param hcan CAN编号
 * @param __CAN_Rx_ID CAN ID
 * @param __Motor_DJI_Control_Method 电机控制方式, 默认角度
 * @param __Gearbox_Rate 减速箱减速比, 默认为原装减速箱, 如拆去减速箱则该值设为1
 * @param __Current_Max 最大电流
 */
void Class_Motor_DJI_C610::Init(CAN_HandleTypeDef *hcan, Enum_Motor_DJI_ID __CAN_Rx_ID, Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method, float __Gearbox_Rate, float __Current_Max)
{
    if (hcan->Instance == CAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == CAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    CAN_Rx_ID = __CAN_Rx_ID;
    Motor_DJI_Control_Method = __Motor_DJI_Control_Method;
    Gearbox_Rate = __Gearbox_Rate;
    Current_Max = __Current_Max;
    Tx_Data = allocate_tx_data(hcan, __CAN_Rx_ID);
}

/**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Motor_DJI_C610::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    // 滑动窗口, 判断电机是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Motor_DJI_C610::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Motor_DJI_Status = Motor_DJI_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
    }
    else
    {
        // 电机保持连接
        Motor_DJI_Status = Motor_DJI_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Motor_DJI_C610::TIM_Calculate_PeriodElapsedCallback()
{
    PID_Calculate();

    float tmp_value = Target_Current + Feedforward_Current;
    Math_Constrain(&tmp_value, -Current_Max, Current_Max);
    Out = tmp_value * Current_To_Out;

    Output();

    Feedforward_Current = 0.0f;
    Feedforward_Omega = 0.0f;
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Motor_DJI_C610::Data_Process()
{
    // 数据处理过程
    int16_t delta_encoder;
    uint16_t tmp_encoder;
    int16_t tmp_omega, tmp_current;
    Struct_Motor_DJI_CAN_Rx_Data *tmp_buffer = (Struct_Motor_DJI_CAN_Rx_Data *) CAN_Manage_Object->Rx_Buffer.Data;

    // 处理大小端
    Math_Endian_Reverse_16((void *) &tmp_buffer->Encoder_Reverse, (void *) &tmp_encoder);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Omega_Reverse, (void *) &tmp_omega);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Current_Reverse, (void *) &tmp_current);

    // 计算圈数与总编码器值
    delta_encoder = tmp_encoder - Rx_Data.Pre_Encoder;
    if (delta_encoder < -Encoder_Num_Per_Round / 2)
    {
        // 正方向转过了一圈
        Rx_Data.Total_Round++;
    }
    else if (delta_encoder > Encoder_Num_Per_Round / 2)
    {
        // 反方向转过了一圈
        Rx_Data.Total_Round--;
    }
    Rx_Data.Total_Encoder = Rx_Data.Total_Round * Encoder_Num_Per_Round + tmp_encoder;

    // 计算电机本身信息
    Rx_Data.Now_Angle = (float) Rx_Data.Total_Encoder / (float) Encoder_Num_Per_Round * 2.0f * PI / Gearbox_Rate;
    Rx_Data.Now_Omega = (float) tmp_omega * RPM_TO_RADPS / Gearbox_Rate;
    Rx_Data.Now_Current = tmp_current / Current_To_Out;
    Rx_Data.Now_Temperature = tmp_buffer->Temperature + CELSIUS_TO_KELVIN;

    // 存储预备信息
    Rx_Data.Pre_Encoder = tmp_encoder;
}

/**
 * @brief 计算PID
 *
 */
void Class_Motor_DJI_C610::PID_Calculate()
{
    switch (Motor_DJI_Control_Method)
    {
    case (Motor_DJI_Control_Method_CURRENT):
    {
        break;
    }
    case (Motor_DJI_Control_Method_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Current = PID_Omega.Get_Out();

        break;
    }
    case (Motor_DJI_Control_Method_ANGLE):
    {
        PID_Angle.Set_Target(Target_Angle);
        PID_Angle.Set_Now(Rx_Data.Now_Angle);
        PID_Angle.TIM_Calculate_PeriodElapsedCallback();

        Target_Omega = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Current = PID_Omega.Get_Out();

        break;
    }
    default:
    {
        Target_Current = 0.0f;

        break;
    }
    }
}

/**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
void Class_Motor_DJI_C610::Output()
{
    Tx_Data[0] = (int16_t) Out >> 8;
    Tx_Data[1] = (int16_t) Out;
}

/**
 * @brief 电机初始化
 *
 * @param hcan CAN编号
 * @param __CAN_Rx_ID CAN ID
 * @param __Motor_DJI_Control_Method 电机控制方式, 默认速度
 * @param __Gearbox_Rate 减速箱减速比, 默认为原装减速箱, 如拆去减速箱则该值设为1
 * @param __Power_Limit_Status 是否开启功率控制
 * @param __Current_Max 最大电流
 */
void Class_Motor_DJI_C620::Init(CAN_HandleTypeDef *hcan, Enum_Motor_DJI_ID __CAN_Rx_ID, Enum_Motor_DJI_Control_Method __Motor_DJI_Control_Method, float __Gearbox_Rate, Enum_Motor_DJI_Power_Limit_Status __Power_Limit_Status, float __Current_Max)
{
    if (hcan->Instance == CAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == CAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    CAN_Rx_ID = __CAN_Rx_ID;
    Motor_DJI_Control_Method = __Motor_DJI_Control_Method;
    Power_Limit_Status = __Power_Limit_Status;
    Gearbox_Rate = __Gearbox_Rate;
    Current_Max = __Current_Max;
    Tx_Data = allocate_tx_data(hcan, __CAN_Rx_ID);
}

/**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Motor_DJI_C620::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    // 滑动窗口, 判断电机是否在线
    Flag += 1;

    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Motor_DJI_C620::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过电机数据
    if (Flag == Pre_Flag)
    {
        // 电机断开连接
        Motor_DJI_Status = Motor_DJI_Status_DISABLE;
        Motor_DJI_Control_Method = Motor_DJI_Control_Method_CURRENT;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
        Target_Current = 0.0f;
    }
    else
    {
        // 电机保持连接
        Motor_DJI_Status = Motor_DJI_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Motor_DJI_C620::TIM_Calculate_PeriodElapsedCallback()
{
    PID_Calculate();

    float tmp_value = Target_Current + Feedforward_Current;
    Math_Constrain(&tmp_value, -Current_Max, Current_Max);
    Out = tmp_value * Current_To_Out;

    Output();

    if (Power_Limit_Status == Motor_DJI_Power_Limit_Status_DISABLE)
    {
        Feedforward_Current = 0.0f;
        Feedforward_Omega = 0.0f;
    }
}

/**
 * @brief TIM定时器中断功率控制善后计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Motor_DJI_C620::TIM_Power_Limit_After_Calculate_PeriodElapsedCallback()
{
    if (Power_Limit_Status == Motor_DJI_Power_Limit_Status_ENABLE)
    {
        Power_Limit_Control();
    }

    Math_Constrain(&Target_Current, -Current_Max, Current_Max);
    Out = Target_Current * Current_To_Out;

    Output();

    Feedforward_Current = 0.0f;
    Feedforward_Omega = 0.0f;
}

/**
 * @brief 数据处理过程
 *
 */
void Class_Motor_DJI_C620::Data_Process()
{
    // 数据处理过程
    int16_t delta_encoder;
    uint16_t tmp_encoder;
    int16_t tmp_omega, tmp_current;
    Struct_Motor_DJI_CAN_Rx_Data *tmp_buffer = (Struct_Motor_DJI_CAN_Rx_Data *) CAN_Manage_Object->Rx_Buffer.Data;

    // 处理大小端
    Math_Endian_Reverse_16((void *) &tmp_buffer->Encoder_Reverse, (void *) &tmp_encoder);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Omega_Reverse, (void *) &tmp_omega);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Current_Reverse, (void *) &tmp_current);

    // 计算圈数与总编码器值
    delta_encoder = tmp_encoder - Rx_Data.Pre_Encoder;
    if (delta_encoder < -Encoder_Num_Per_Round / 2)
    {
        // 正方向转过了一圈
        Rx_Data.Total_Round++;
    }
    else if (delta_encoder > Encoder_Num_Per_Round / 2)
    {
        // 反方向转过了一圈
        Rx_Data.Total_Round--;
    }
    Rx_Data.Total_Encoder = Rx_Data.Total_Round * Encoder_Num_Per_Round + tmp_encoder;

    // 计算电机本身信息
    Rx_Data.Now_Angle = (float) Rx_Data.Total_Encoder / (float) Encoder_Num_Per_Round * 2.0f * PI / Gearbox_Rate;
    Rx_Data.Now_Omega = (float) tmp_omega * RPM_TO_RADPS / Gearbox_Rate;
    Rx_Data.Now_Current = tmp_current / Current_To_Out;
    Rx_Data.Now_Temperature = tmp_buffer->Temperature + CELSIUS_TO_KELVIN;
    Rx_Data.Now_Power = power_calculate(Power_K_0, Power_K_1, Power_K_2, Power_A, Rx_Data.Now_Current, Rx_Data.Now_Omega);

    // 存储预备信息
    Rx_Data.Pre_Encoder = tmp_encoder;
}

/**
 * @brief 计算PID
 *
 */
void Class_Motor_DJI_C620::PID_Calculate()
{
    switch (Motor_DJI_Control_Method)
    {
    case (Motor_DJI_Control_Method_CURRENT):
    {
        break;
    }
    case (Motor_DJI_Control_Method_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Current = PID_Omega.Get_Out();

        break;
    }
    case (Motor_DJI_Control_Method_ANGLE):
    {
        PID_Angle.Set_Target(Target_Angle);
        PID_Angle.Set_Now(Rx_Data.Now_Angle);
        PID_Angle.TIM_Calculate_PeriodElapsedCallback();

        Target_Omega = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega + Feedforward_Omega);
        PID_Omega.Set_Now(Rx_Data.Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        Target_Current = PID_Omega.Get_Out();

        break;
    }
    default:
    {
        Target_Current = 0.0f;

        break;
    }
    }
}

/**
 * @brief 功率控制算法, 修改电流目标值
 *
 */
void Class_Motor_DJI_C620::Power_Limit_Control()
{
    // 计算功率估计值
    Power_Estimate = power_calculate(Power_K_0, Power_K_1, Power_K_2, Power_A, Target_Current, Rx_Data.Now_Omega);

    // 若功率为正则考虑功率控制限制
    if (Power_Estimate > 0.0f)
    {
        if (Power_Factor >= 1.0f)
        {
            // 无需功率控制
        }
        else
        {
            // 需要功率控制

            // 根据功率估计公式解一元二次方程求电流值
            float a = Power_K_2;
            float b = Power_K_0 * Rx_Data.Now_Omega;
            float c = Power_A + Power_K_1 * Rx_Data.Now_Omega * Rx_Data.Now_Omega - Power_Factor * Power_Estimate;
            float delta, h;
            delta = b * b - 4 * a * c;
            if (delta < 0.0f)
            {
                // 无解
                Target_Current = 0.0f;
            }
            else
            {
                arm_sqrt_f32(delta, &h);
                float result_1, result_2;
                result_1 = (-b + h) / (2.0f * a);
                result_2 = (-b - h) / (2.0f * a);

                // 两个潜在的可行电流值, 取绝对值最小的那个
                if ((result_1 > 0.0f && result_2 < 0.0f) || (result_1 < 0.0f && result_2 > 0.0f))
                {
                    if ((Target_Current > 0.0f && result_1 > 0.0f) || (Target_Current < 0.0f && result_1 < 0.0f))
                    {
                        Target_Current = result_1;
                    }
                    else
                    {
                        Target_Current = result_2;
                    }
                }
                else
                {
                    if (Math_Abs(result_1) < Math_Abs(result_2))
                    {
                        Target_Current = result_1;
                    }
                    else
                    {
                        Target_Current = result_2;
                    }
                }
            }
        }
    }
}

/**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
void Class_Motor_DJI_C620::Output()
{
    Tx_Data[0] = (int16_t) Out >> 8;
    Tx_Data[1] = (int16_t) Out;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
