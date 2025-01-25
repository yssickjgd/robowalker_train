/**
 * @file DR16.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 遥控器DR16
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

#ifndef DVC_DR16_H
#define DVC_DR16_H

/* Includes ------------------------------------------------------------------*/

#include "1_Middleware/1_Driver/UART/drv_uart.h"

/* Exported macros -----------------------------------------------------------*/

// 拨动开关位置
#define DR16_SWITCH_UP (1)
#define DR16_SWITCH_DOWN (2)
#define DR16_SWITCH_MIDDLE (3)

// 按键开关位置
#define DR16_KEY_FREE (0)
#define DR16_KEY_PRESSED (1)

// 键位宏定义
#define DR16_KEY_W 0
#define DR16_KEY_S 1
#define DR16_KEY_A 2
#define DR16_KEY_D 3
#define DR16_KEY_SHIFT 4
#define DR16_KEY_CTRL 5
#define DR16_KEY_Q 6
#define DR16_KEY_E 7
#define DR16_KEY_R 8
#define DR16_KEY_F 9
#define DR16_KEY_G 10
#define DR16_KEY_Z 11
#define DR16_KEY_X 12
#define DR16_KEY_C 13
#define DR16_KEY_V 14
#define DR16_KEY_B 15

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 遥控器DR16状态
 *
 */
enum Enum_DR16_Status
{
    DR16_Status_DISABLE = 0,
    DR16_Status_ENABLE,
};

/**
 * @brief 拨动开关状态
 *
 */
enum Enum_DR16_Switch_Status
{
    DR16_Switch_Status_UP = 0,
    DR16_Switch_Status_TRIG_UP_MIDDLE,
    DR16_Switch_Status_TRIG_MIDDLE_UP,
    DR16_Switch_Status_MIDDLE,
    DR16_Switch_Status_TRIG_MIDDLE_DOWN,
    DR16_Switch_Status_TRIG_DOWN_MIDDLE,
    DR16_Switch_Status_DOWN,
};

/**
 * @brief 按键状态
 *
 */
enum Enum_DR16_Key_Status
{
    DR16_Key_Status_FREE = 0,
    DR16_Key_Status_TRIG_FREE_PRESSED,
    DR16_Key_Status_TRIG_PRESSED_FREE,
    DR16_Key_Status_PRESSED,
};

/**
 * @brief DR16源数据
 *
 */
struct Struct_DR16_UART_Data
{
    uint64_t Channel_0 : 11;
    uint64_t Channel_1 : 11;
    uint64_t Channel_2 : 11;
    uint64_t Channel_3 : 11;
    uint64_t Switch_2 : 2;
    uint64_t Switch_1 : 2;
    int16_t Mouse_X;
    int16_t Mouse_Y;
    int16_t Mouse_Z;
    uint64_t Mouse_Left_Key : 8;
    uint64_t Mouse_Right_Key : 8;
    uint64_t Keyboard_Key : 16;
    uint64_t Channel_Yaw : 11;
} __attribute__((packed));

/**
 * @brief DR16经过处理的的数据, 摇杆信息经过归一化到-1~1, 鼠标信息有待进一步标定
 *
 */
struct Struct_DR16_Data
{
    float Right_X;
    float Right_Y;
    float Left_X;
    float Left_Y;
    Enum_DR16_Switch_Status Left_Switch;
    Enum_DR16_Switch_Status Right_Switch;
    float Mouse_X;
    float Mouse_Y;
    float Mouse_Z;
    Enum_DR16_Key_Status Mouse_Left_Key;
    Enum_DR16_Key_Status Mouse_Right_Key;
    Enum_DR16_Key_Status Keyboard_Key[16];
    float Yaw;
};

/**
 * @brief Specialized, 遥控器DR16
 *
 */
class Class_DR16
{
public:
    void Init(UART_HandleTypeDef *huart);

    inline Enum_DR16_Status Get_Status();

    inline float Get_Right_X();

    inline float Get_Right_Y();

    inline float Get_Left_X();

    inline float Get_Left_Y();

    inline Enum_DR16_Switch_Status Get_Left_Switch();

    inline Enum_DR16_Switch_Status Get_Right_Switch();

    inline float Get_Mouse_X();

    inline float Get_Mouse_Y();

    inline float Get_Mouse_Z();

    inline Enum_DR16_Key_Status Get_Mouse_Left_Key();

    inline Enum_DR16_Key_Status Get_Mouse_Right_Key();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_W();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_S();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_A();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_D();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_Shift();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_Ctrl();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_Q();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_E();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_R();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_F();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_G();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_Z();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_X();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_C();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_V();

    inline Enum_DR16_Key_Status Get_Keyboard_Key_B();

    inline float Get_Yaw();

    void UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length);

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_1ms_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;

    // 常量

    // 摇杆偏移量
    float Rocker_Offset = 1024.0f;
    // 摇杆总刻度
    float Rocker_Num = 660.0f;

    // 内部变量

    // 前一时刻的遥控器DR16状态信息
    Struct_DR16_UART_Data Pre_UART_Rx_Data;
    // 当前时刻的遥控器DR16接收flag
    uint32_t Flag = 0;
    // 前一时刻的遥控器DR16接收flag
    uint32_t Pre_Flag = 0;

    // 读变量

    // 遥控器DR16状态
    Enum_DR16_Status DR16_Status = DR16_Status_DISABLE;
    // 遥控器DR16对外接口信息
    Struct_DR16_Data Data;

    // 写变量

    // 读写变量

    // 内部函数

    void Data_Process(uint16_t Length);

    void _Judge_Switch(Enum_DR16_Switch_Status *Switch, uint8_t Status, uint8_t Pre_Status);

    void _Judge_Key(Enum_DR16_Key_Status *Key, uint8_t Status, uint8_t Pre_Status);
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取遥控器DR16在线状态
 *
 * @return Enum_DR16_Status 遥控器DR16在线状态
 */
inline Enum_DR16_Status Class_DR16::Get_Status()
{
    return (DR16_Status);
}

/**
 * @brief 获取遥控器DR16右侧x轴摇杆状态
 *
 * @return float 遥控器DR16右侧x轴摇杆状态
 */
inline float Class_DR16::Get_Right_X()
{
    return (Data.Right_X);
}

/**
 * @brief 获取遥控器DR16右侧y轴摇杆状态
 *
 * @return float 遥控器DR16右侧y轴摇杆状态
 */
inline float Class_DR16::Get_Right_Y()
{
    return (Data.Right_Y);
}

/**
 * @brief 获取遥控器DR16左侧x轴摇杆状态
 *
 * @return float 遥控器DR16左侧x轴摇杆状态
 */
inline float Class_DR16::Get_Left_X()
{
    return (Data.Left_X);
}

/**
 * @brief 获取遥控器DR16左侧y轴摇杆状态
 *
 * @return float 遥控器DR16左侧y轴摇杆状态
 */
inline float Class_DR16::Get_Left_Y()
{
    return (Data.Left_Y);
}

/**
 * @brief 获取遥控器DR16左侧拨动开关状态
 *
 * @return Enum_DR16_Switch_Status 遥控器DR16左侧拨动开关状态
 */
inline Enum_DR16_Switch_Status Class_DR16::Get_Left_Switch()
{
    return (Data.Left_Switch);
}

/**
 * @brief 获取遥控器DR16右侧拨动开关状态
 *
 * @return Enum_DR16_Switch_Status 遥控器DR16右侧拨动开关状态
 */
inline Enum_DR16_Switch_Status Class_DR16::Get_Right_Switch()
{
    return (Data.Right_Switch);
}

/**
 * @brief 获取鼠标x轴状态
 *
 * @return float 鼠标x轴状态
 */
inline float Class_DR16::Get_Mouse_X()
{
    return (Data.Mouse_X);
}

/**
 * @brief 获取鼠标y轴状态
 *
 * @return float 鼠标y轴状态
 */
inline float Class_DR16::Get_Mouse_Y()
{
    return (Data.Mouse_Y);
}

/**
 * @brief 获取鼠标z轴状态
 *
 * @return float 鼠标z轴状态
 */
inline float Class_DR16::Get_Mouse_Z()
{
    return (Data.Mouse_Z);
}

/**
 * @brief 获取鼠标左键状态
 *
 * @return Enum_DR16_Key_Status 鼠标左键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Mouse_Left_Key()
{
    return (Data.Mouse_Left_Key);
}

/**
 * @brief 获取鼠标右键状态
 *
 * @return Enum_DR16_Key_Status 鼠标右键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Mouse_Right_Key()
{
    return (Data.Mouse_Right_Key);
}

/**
 * @brief 获取键盘W键状态
 *
 * @return Enum_DR16_Key_Status 键盘W键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_W()
{
    return (Data.Keyboard_Key[DR16_KEY_W]);
}

/**
 * @brief 获取键盘S键状态
 *
 * @return Enum_DR16_Key_Status 键盘S键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_S()
{
    return (Data.Keyboard_Key[DR16_KEY_S]);
}

/**
 * @brief 获取键盘A键状态
 *
 * @return Enum_DR16_Key_Status 键盘A键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_A()
{
    return (Data.Keyboard_Key[DR16_KEY_A]);
}

/**
 * @brief 获取键盘D键状态
 *
 * @return Enum_DR16_Key_Status 键盘D键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_D()
{
    return (Data.Keyboard_Key[DR16_KEY_D]);
}

/**
 * @brief 获取键盘Shift键状态
 *
 * @return Enum_DR16_Key_Status 键盘Shift键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_Shift()
{
    return (Data.Keyboard_Key[DR16_KEY_SHIFT]);
}

/**
 * @brief 获取键盘Ctrl键状态
 *
 * @return Enum_DR16_Key_Status 键盘Ctrl键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_Ctrl()
{
    return (Data.Keyboard_Key[DR16_KEY_CTRL]);
}

/**
 * @brief 获取键盘Q键状态
 *
 * @return Enum_DR16_Key_Status 键盘Q键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_Q()
{
    return (Data.Keyboard_Key[DR16_KEY_Q]);
}

/**
 * @brief 获取键盘E键状态
 *
 * @return Enum_DR16_Key_Status 键盘E键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_E()
{
    return (Data.Keyboard_Key[DR16_KEY_E]);
}

/**
 * @brief 获取键盘R键状态
 *
 * @return Enum_DR16_Key_Status 键盘R键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_R()
{
    return (Data.Keyboard_Key[DR16_KEY_R]);
}

/**
 * @brief 获取键盘F键状态
 *
 * @return Enum_DR16_Key_Status 键盘F键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_F()
{
    return (Data.Keyboard_Key[DR16_KEY_F]);
}

/**
 * @brief 获取键盘G键状态
 *
 * @return Enum_DR16_Key_Status 键盘G键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_G()
{
    return (Data.Keyboard_Key[DR16_KEY_G]);
}

/**
 * @brief 获取键盘Z键状态
 *
 * @return Enum_DR16_Key_Status 键盘Z键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_Z()
{
    return (Data.Keyboard_Key[DR16_KEY_Z]);
}

/**
 * @brief 获取键盘X键状态
 *
 * @return Enum_DR16_Key_Status 键盘X键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_X()
{
    return (Data.Keyboard_Key[DR16_KEY_X]);
}

/**
 * @brief 获取键盘C键状态
 *
 * @return Enum_DR16_Key_Status 键盘C键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_C()
{
    return (Data.Keyboard_Key[DR16_KEY_C]);
}

/**
 * @brief 获取键盘V键状态
 *
 * @return Enum_DR16_Key_Status 键盘V键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_V()
{
    return (Data.Keyboard_Key[DR16_KEY_V]);
}

/**
 * @brief 获取键盘B键状态
 *
 * @return Enum_DR16_Key_Status 键盘B键状态
 */
inline Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_B()
{
    return (Data.Keyboard_Key[DR16_KEY_B]);
}

/**
 * @brief 获取遥控器DR16yaw轴状态
 *
 * @return float 遥控器DR16yaw轴状态
 */
inline float Class_DR16::Get_Yaw()
{
    return (Data.Yaw);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
