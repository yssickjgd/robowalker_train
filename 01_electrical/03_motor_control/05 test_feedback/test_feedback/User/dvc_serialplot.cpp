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
void Class_Serialplot::Init(UART_HandleTypeDef *__huart, uint8_t __Serialplot_Rx_Variable_Assignment_Num, char **__Serialplot_Rx_Variable_Assignment_List, Enum_Serialplot_Data_Type __Serialplot_Tx_Data_Type, uint8_t __Frame_Header)
{
    huart = __huart;
    if (huart->Instance == USART1)
    {
        UART_Rx_Data = UART1_Manage_Object.Rx_Buffer;
        UART_Tx_Data = UART1_Tx_Data;
    }
    else if (huart->Instance == USART2)
    {
        UART_Rx_Data = UART2_Manage_Object.Rx_Buffer;
        UART_Tx_Data = UART2_Tx_Data;
    }
    else if (huart->Instance == USART3)
    {
        UART_Rx_Data = UART3_Manage_Object.Rx_Buffer;
        UART_Tx_Data = UART3_Tx_Data;
    }
    else if (huart->Instance == UART4)
    {
        UART_Rx_Data = UART4_Manage_Object.Rx_Buffer;
        UART_Tx_Data = UART4_Tx_Data;
    }
    else if (huart->Instance == UART5)
    {
        UART_Rx_Data = UART5_Manage_Object.Rx_Buffer;
        UART_Tx_Data = UART5_Tx_Data;
    }
    else if (huart->Instance == USART6)
    {
        UART_Rx_Data = UART6_Manage_Object.Rx_Buffer;
        UART_Tx_Data = UART6_Tx_Data;
    }
    else if (huart->Instance == UART7)
    {
        UART_Rx_Data = UART7_Manage_Object.Rx_Buffer;
        UART_Tx_Data = UART7_Tx_Data;
    }
    else if (huart->Instance == UART8)
    {
        UART_Rx_Data = UART8_Manage_Object.Rx_Buffer;
        UART_Tx_Data = UART8_Tx_Data;
    }
    UART_Rx_Variable_Num = __Serialplot_Rx_Variable_Assignment_Num;
    UART_Rx_Variable_List = __Serialplot_Rx_Variable_Assignment_List;
    UART_Tx_Data_Type = __Serialplot_Tx_Data_Type;
    Frame_Header = __Frame_Header;

    UART_Tx_Data[0] = __Frame_Header;
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
 * @brief 判断指令变量名
 *
 * @return uint8_t 指令数值位置的指针, 也就是"variable=value#"中v的坐标
 */
uint8_t Class_Serialplot::Judge_Variable_Name()
{
    char tmp_variable_name[SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH];
    int flag;
    for (flag = 0; UART_Rx_Data[flag] != '='; flag++)
    {
        tmp_variable_name[flag] = UART_Rx_Data[flag];
    }
    tmp_variable_name[flag] = 0;
    for (int i = 0; i < UART_Rx_Variable_Num; i++)
    {
        if (strcmp(tmp_variable_name, (char *)((int)UART_Rx_Variable_List + SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH * i)) == 0)
        {
            Variable_Index = i;
            return (flag + 1);
        }
    }
    Variable_Index = -1;
    return (flag + 1);
}

/**
 * @brief 判断指令变量数值
 *
 */
void Class_Serialplot::Judge_Variable_Value(int flag)
{
    int tmp_dot_flag, tmp_sign_coefficient, i;
    tmp_dot_flag = 0;
    tmp_sign_coefficient = 1;
    Variable_Value = 0;

    if (Variable_Index == -1)
    {
        return;
    }

    if (UART_Rx_Data[flag] == '-')
    {
        tmp_sign_coefficient = -1;
        flag++;
    }

    for (i = flag; UART_Rx_Data[i] != '#' && UART_Rx_Data[i] != 0; i++)
    {
        if (UART_Rx_Data[i] == '.')
        {
            tmp_dot_flag = i;
        }
        else
        {
            Variable_Value = Variable_Value * 10 + (UART_Rx_Data[i] - '0');
        }
    }

    if (tmp_dot_flag != 0)
    {
        Variable_Value /= pow(10.0f, i - tmp_dot_flag - 1);
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
        UART_Tx_Data[i] = 0;
    }
    UART_Tx_Data[0] = Frame_Header;
    if (UART_Tx_Data_Type == Serialplot_Data_Type_UINT8 || UART_Tx_Data_Type == Serialplot_Data_Type_INT8)
    {
        for (int i = 0; i < Data_Number; i++)
        {
            UART_Tx_Data[1 + i + 0] = *((char *)((int)Data[i] + 0));
        }
    }
    else if (UART_Tx_Data_Type == Serialplot_Data_Type_UINT16 || UART_Tx_Data_Type == Serialplot_Data_Type_INT16)
    {
        for (int i = 0; i < Data_Number; i++)
        {
            UART_Tx_Data[1 + 2 * i + 0] = *((char *)(int)Data[i] + 0);
            UART_Tx_Data[1 + 2 * i + 1] = *((char *)(int)Data[i] + 1);
        }
    }
    else if (UART_Tx_Data_Type == Serialplot_Data_Type_UINT32 || UART_Tx_Data_Type == Serialplot_Data_Type_INT32 || UART_Tx_Data_Type == Serialplot_Data_Type_FLOAT)
    {
        for (int i = 0; i < Data_Number; i++)
        {
            UART_Tx_Data[1 + 4 * i + 0] = *((char *)(int)Data[i] + 0);
            UART_Tx_Data[1 + 4 * i + 1] = *((char *)(int)Data[i] + 1);
            UART_Tx_Data[1 + 4 * i + 2] = *((char *)(int)Data[i] + 2);
            UART_Tx_Data[1 + 4 * i + 3] = *((char *)(int)Data[i] + 3);
        }
    }
    else if (UART_Tx_Data_Type == Serialplot_Data_Type_DOUBLE)
    {
        for (int i = 0; i < Data_Number; i++)
        {
            UART_Tx_Data[1 + 8 * i + 0] = *((char *)(int)Data[i] + 0);
            UART_Tx_Data[1 + 8 * i + 1] = *((char *)(int)Data[i] + 1);
            UART_Tx_Data[1 + 8 * i + 2] = *((char *)(int)Data[i] + 2);
            UART_Tx_Data[1 + 8 * i + 3] = *((char *)(int)Data[i] + 3);
            UART_Tx_Data[1 + 8 * i + 4] = *((char *)(int)Data[i] + 4);
            UART_Tx_Data[1 + 8 * i + 5] = *((char *)(int)Data[i] + 5);
            UART_Tx_Data[1 + 8 * i + 6] = *((char *)(int)Data[i] + 6);
            UART_Tx_Data[1 + 8 * i + 7] = *((char *)(int)Data[i] + 7);
        }
    }
}

/**
 * @brief TIM定时器中断增加数据写入回调函数
 *
 */
void Class_Serialplot::TIM_Add_PeriodElapsedCallback()
{
    Output();
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
/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
