/**
 * @file bsp_bmi088_gyro.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief BMI088组件之陀螺仪
 * @version 0.1
 * @date 2025-08-19 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "bsp_bmi088_gyro.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化BMI088加速度计
 *
 * @param Heater_Enable 是否使能加热电阻
 */
void Class_BMI088_Gyro::Init()
{
    // 绑定SPI
    SPI_Manage_Object = &SPI2_Manage_Object;

    // 绑定片选
    CS_GPIO_Port = BMI088_GYRO__SPI_CS_GPIO_Port;
    CS_Pin = BMI088_GYRO__SPI_CS_Pin;
    Activate_Pin_State = GPIO_PIN_RESET;

    uint8_t res;

    // 检测通信是否正常
    Register.GYRO_CHIP_ID_RO = 0x00;
    while (Register.GYRO_CHIP_ID_RO != 0x0f)
    {
        Read_Single_Register(offsetof(Struct_BMI088_Gyro_Register, GYRO_CHIP_ID_RO));
        Namespace_SYS_Timestamp::Delay_Millisecond(100);
    }

    // 软重启
    res = 0xb6;
    Write_Single_Register(offsetof(Struct_BMI088_Gyro_Register, GYRO_SOFTRESET_WO), &res);
    Namespace_SYS_Timestamp::Delay_Millisecond(100);

    // 检测通信是否正常
    Register.GYRO_CHIP_ID_RO = 0x00;
    while (Register.GYRO_CHIP_ID_RO != 0x0f)
    {
        Read_Single_Register(offsetof(Struct_BMI088_Gyro_Register, GYRO_CHIP_ID_RO));
        Namespace_SYS_Timestamp::Delay_Millisecond(100);
    }

    for (uint8_t i = 0; i < BMI088_GYRO_INIT_INSTRUCTION_NUM; i++)
    {
        ((uint8_t *) (&Register))[BMI088_GYRO_REGISTER_CONFIG[i][0]] = 0x00;
        while (((uint8_t *) (&Register))[BMI088_GYRO_REGISTER_CONFIG[i][0]] != BMI088_GYRO_REGISTER_CONFIG[i][1])
        {
            // 写入寄存器
            Write_Single_Register(BMI088_GYRO_REGISTER_CONFIG[i][0], &BMI088_GYRO_REGISTER_CONFIG[i][1]);
            Namespace_SYS_Timestamp::Delay_Millisecond(100);

            // 读取寄存器
            Read_Single_Register(BMI088_GYRO_REGISTER_CONFIG[i][0]);
            Namespace_SYS_Timestamp::Delay_Millisecond(100);
        }
    }

    // 预读取一次陀螺仪数据
    Read_Multi_Register(offsetof(Struct_BMI088_Gyro_Register, RATE_X_RO), 6);
    Namespace_SYS_Timestamp::Delay_Millisecond(100);
}

/**
 * @brief SPI接收回调函数, 处理加速度计数据
 *
 */
void Class_BMI088_Gyro::SPI_RxCallback()
{
    uint8_t spi_init_address = SPI_Manage_Object->Tx_Buffer[0] & ~BMI088_GYRO_READ_MASK;

    memcpy((uint8_t *) (&Register) + spi_init_address, &SPI_Manage_Object->Rx_Buffer[1], SPI_Manage_Object->Rx_Buffer_Length);

    // 处理数据
    if (spi_init_address == offsetof(Struct_BMI088_Gyro_Register, RATE_X_RO))
    {
        // 读取陀螺仪数据完成

        Vector_Raw_Gyro[0][0] = (float) (Register.RATE_X_RO) / 32768.0f * (1 << (4 - BMI088_GYRO_RANGE)) * 125.0f * BASIC_MATH_DEG_TO_RAD;
        Vector_Raw_Gyro[1][0] = (float) (Register.RATE_Y_RO) / 32768.0f * (1 << (4 - BMI088_GYRO_RANGE)) * 125.0f * BASIC_MATH_DEG_TO_RAD;
        Vector_Raw_Gyro[2][0] = (float) (Register.RATE_Z_RO) / 32768.0f * (1 << (4 - BMI088_GYRO_RANGE)) * 125.0f * BASIC_MATH_DEG_TO_RAD;

        if (Basic_Math_Is_Invalid_Float(Vector_Raw_Gyro[0][0]) || Basic_Math_Is_Invalid_Float(Vector_Raw_Gyro[1][0]) || Basic_Math_Is_Invalid_Float(Vector_Raw_Gyro[2][0]))
        {
            Valid_Flag = false;
        }
        else
        {
            Valid_Flag = true;
        }
    }
}

/**
 * @brief TIM定时器中断回调函数, 100us周期
 *
 */
void Class_BMI088_Gyro::SPI_Request_Gyro()
{
    // 读取陀螺仪数据
    Read_Multi_Register(offsetof(Struct_BMI088_Gyro_Register, RATE_X_RO), 6);
}

/**
 * @brief 读取单个寄存器, 数据不会立即返回, 而是在SPI接收回调函数中处理
 *
 * @param Register_Address 寄存器地址
 */
void Class_BMI088_Gyro::Read_Single_Register(const uint8_t &Register_Address) const
{
    SPI_Manage_Object->Tx_Buffer[0] = Register_Address | BMI088_GYRO_READ_MASK;

    SPI_Transmit_Receive_Data(SPI_Manage_Object->SPI_Handler, CS_GPIO_Port, CS_Pin, Activate_Pin_State, 1, 1);
}

/** * @brief 读取多个寄存器, 数据不会立即返回, 而是在SPI接收回调函数中处理
 *
 * @param Register_Address 寄存器地址
 * @param Rx_Length 接收数据长度
 */
void Class_BMI088_Gyro::Read_Multi_Register(const uint8_t &Register_Address, const uint32_t &Rx_Length) const
{
    SPI_Manage_Object->Tx_Buffer[0] = Register_Address | BMI088_GYRO_READ_MASK;

    SPI_Transmit_Receive_Data(SPI_Manage_Object->SPI_Handler, CS_GPIO_Port, CS_Pin, Activate_Pin_State, 1, Rx_Length);
}

/**
 * @brief 写入单个寄存器
 *
 * @param Register_Address 寄存器地址
 * @param Tx_Data_Buffer 发送数据缓冲区
 */
void Class_BMI088_Gyro::Write_Single_Register(const uint8_t &Register_Address, const uint8_t *Tx_Data_Buffer) const
{
    SPI_Manage_Object->Tx_Buffer[0] = Register_Address;
    SPI_Manage_Object->Tx_Buffer[1] = Tx_Data_Buffer[0];

    SPI_Transmit_Data(SPI_Manage_Object->SPI_Handler, CS_GPIO_Port, CS_Pin, Activate_Pin_State, 2);
}

/**
 * @brief 写入多个寄存器
 *
 * @param Register_Address 寄存器地址
 * @param Tx_Data_Buffer 发送数据缓冲区
 * @param Tx_Length 发送数据长度
 */
void Class_BMI088_Gyro::Write_Multi_Register(const uint8_t &Register_Address, const uint8_t *Tx_Data_Buffer, const uint32_t &Tx_Length) const
{
    SPI_Manage_Object->Tx_Buffer[0] = Register_Address;
    memcpy(&SPI_Manage_Object->Tx_Buffer[1], Tx_Data_Buffer, Tx_Length);

    SPI_Transmit_Data(SPI_Manage_Object->SPI_Handler, CS_GPIO_Port, CS_Pin, Activate_Pin_State, Tx_Length);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/