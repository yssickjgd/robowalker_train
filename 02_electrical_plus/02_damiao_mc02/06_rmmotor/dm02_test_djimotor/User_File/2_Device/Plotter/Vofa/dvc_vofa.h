/**
 * @file dvc_vofa.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief vofa
 * @version 0.1
 * @date 2025-09-22 0.1 新建
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

#ifndef DVC_VOFA_H
#define DVC_VOFA_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/Algorithm/Basic/alg_basic.h"
#include "1_Middleware/Driver/UART/drv_uart.h"
#include "1_Middleware/Driver/USB/drv_usb.h"
#include <stdarg.h>

/* Exported macros -----------------------------------------------------------*/

#define VOFA_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH (100)

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Specialized, vofa工具, justfloat模式
 *
 */
class Class_Vofa
{
public:
    void Init(const UART_HandleTypeDef *huart, const uint8_t &__Rx_Variable_Assignment_Num = 0, const char **__Rx_Variable_Assignment_List = NULL, const uint32_t &__Frame_Tail = 0x7f800000);

    inline int32_t Get_Variable_Index() const;

    inline float Get_Variable_Value() const;

    inline void Set_Data(const int &Number, ...);

    void UART_RxCpltCallback(const uint8_t *Rx_Data, const uint16_t &Length);

    void TIM_1ms_Write_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;
    // 接收指令字典的数量
    uint8_t Rx_Variable_Num;
    // 接收指令字典列表指针
    char **Rx_Variable_List;
    // 数据包尾标
    uint32_t Frame_Tail;

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
 * @brief Specialized, vofa工具, justfloat模式
 *
 */
class Class_Vofa_USB
{
public:
    void Init(const uint8_t &__Rx_Variable_Assignment_Num = 0, const char **__Rx_Variable_Assignment_List = NULL, const uint32_t &__Frame_Tail = 0x7f800000);

    inline int32_t Get_Variable_Index() const;

    inline float Get_Variable_Value() const;

    inline void Set_Data(const int &Number, ...);

    void USB_RxCallback(const uint8_t *Rx_Data, const uint16_t &Length);

    void TIM_1ms_Write_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的USB
    Struct_USB_Manage_Object *USB_Manage_Object;
    // 接收指令字典的数量
    uint8_t Rx_Variable_Num;
    // 接收指令字典列表指针
    char **Rx_Variable_List;
    // 数据包尾标
    uint32_t Frame_Tail;

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

extern Class_Vofa Vofa;
extern Class_Vofa_USB Vofa_USB;

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取当前接收的指令在指令字典中的编号
 *
 * @return int32_t 当前接收的指令在指令字典中的编号
 */
inline int32_t Class_Vofa::Get_Variable_Index() const
{
    return (Variable_Index);
}

/**
 * @brief 获取当前接收的指令在指令字典中的值
 *
 * @return float 当前接收的指令在指令字典中的值
 */
inline float Class_Vofa::Get_Variable_Value() const
{
    return (Variable_Value);
}

/**
 * @brief 添加被发送的数据
 *
 * @param Number 添加的数据数量
 * @param ... 每个数据的指针
 */
inline void Class_Vofa::Set_Data(const int &Number, ...)
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
inline int32_t Class_Vofa_USB::Get_Variable_Index() const
{
    return (Variable_Index);
}

/**
 * @brief 获取当前接收的指令在指令字典中的值
 *
 * @return float 当前接收的指令在指令字典中的值
 */
inline float Class_Vofa_USB::Get_Variable_Value() const
{
    return (Variable_Value);
}

/**
 * @brief 添加被发送的数据
 *
 * @param Number 添加的数据数量
 * @param ... 每个数据的指针
 */
inline void Class_Vofa_USB::Set_Data(const int &Number, ...)
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