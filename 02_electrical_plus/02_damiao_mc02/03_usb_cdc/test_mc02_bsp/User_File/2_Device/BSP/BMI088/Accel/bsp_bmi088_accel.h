/**
 * @file bsp_bmi088_accel.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief BMI088组件之加速度计, 内含加热电阻
 * @version 0.1
 * @date 2025-08-14 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef BSP_BMI088_ACCEL_H
#define BSP_BMI088_ACCEL_H

/* Includes ------------------------------------------------------------------*/

#include "bsp_bmi088_accel_register.h"
#include "2_Device/BSP/Power/bsp_power.h"
#include "1_Middleware/Algorithm/PID/alg_pid.h"
#include "1_Middleware/Driver/SPI/drv_spi.h"
#include "1_Middleware/Algorithm/Matrix/alg_matrix.h"
#include "stm32h7xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 加速度计量程枚举类型
 *
 */
enum Enum_BSP_BMI088_Accel_Range : uint8_t
{
    BMI088_ACCEL_RANGE_3G = 0x00,
    BMI088_ACCEL_RANGE_6G,
    BMI088_ACCEL_RANGE_12G,
    BMI088_ACCEL_RANGE_24G,
};

/**
 * @brief Specialized, 加速度计
 *
 */
class Class_BMI088_Accel
{
public:
    // 温控PID
    Class_PID PID_Temperature;

    void Init(const bool &__Heater_Enable = false);

    inline float Get_Now_Temperature() const;

    inline bool Get_Valid_Flag() const;

    inline Class_Matrix_f32<3, 1> Get_Raw_Accel() const;

    inline float Get_Heater_Enable() const;

    inline void Set_Target_Temperature(const float &__Target_Temperature);

    inline void Set_Heater_Enable(const bool &__Heater_Enable);

    void SPI_RxCpltCallback();

    void SPI_Request_Accel();

    void SPI_Request_Temperature();

    void TIM_128ms_Heater_PID_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的SPI
    Struct_SPI_Manage_Object *SPI_Manage_Object;
    // 片选绑定的GPIO
    GPIO_TypeDef *CS_GPIO_Port;
    uint16_t CS_Pin;
    GPIO_PinState Activate_Pin_State;
    // 加热电阻绑定的定时器
    TIM_HandleTypeDef *htim;
    uint32_t TIM_Channel;

    // 常量

    // 读取寄存器时需要设置的掩码
    const uint8_t BMI088_GYRO_READ_MASK = 0x80;
    // 读数据模式下, SPI发送数据完毕后还需再发送1字节的保留字节, 该字节在SPI接收数据时会被忽略
    const uint8_t BMI088_GYRO_SPI_RX_RESERVED = 1;
    // 初始化指令数
    const uint8_t BMI088_ACCEL_INIT_INSTRUCTION_NUM = 6;
    // 加速度计量程, 默认±24g
    const Enum_BSP_BMI088_Accel_Range BMI088_ACCEL_RANGE = BMI088_ACCEL_RANGE_24G;
    // 寄存器配置相关
    const uint8_t BMI088_GYRO_REGISTER_CONFIG[6][2] = {
        // 开启加速度计电源
        {offsetof(Struct_BMI088_Accel_Register, ACC_PWR_CTRL_RW), 0x04},
        // 将加速度计从默认挂起状态0x03改为工作状态0x00
        {offsetof(Struct_BMI088_Accel_Register, ACC_PWR_CONF_RW), 0x00},
        // 加速度计无滤波器, 频率1600Hz
        {offsetof(Struct_BMI088_Accel_Register, ACC_CONF_RW), (0x0a << 4) | 0x0c},
        // 加速度计量程
        {offsetof(Struct_BMI088_Accel_Register, ACC_RANGE_RW), BMI088_ACCEL_RANGE},
        // 中断1号引脚配置推挽输出模式
        {offsetof(Struct_BMI088_Accel_Register, INT1_IO_CTRL_RW), 0x01 << 3},
        // 配置为如果数据准备好就中断
        {offsetof(Struct_BMI088_Accel_Register, INT_MAP_DATA_RW), 0x01 << 2},
    };

    // 加热电阻标称电压
    float HEATER_NOMINAL_VOLTAGE = 25.2f;
    // 加热电阻预热功率, 单位与PID等同
    float HEATER_PREHEAT_POWER = 1000.0f;
    // 触发预热基础温度
    float HEATER_PREHEAT_BASE_TEMPERATURE = 45.0f;
    // 加热电阻目标温度
    float HEATER_TARGET_TEMPERATURE = 50.0f;

    // 内部变量

    // 寄存器结构体
    Struct_BMI088_Accel_Register Register = {0};

    // 加热电阻预热时间戳
    bool Heater_Preheat_Finished_Flag = false;

    // 读变量

    // 当前温度
    float Now_Temperature = 0.0f;

    // 当前加速度是否有效
    bool Valid_Flag = true;
    // 当前加速度
    Class_Matrix_f32<3, 1> Vector_Raw_Accel;;

    // 写变量

    // 目标温度
    float Target_Temperature = 0.0f;

    // 读写变量

    // 加热电阻使能
    bool Heater_Enable = false;

    // 内部函数

    void Read_Single_Register(const uint8_t &Register_Address) const;

    void Read_Multi_Register(const uint8_t &Register_Address, const uint32_t &Rx_Length) const;

    void Write_Single_Register(const uint8_t &Register_Address, const uint8_t *Tx_Data_Buffer) const;

    void Write_Multi_Register(const uint8_t &Register_Address, const uint8_t *Tx_Data_Buffer, const uint32_t &Tx_Length) const;
};

/* Exported variables --------------------------------------------------------*/

extern const float GRAVITY_ACCELERATION;

extern Class_Power BSP_Power;

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取当前温度
 *
 * @return 当前温度
 */
inline float Class_BMI088_Accel::Get_Now_Temperature() const
{
    return (Now_Temperature);
}

/**
 * @brief 获取当前加速度是否有效
 *
 * @return 当前加速度是否有效
 */
inline bool Class_BMI088_Accel::Get_Valid_Flag() const
{
    return (Valid_Flag);
}

/**
 * @brief 获取当前加速度原始数据
 *
 * @return 当前加速度原始数据
 */
inline Class_Matrix_f32<3, 1> Class_BMI088_Accel::Get_Raw_Accel() const
{
    return (Vector_Raw_Accel);
}

/**
 * @brief 获取加热电阻使能
 *
 * @return 加热电阻使能
 */
inline float Class_BMI088_Accel::Get_Heater_Enable() const
{
    return (Heater_Enable);
}

/**
 * @brief 设置目标温度
 *
 * @param __Target_Temperature 目标温度
 */
inline void Class_BMI088_Accel::Set_Target_Temperature(const float &__Target_Temperature)
{
    Target_Temperature = __Target_Temperature;
}

/**
 * @brief 设置加热电阻使能
 *
 * @param __Heater_Enable 加热电阻使能
 */
inline void Class_BMI088_Accel::Set_Heater_Enable(const bool &__Heater_Enable)
{
    Heater_Enable = __Heater_Enable;
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/