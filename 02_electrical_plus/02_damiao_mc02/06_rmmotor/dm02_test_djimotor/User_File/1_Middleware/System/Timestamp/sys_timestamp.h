/**
 * @file sys_timestamp.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 时间计算相关支持库
 * @version 0.1
 * @date 2025-08-16 0.1 新建文档
 *
 * @copyright USTC-RoboWalker (c) 2025
 *
 */

/**
 * 要求: 使能并绑定一个定时器, 开外部中断, PSC分频到1MHz, ARR为3600000000
 * 保证arr计数器1us增一次, 1h触发一次外部中断
 */

#ifndef SYS_TIME_H
#define SYS_TIME_H

/* Includes ------------------------------------------------------------------*/

#include "tim.h"
#include "stm32h7xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Specialized, 系统时间戳
 *
 */
class Class_Timestamp
{
public:
    void Init(TIM_HandleTypeDef *htim);

    inline uint64_t Get_Current_Timestamp() const;

    inline float Get_Now_Second() const;

    inline float Get_Now_Millisecond() const;

    inline uint64_t Get_Now_Microsecond() const;

    void TIM_3600s_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    TIM_HandleTypeDef *TIM_Handler;

    // 常量

    // 内部变量

    // 定时器溢出计数, 一小时溢出一次
    uint32_t TIM_Overflow_Count = 0;

    // 写变量

    // 读写变量

    // 内部函数

    uint64_t Calculate_Timestamp() const;
};

/**
 * @brief Reusable, 时间对象, 可以为负数
 *
 */
class Class_Time
{
public:
    // 构造函数
    Class_Time()
    {
        Microsecond = 0;
    }

    explicit Class_Time(const int64_t &__Microsecond)
    {
        Microsecond = __Microsecond;
    }

    Class_Time(const int32_t &__Second, const int16_t &__Millisecond, const int16_t &__Microsecond)
    {
        Microsecond = (int64_t)(__Second) * 1000000LL + (int64_t)(__Millisecond) * 1000LL + (int64_t)(__Microsecond);
    }

    // 复制构造函数
    Class_Time(const Class_Time &Time)
    {
        Microsecond = Time.Microsecond;
    }

    // 移动构造函数
    Class_Time(Class_Time &&Time) noexcept
    {
        Microsecond = Time.Microsecond;
    }

    // 析构函数
    ~Class_Time() = default;

    // 复制赋值函数
    Class_Time &operator=(const Class_Time &Time)
    {
        if (this != &Time)
        {
            Microsecond = Time.Microsecond;
        }
        return (*this);
    }

    // 移动赋值函数
    Class_Time &operator=(Class_Time &&Time) noexcept
    {
        Microsecond = Time.Microsecond;
        return (*this);
    }

    inline Class_Time operator+(const Class_Time &Time) const
    {
        return (Class_Time(Microsecond + Time.Get_Microsecond()));
    }

    inline Class_Time operator-(const Class_Time &Time) const
    {
        return (Class_Time(Microsecond - Time.Get_Microsecond()));
    }

    inline Class_Time operator*(const float &value) const
    {
        return (Class_Time((int64_t)((float) (Microsecond) * value)));
    }

    inline friend Class_Time operator*(const float &value, const Class_Time &Time)
    {
        return (Class_Time((int64_t)((float) (Time.Get_Microsecond()) * value)));
    }

    inline Class_Time operator/(const float &value) const
    {
        return (Class_Time((int64_t)((float) (Microsecond) / value)));
    }

    inline Class_Time &operator+=(const Class_Time &Time)
    {
        Microsecond += Time.Get_Microsecond();
        return (*this);
    }

    inline Class_Time &operator-=(const Class_Time &Time)
    {
        Microsecond -= Time.Get_Microsecond();
        return (*this);
    }

    inline Class_Time &operator*=(const float &value)
    {
        Microsecond = (int64_t)((float) (Microsecond) * value);
        return (*this);
    }

    inline Class_Time &operator/=(const float &value)
    {
        Microsecond = (int64_t)((float) (Microsecond) / value);
        return (*this);
    }

    inline bool operator>(const Class_Time &Time) const
    {
        return (Microsecond > Time.Get_Microsecond());
    }

    inline bool operator<(const Class_Time &Time) const
    {
        return (Microsecond < Time.Get_Microsecond());
    }

    inline bool operator>=(const Class_Time &Time) const
    {
        return (Microsecond >= Time.Get_Microsecond());
    }

    inline bool operator<=(const Class_Time &Time) const
    {
        return (Microsecond <= Time.Get_Microsecond());
    }

    inline bool operator==(const Class_Time &Time) const
    {
        return (Microsecond == Time.Get_Microsecond());
    }

    inline bool operator!=(const Class_Time &Time) const
    {
        return (Microsecond != Time.Get_Microsecond());
    }

    inline float Get_Second() const;

    inline float Get_Millisecond() const;

    inline int64_t Get_Microsecond() const;

protected:
    // 初始化相关常量

    // 常量

    // 内部变量

    // 读变量

    // 微秒
    int64_t Microsecond = 0;

    // 写变量

    // 读写变量

    // 内部函数
};

/* Exported variables --------------------------------------------------------*/

extern Class_Timestamp SYS_Timestamp;

namespace Namespace_SYS_Timestamp
{
    void Delay_Second(const uint32_t &Second);

    void Delay_Millisecond(const uint32_t &Millisecond);

    void Delay_Microsecond(const uint32_t &Microsecond);
}

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取当前时间
 *
 * @return uint64_t 当前时间
 */
inline uint64_t Class_Timestamp::Get_Current_Timestamp() const
{
    return (Calculate_Timestamp());
}

/**
 * @brief 获取当前时间, 单位秒
 *
 * @return float 当前时间, 单位秒
 */
inline float Class_Timestamp::Get_Now_Second() const
{
    return ((float) (Calculate_Timestamp()) / 1000000.0f);
}

/**
 * @brief 获取当前时间, 单位毫秒
 *
 * @return float 当前时间, 单位毫秒
 */
inline float Class_Timestamp::Get_Now_Millisecond() const
{
    return ((float) (Calculate_Timestamp()) / 1000.0f);
}

/**
 * @brief 获取当前时间, 单位微秒
 *
 * @return uint64_t 当前时间, 单位微秒
 */
inline uint64_t Class_Timestamp::Get_Now_Microsecond() const
{
    return (Calculate_Timestamp());
}

/**
 * @brief 获取时间, 单位秒
 *
 * @return float 时间, 单位秒
 */
inline float Class_Time::Get_Second() const
{
    return ((float) Microsecond / 1000000.0f);
}

/**
 * @brief 获取时间, 单位毫秒
 *
 * @return float 时间, 单位毫秒
 */
inline float Class_Time::Get_Millisecond() const
{
    return ((float) Microsecond / 1000.0f);
}

/**
 * @brief 获取时间, 单位微秒
 *
 * @return int64_t 时间, 单位微秒
 */
inline int64_t Class_Time::Get_Microsecond() const
{
    return (Microsecond);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/