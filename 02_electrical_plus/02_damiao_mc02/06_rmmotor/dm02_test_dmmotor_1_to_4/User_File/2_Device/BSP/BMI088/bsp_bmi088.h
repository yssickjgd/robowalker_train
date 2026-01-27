/**
 * @file bsp_bmi088.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief BMI088组件之加速度计, 内含加热电阻
 * @version 0.1
 * @date 2025-08-26 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef BSP_BMI088_H
#define BSP_BMI088_H

/* Includes ------------------------------------------------------------------*/

#include "Accel/bsp_bmi088_accel.h"
#include "Gyro/bsp_bmi088_gyro.h"
#include "1_Middleware/Algorithm/Quaternion/alg_quaternion.h"
#include "1_Middleware/Algorithm/Filter/EKF/alg_filter_ekf.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Specialized, 板载AHRS
 *
 */
class Class_BMI088
{
public:
    Class_BMI088_Accel BMI088_Accel;
    Class_BMI088_Gyro BMI088_Gyro;

    Class_Filter_EKF<4, 3, 3> EKF_Quaternion;

    void Init();

    inline Class_Matrix_f32<3, 1> Get_Original_Accel() const;

    inline Class_Matrix_f32<3, 1> Get_Original_Gyro() const;

    inline Class_Matrix_f32<3, 1> Get_Euler_Angle() const;

    inline Class_Matrix_f32<3, 3> Get_Rotation_Matrix() const;

    inline Class_Matrix_f32<4, 1> Get_Axis_Angle() const;

    inline Class_Quaternion_f32 Get_Quaternion() const;

    inline Class_Matrix_f32<3, 1> Get_Accel_Body();

    inline Class_Matrix_f32<3, 1> Get_Gyro_Body();

    inline Class_Matrix_f32<3, 1> Get_Accel();

    inline Class_Matrix_f32<3, 1> Get_Gyro();

    inline float Get_Accel_Chi_Square_Loss() const;

    inline uint64_t Get_Calculating_Time() const;

    void SPI_RxCpltCallback();

    void EXTI_Flag_Callback(uint16_t GPIO_Pin);

    void TIM_128ms_Calculate_PeriodElapsedCallback();

    void TIM_125us_Calculate_PeriodElapsedCallback();

    void TIM_10us_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的SPI
    Struct_SPI_Manage_Object *SPI_Manage_Object;

    // 常量

    // 数据传输超时时间, 单位us
    uint64_t TRANSFERING_TIMEOUT = 20;

    // D_T超时时间阈值
    float D_T_TIMEOUT_THRESHOLD = 0.1f;

    // 卡方检验残差阈值
    float ACCEL_CHI_SQUARE_TEST_THRESHOLD = 5.0f;

    // 校正数据, 与温控有关, 温控在50℃
    // 加速度计仿射矩阵源数据
    const float ACCEL_AFFINE_DATA[9] = {0.9813826498493404f, 0.17232440504057203f, 0.027984325323801115f, -0.1690535919907899f, 0.9747302115792275f, -0.10336863799715153f, -0.046825636945091266f, 0.09953521655990044f, 0.986897809387138f};
    // 加速度计偏置
    const float ACCEL_BIAS_DATA[3] = {0.0038458286072392397, 0.00647039594993548f, 0.014968990490337293f};
    // 陀螺仪偏置
    const float GYRO_ZERO_OFFSET[3] = {-0.005280993487f, -0.000237223741f, -0.000647540528f};

    // 内部变量

    // EKF初始化状态完成标志
    bool EKF_Init_Finished_Flag = false;

    // 初始化完成标志
    bool Init_Finished_Flag = false;

    // 数据准备好标志
    bool Accel_Ready_Flag = false;
    uint64_t Accel_Ready_Timestamp = 0;
    bool Gyro_Ready_Flag = false;
    uint64_t Gyro_Ready_Timestamp = 0;
    bool Temperature_Ready_Flag = false;
    // 数据传输标志
    bool Accel_Transfering_Flag = false;
    uint64_t Accel_Transfering_Timestamp = 0;
    bool Gyro_Transfering_Flag = false;
    uint64_t Gyro_Transfering_Timestamp = 0;
    bool Temperature_Transfering_Flag = false;
    uint64_t Temperature_Transfering_Timestamp = 0;
    // 数据更新标志
    bool Accel_Update_Flag = false;
    uint64_t Accel_Update_Timestamp = 0;
    bool Gyro_Update_Flag = false;
    uint64_t Gyro_Update_Timestamp = 0;

    // 上一次陀螺仪源数据
    Class_Matrix_f32<3, 1> Vector_Pre_Original_Gyro;

    // 加速度计归一化数据
    Class_Matrix_f32<3, 1> Vector_Normalized_Accel;

    // EKF计算时间戳
    uint64_t EKF_Now_Timestamp = 0;
    // 上次EKF计算时间戳
    uint64_t EKF_Pre_Timestamp = 0;

    // 时间差
    float D_T = 0.000125f;

    // 读变量

    // 加速度计源数据
    Class_Matrix_f32<3, 1> Vector_Original_Accel;
    // 陀螺仪源数据
    Class_Matrix_f32<3, 1> Vector_Original_Gyro;

    // 欧拉角, Yaw-Pitch-Roll顺序
    Class_Matrix_f32<3, 1> Vector_Euler_Angle;
    // 旋转矩阵
    Class_Matrix_f32<3, 3> Matrix_Rotation;
    // 轴角式
    Class_Matrix_f32<4, 1> Vector_Axis_Angle;
    // 四元数
    Class_Quaternion_f32 Quarternion;

    // 机体坐标系下的加速度
    Class_Matrix_f32<3, 1> Vector_Accel_Body;
    // 机体坐标系下的角速度
    Class_Matrix_f32<3, 1> Vector_Gyro_Body;
    // 大地坐标系下的加速度
    Class_Matrix_f32<3, 1> Vector_Accel;
    // 大地坐标系下的角速度
    Class_Matrix_f32<3, 1> Vector_Gyro;

    // 卡方检验值
    float Accel_Chi_Square_Loss = 0.0f;
    // 处理时间
    uint64_t Calculating_Time = 0;

    // 写变量

    // 读写变量

    // 内部函数

    // EKF的相关函数
    // 四元数状态转移函数
    static Class_Matrix_f32<4, 1> EKF_Function_F(const Class_Matrix_f32<4, 1> &Vector_X, const Class_Matrix_f32<3, 1> &Vector_U, const float &D_T);

    // 四元数状态转移函数对状态的雅可比矩阵
    static Class_Matrix_f32<4, 4> EKF_Function_Jacobian_F_X(const Class_Matrix_f32<4, 1> &Vector_X, const Class_Matrix_f32<3, 1> &Vector_U, const float &D_T);

    // 四元数状态转移函数对过程噪声的雅可比矩阵
    static Class_Matrix_f32<4, 3> EKF_Function_Jacobian_F_W(const Class_Matrix_f32<4, 1> &Vector_X, const Class_Matrix_f32<3, 1> &Vector_U, const float &D_T);

    // 四元数测量函数
    static Class_Matrix_f32<3, 1> EKF_Function_H(const Class_Matrix_f32<4, 1> &Vector_X, const float &D_T);

    // 四元数测量函数对状态的雅可比矩阵
    static Class_Matrix_f32<3, 4> EKF_Function_Jacobian_H_X(const Class_Matrix_f32<4, 1> &Vector_X, const float &D_T);

    // 四元数测量函数对测量噪声的雅可比矩阵
    static Class_Matrix_f32<3, 3> EKF_Function_Jacobian_H_V(const Class_Matrix_f32<4, 1> &Vector_X, const float &D_T);

    void Accel_Chi_Square_Calculate();
};

/* Exported variables --------------------------------------------------------*/

extern Class_BMI088 BSP_BMI088;

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取加速度计原始数据
 *
 * @return 加速度计原始数据, 单位m/s²
 */
inline Class_Matrix_f32<3, 1> Class_BMI088::Get_Original_Accel() const
{
    return (Vector_Original_Accel);
}

/**
 * @brief 获取陀螺仪原始数据
 *
 * @return 陀螺仪原始数据, 单位rad/s
 */
inline Class_Matrix_f32<3, 1> Class_BMI088::Get_Original_Gyro() const
{
    return (Vector_Original_Gyro);
}

/**
 * @brief 获取Euler角
 *
 * @return Euler角, 单位rad
 */
inline Class_Matrix_f32<3, 1> Class_BMI088::Get_Euler_Angle() const
{
    return (Vector_Euler_Angle);
}

/**
 * @brief 获取旋转矩阵
 *
 * @return 旋转矩阵
 */
inline Class_Matrix_f32<3, 3> Class_BMI088::Get_Rotation_Matrix() const
{
    return (Matrix_Rotation);
}

/**
 * @brief 获取轴角式的轴
 *
 * @return 轴角式
 */
inline Class_Matrix_f32<4, 1> Class_BMI088::Get_Axis_Angle() const
{
    return (Vector_Axis_Angle);
}

/**
 * @brief 获取四元数
 *
 */
inline Class_Quaternion_f32 Class_BMI088::Get_Quaternion() const
{
    return (Quarternion);
}

/**
 * @brief 获取机体坐标系下的加速度
 *
 */
inline Class_Matrix_f32<3, 1> Class_BMI088::Get_Accel_Body()
{
    return (Vector_Accel_Body);
}

/**
 * @brief 获取机体坐标系下的角速度
 *
 */
inline Class_Matrix_f32<3, 1> Class_BMI088::Get_Gyro_Body()
{
    return (Vector_Gyro_Body);
}

/**
 * @brief 获取大地坐标系下的加速度
 *
 */
inline Class_Matrix_f32<3, 1> Class_BMI088::Get_Accel()
{
    return (Vector_Accel);
}

/**
 * @brief 获取大地坐标系下的角速度
 *
 */
inline Class_Matrix_f32<3, 1> Class_BMI088::Get_Gyro()
{
    return (Vector_Gyro);
}

/**
 * @brief 获取加速度计卡方检验残差
 *
 * @return 加速度计卡方检验残差
 */
inline float Class_BMI088::Get_Accel_Chi_Square_Loss() const
{
    return (Accel_Chi_Square_Loss);
}

/**
 * @brief 获取计算时间
 *
 * @return 计算时间, 单位s
 */
inline uint64_t Class_BMI088::Get_Calculating_Time() const
{
    return (Calculating_Time);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
