/**
 * @file dvc_serialplot.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 串口绘图
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2025-08-14 1.1 适配达妙MC02板, 并引入USB虚拟串口的串口绘图
 *
 * @copyright USTC-RoboWalker (c) 2023-2025
 *
 */

#ifndef DVC_SERIALPLOT_H
#define DVC_SERIALPLOT_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Algorithm/Basic/alg_basic.h"
#include "1_Middleware/Driver/UART/drv_uart.h"
#include "1_Middleware/Driver/USB/drv_usb.h"
#include <stdarg.h>

/* Exported macros -----------------------------------------------------------*/

#define SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH (100)

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 是否开启校验和
 *
 */
enum Enum_Serialplot_Checksum_8
{
    Serialplot_Checksum_8_DISABLE = 0,
    Serialplot_Checksum_8_ENABLE,
};

/**
 * @brief 串口绘图传输数据类型
 *
 */
enum Enum_Serialplot_Data_Type
{
    Serialplot_Data_Type_UINT8 = 0,
    Serialplot_Data_Type_UINT16,
    Serialplot_Data_Type_UINT32,
    Serialplot_Data_Type_INT8,
    Serialplot_Data_Type_INT16,
    Serialplot_Data_Type_INT32,
    Serialplot_Data_Type_FLOAT,
    Serialplot_Data_Type_DOUBLE,
};

/**
 * @brief Reusable, 串口绘图工具, 最多支持25个通道
 *
 */
class Class_Serialplot_UART
{
public:
    void Init(const UART_HandleTypeDef *huart, const Enum_Serialplot_Checksum_8 &__Checksum_8 = Serialplot_Checksum_8_ENABLE, const uint8_t &__Rx_Variable_Assignment_Num = 0, const char **__Rx_Variable_Assignment_List = NULL, const Enum_Serialplot_Data_Type &__Data_Type = Serialplot_Data_Type_FLOAT, const uint8_t &__Frame_Header = 0xab);

    inline int32_t Get_Variable_Index() const;

    inline float Get_Variable_Value() const;

    inline void Set_Data(const int &Number, ...);

    void UART_RxCpltCallback(const uint8_t *Rx_Data, const uint16_t &Length);

    void TIM_1ms_Write_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;
    // 是否开启校验和
    Enum_Serialplot_Checksum_8 Checksum_8;
    // 接收指令字典的数量
    uint8_t Rx_Variable_Num;
    // 接收指令字典列表指针
    char **Rx_Variable_List;
    // 串口绘图数据类型
    Enum_Serialplot_Data_Type Tx_Data_Type;
    // 数据包头标
    uint8_t Frame_Header;

    // 常量

    // 内部变量

    // 发送缓冲区
    uint8_t Tx_Buffer[UART_BUFFER_SIZE];

    // 需要绘图的各个变量数据地址, 软件限制只有24个通道, 多了闪退
    const void *Data[24];
    // 当前发送的数据长度, 等价于新数据偏移量
    uint8_t Data_Number = 0;
    // 当前接收的指令在指令字典中的编号
    int32_t Variable_Index = 0;
    // 当前接收的指令在指令字典中的值
    float Variable_Value = 0.0f;

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数

    void Data_Process(const uint16_t &Length);

    uint8_t _Judge_Variable_Name(const uint16_t &Length);

    void _Judge_Variable_Value(const uint16_t &Length, int flag);

    void Output();
};

/**
 * @brief Reusable, 串口绘图工具, 最多支持25个通道
 *
 */
class Class_Serialplot_USB
{
public:
    void Init(const Enum_Serialplot_Checksum_8 &__Checksum_8 = Serialplot_Checksum_8_ENABLE, const uint8_t &__Rx_Variable_Assignment_Num = 0, const char **__Rx_Variable_Assignment_List = NULL, const Enum_Serialplot_Data_Type &__Data_Type = Serialplot_Data_Type_FLOAT, const uint8_t &__Frame_Header = 0xab);

    inline int32_t Get_Variable_Index() const;

    inline float Get_Variable_Value() const;

    inline void Set_Data(const int &Number, ...);

    void USB_RxCallback(const uint8_t *Rx_Data, const uint16_t &Length);

    void TIM_1ms_Write_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的USB
    Struct_USB_Manage_Object *USB_Manage_Object;
    // 是否开启校验和
    Enum_Serialplot_Checksum_8 Checksum_8;
    // 接收指令字典的数量
    uint8_t Rx_Variable_Num;
    // 接收指令字典列表指针
    char **Rx_Variable_List;
    // 串口绘图数据类型
    Enum_Serialplot_Data_Type Tx_Data_Type;
    // 数据包头标
    uint8_t Frame_Header;

    // 常量

    // 内部变量

    // 发送缓冲区
    uint8_t Tx_Buffer[UART_BUFFER_SIZE];

    // 需要绘图的各个变量数据地址, 软件限制只有24个通道, 多了闪退
    const void *Data[24];
    // 当前发送的数据长度, 等价于新数据偏移量
    uint8_t Data_Number = 0;
    // 当前接收的指令在指令字典中的编号
    int32_t Variable_Index = 0;
    // 当前接收的指令在指令字典中的值
    float Variable_Value = 0.0f;

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数

    void Data_Process(const uint16_t &Length);

    uint8_t _Judge_Variable_Name(const uint16_t &Length);

    void _Judge_Variable_Value(const uint16_t &Length, int flag);

    void Output();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取当前接收的指令在指令字典中的编号
 *
 * @return int32_t 当前接收的指令在指令字典中的编号
 */
inline int32_t Class_Serialplot_UART::Get_Variable_Index() const
{
    return (Variable_Index);
}

/**
 * @brief 获取当前接收的指令在指令字典中的值
 *
 * @return float 当前接收的指令在指令字典中的值
 */
inline float Class_Serialplot_UART::Get_Variable_Value() const
{
    return (Variable_Value);
}

/**
 * @brief 添加被发送的数据
 *
 * @param Number 添加的数据数量
 * @param ... 每个数据的指针
 */
inline void Class_Serialplot_UART::Set_Data(const int &Number, ...)
{
    va_list data_ptr;
    va_start(data_ptr, Number);
    for (int i = 0; i < Number; i++)
    {
        Data[i] = (void *) va_arg(data_ptr, int);
    }
    va_end(data_ptr);
    Data_Number = Number;
}

/**
 * @brief 获取当前接收的指令在指令字典中的编号
 *
 * @return int32_t 当前接收的指令在指令字典中的编号
 */
inline int32_t Class_Serialplot_USB::Get_Variable_Index() const
{
    return (Variable_Index);
}

/**
 * @brief 获取当前接收的指令在指令字典中的值
 *
 * @return float 当前接收的指令在指令字典中的值
 */
inline float Class_Serialplot_USB::Get_Variable_Value() const
{
    return (Variable_Value);
}

/**
 * @brief 添加被发送的数据
 *
 * @param Number 添加的数据数量
 * @param ... 每个数据的指针
 */
inline void Class_Serialplot_USB::Set_Data(const int &Number, ...)
{
    va_list data_ptr;
    va_start(data_ptr, Number);
    for (int i = 0; i < Number; i++)
    {
        Data[i] = (void *) va_arg(data_ptr, int);
    }
    va_end(data_ptr);
    Data_Number = Number;
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/