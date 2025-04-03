#include <iostream>
#include <conio.h>
#include <windows.h>
#include "alg_fsm.h"

using namespace std;

char tmp_char;

/**
 * @brief 售货员状态类型
 *
 */
enum Enum_FSM_Salesman_Work_Status
{
    FSM_Salesman_Work_Status_REST = 0,
    FSM_Salesman_Work_Status_WAITING,
    FSM_Salesman_Work_Status_WORKING,
};

/**
 * @brief Specialized, 售货员有限状态机
 *
 */
class Class_FSM_Salesman_Work_Status : public Class_FSM
{
public:
    void TIM_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // 内部变量

    // 叫保安时间溢出阈值, 单位ms
    uint32_t Guard_Time_Threshold = 250;

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数
};

/**
 * @brief 售货员有限状态机状态转移策略
 *
 */
void Class_FSM_Salesman_Work_Status::TIM_Calculate_PeriodElapsedCallback()
{
    Status[Now_Status_Serial].Count_Time++;

    // 自己接着编写状态转移函数
    switch (Now_Status_Serial)
    {
    case (FSM_Salesman_Work_Status_REST):
    {
        // 休息状态

        if (tmp_char == 'b')
        {
            // 上班
            Set_Status(FSM_Salesman_Work_Status_WAITING);
            cout << "go to work" << endl;
        }

        break;
    }
    case (FSM_Salesman_Work_Status_WAITING):
    {
        // 等待工作状态

        if (tmp_char == 'e')
        {
            // 下班
            Set_Status(FSM_Salesman_Work_Status_REST);
            cout << "get off work" << endl;
        }
        else if (tmp_char == '1')
        {
            // 有客人来了
            Set_Status(FSM_Salesman_Work_Status_WORKING);
            cout << "guest arrived" << endl;
        }

        break;
    }
    case (FSM_Salesman_Work_Status_WORKING):
    {
        // 工作中状态

        if(Status[Now_Status_Serial].Count_Time > Guard_Time_Threshold)
        {
            // 超时, 保安来了
            Set_Status(FSM_Salesman_Work_Status_WAITING);
            cout << "guard arrived and kick off the guest" << endl;
        }
        else if (tmp_char == '0')
        {
            // 客人离开
            Set_Status(FSM_Salesman_Work_Status_WAITING);
            cout << "guest leave" << endl;
        }

        break;
    }
    }
}

int main()
{
    Class_FSM_Salesman_Work_Status Salesman;
    cout << "there is a salesman" << endl;
    while (1)
    {
        // 1ms延时, 但可能不太准确
        Sleep(1);

        // 非阻塞式捕获键盘输入
        if (_kbhit())
        {
            tmp_char = _getch();
        }

        // 状态机计算
        Salesman.TIM_Calculate_PeriodElapsedCallback();

        // 清空输入字符信息
        tmp_char = 0;
    }
}