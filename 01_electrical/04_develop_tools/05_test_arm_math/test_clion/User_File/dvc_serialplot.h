/**
 * @file dvc_motor.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 串口绘图
 * @version 0.1
 * @date 2022-08-07
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

#ifndef DVC_SERIALPLOT_H
#define DVC_SERIALPLOT_H

/* Includes ------------------------------------------------------------------*/

#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "drv_uart.h"

/* Exported macros -----------------------------------------------------------*/

//串口绘图单条指令最大长度
#define SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH (100)

/* Exported types ------------------------------------------------------------*/

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
 * @brief 串口绘图工具, 最多支持12个通道
 *
 */
class Class_Serialplot
{
public:
    void Init(UART_HandleTypeDef *huart, uint8_t __Serialplot_Rx_Variable_Assignment_Num = 0, char **__Serialplot_Rx_Variable_Assignment_List = NULL, Enum_Serialplot_Data_Type __Serialplot_Data_Type = Serialplot_Data_Type_FLOAT, uint8_t __Frame_Header = 0xab);

    int8_t Get_Variable_Index();
    double Get_Variable_Value();

    void Set_Data(uint8_t Number, ...);

    void UART_RxCpltCallback(uint8_t *Rx_Data);
    void TIM_Write_PeriodElapsedCallback();

protected:
    //初始化相关常量

    //绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;
    //接收指令字典的数量
    uint8_t UART_Rx_Variable_Num;
    //接收指令字典列表指针
    char **UART_Rx_Variable_List;
    //串口绘图数据类型
    Enum_Serialplot_Data_Type UART_Tx_Data_Type;
    //数据包头标
    uint8_t Frame_Header;

    //常量

    //内部变量

    //需要绘图的各个变量数据地址
    const void *Data[12];
    //当前发送的数据长度, 等价于新数据偏移量
    uint8_t Data_Number = 0;
    //当前接收的指令在指令字典中的编号
    int8_t Variable_Index = 0;
    //当前接收的指令在指令字典中的值
    float Variable_Value = 0.0f;

    //读变量

    //写变量

    //读写变量

    //内部函数

    uint8_t Judge_Variable_Name();
    void Judge_Variable_Value(int flag);
    void Output();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
