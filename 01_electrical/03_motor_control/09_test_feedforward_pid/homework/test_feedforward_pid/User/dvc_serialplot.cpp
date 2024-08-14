/**
 * @file dvc_motor.cpp
 * @author yssickjgd (yssickjgd@mail.ustc.edu.cn)
 * @brief 串口绘图
 * @version 0.1
 * @date 2022-08-07
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_serialplot.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 串口绘图初始化
 *
 * @param __huart 指定的UART
 * @param __Serialplot_Rx_Variable_Assignment_Num 接收指令字典的数量
 * @param __Serialplot_Rx_Variable_Assignment_List 接收指令字典列表
 * @param __Serialplot_Tx_Data_Type 传输数据类型, 默认float
 * @param __Frame_Header 帧头标识符
 */
void Class_Serialplot::Init(UART_HandleTypeDef *huart, uint8_t __Serialplot_Rx_Variable_Assignment_Num, char **__Serialplot_Rx_Variable_Assignment_List, Enum_Serialplot_Data_Type __Serialplot_Tx_Data_Type, uint8_t __Frame_Header)
{
    if (huart->Instance == USART1)
    {
        UART_Manage_Object = &UART1_Manage_Object;
    }
    else if (huart->Instance == USART2)
    {
        UART_Manage_Object = &UART2_Manage_Object;
    }
    else if (huart->Instance == USART3)
    {
        UART_Manage_Object = &UART3_Manage_Object;
    }
    else if (huart->Instance == UART4)
    {
        UART_Manage_Object = &UART4_Manage_Object;
    }
    else if (huart->Instance == UART5)
    {
        UART_Manage_Object = &UART5_Manage_Object;
    }
    else if (huart->Instance == USART6)
    {
        UART_Manage_Object = &UART6_Manage_Object;
    }
    else if (huart->Instance == UART7)
    {
        UART_Manage_Object = &UART7_Manage_Object;
    }
    else if (huart->Instance == UART8)
    {
        UART_Manage_Object = &UART8_Manage_Object;
    }

    UART_Rx_Variable_Num = __Serialplot_Rx_Variable_Assignment_Num;
    UART_Rx_Variable_List = __Serialplot_Rx_Variable_Assignment_List;
    UART_Tx_Data_Type = __Serialplot_Tx_Data_Type;
    Frame_Header = __Frame_Header;

    UART_Manage_Object->Tx_Buffer[0] = __Frame_Header;
}

/**
 * @brief 判断指令变量名
 *
 * @return uint8_t 指令数值位置的指针, 也就是"variable=value#"中v的坐标
 */
uint8_t Class_Serialplot::Judge_Variable_Name()
{
    //临时存储变量名
    char tmp_variable_name[SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH];
    //等号位置标记
    int flag;

    //记录变量名并标记等号位置
    for (flag = 0; UART_Manage_Object->Rx_Buffer[flag] != '='; flag++)
    {
        tmp_variable_name[flag] = UART_Manage_Object->Rx_Buffer[flag];
    }
    tmp_variable_name[flag] = 0;

    //比对是否在列表中
    for (int i = 0; i < UART_Rx_Variable_Num; i++)
    {
        //如果在则标记变量名编号
        if (strcmp(tmp_variable_name, (char *)((int)UART_Rx_Variable_List + SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH * i)) == 0)
        {
            Variable_Index = i;
            return (flag + 1);
        }
    }
    //如果变量名不在则-1
    Variable_Index = -1;
    return (flag + 1);
}

/**
 * @brief 判断指令变量数值
 *
 */
void Class_Serialplot::Judge_Variable_Value(int flag)
{
    //小数点位置, 是否有负号
    int tmp_dot_flag, tmp_sign_coefficient, i;

    tmp_dot_flag = 0;
    tmp_sign_coefficient = 1;
    Variable_Value = 0.0f;

    //列表里没有, 没必要比对直接返回
    if (Variable_Index == -1)
    {
        return;
    }

    //判断是否有负号
    if (UART_Manage_Object->Rx_Buffer[flag] == '-')
    {
        tmp_sign_coefficient = -1;
        flag++;
    }

    //计算值并注意小数点是否存在及其位置
    for (i = flag; UART_Manage_Object->Rx_Buffer[i] != '#' && UART_Manage_Object->Rx_Buffer[i] != 0; i++)
    {
        if (UART_Manage_Object->Rx_Buffer[i] == '.')
        {
            tmp_dot_flag = i;
        }
        else
        {
            Variable_Value = Variable_Value * 10.0f + (UART_Manage_Object->Rx_Buffer[i] - '0');
        }
    }

    //如果有小数点则考虑
    if (tmp_dot_flag != 0)
    {
        Variable_Value /= pow(10.0f, i - tmp_dot_flag - 1.0f);
    }

    Variable_Value *= tmp_sign_coefficient;
}

/**
 * @brief 串口绘图数据输出到UART发送缓冲区
 *
 */
void Class_Serialplot::Output()
{
    for (int i = 0; i < 256; i++)
    {
        UART_Manage_Object->Tx_Buffer[i] = 0;
    }

    //放置帧头
    UART_Manage_Object->Tx_Buffer[0] = Frame_Header;
    
    //填充数据
    if (UART_Tx_Data_Type == Serialplot_Data_Type_UINT8 || UART_Tx_Data_Type == Serialplot_Data_Type_INT8)
    {
        for (int i = 0; i < Data_Number; i++)
        {
            memcpy(UART_Manage_Object->Tx_Buffer + i * sizeof(uint8_t) + 1, Data[i], sizeof(uint8_t));
        }
    }
    else if (UART_Tx_Data_Type == Serialplot_Data_Type_UINT16 || UART_Tx_Data_Type == Serialplot_Data_Type_INT16)
    {
        for (int i = 0; i < Data_Number; i++)
        {
            memcpy(UART_Manage_Object->Tx_Buffer + i * sizeof(uint16_t) + 1, Data[i], sizeof(uint16_t));
        }
    }
    else if (UART_Tx_Data_Type == Serialplot_Data_Type_UINT32 || UART_Tx_Data_Type == Serialplot_Data_Type_INT32 || UART_Tx_Data_Type == Serialplot_Data_Type_FLOAT)
    {
        for (int i = 0; i < Data_Number; i++)
        {
            memcpy(UART_Manage_Object->Tx_Buffer + i * sizeof(uint32_t) + 1, Data[i], sizeof(uint32_t));
        }
    }
    else if (UART_Tx_Data_Type == Serialplot_Data_Type_DOUBLE)
    {
        for (int i = 0; i < Data_Number; i++)
        {
            memcpy(UART_Manage_Object->Tx_Buffer + i * sizeof(uint64_t) + 1, Data[i], sizeof(uint64_t));
        }
    }
}

/**
 * @brief 获取当前接收的指令在指令字典中的编号
 *
 * @return int8_t 当前接收的指令在指令字典中的编号
 */
int8_t Class_Serialplot::Get_Variable_Index()
{
    return (Variable_Index);
}

/**
 * @brief 获取当前接收的指令在指令字典中的值
 *
 * @return double 当前接收的指令在指令字典中的值
 */
double Class_Serialplot::Get_Variable_Value()
{
    return (Variable_Value);
}

/**
 * @brief 添加被发送的数据
 *
 * @param Number 添加的数据数量
 * @param ... 每个数据的指针, 至多12个
 */
void Class_Serialplot::Set_Data(uint8_t Number, ...)
{
    va_list data_ptr;
    va_start(data_ptr, Number);
    for (int i = 0; i < Number; i++)
    {
        Data[i] = (void *)va_arg(data_ptr, int);
    }
    va_end(data_ptr);
    Data_Number = Number;
}

/**
 * @brief UART通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Serialplot::UART_RxCpltCallback(uint8_t *Rx_Data)
{
    int flag;
    flag = Judge_Variable_Name();
    Judge_Variable_Value(flag);
}

/**
 * @brief TIM定时器中断增加数据到发送缓冲区
 *
 */
void Class_Serialplot::TIM_Write_PeriodElapsedCallback()
{
    Output();
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
