# 教程文档

**省流版**

>   配套教学视频
>
>   https://space.bilibili.com/337732684/channel/collectiondetail?sid=1043942
>
>   环境配置安装包
>
>   http://home.ustc.edu.cn/~yssickjgd
>
>   配套代码与其他相关资料 ( 内容完全一致, 根据网络状况酌情下载 )
>
>   https://git.lug.ustc.edu.cn/yssickjgd/robowalker_train
>
>   https://github.com/yssickjgd/robowalker_train
>
>   配套PPT资料
>
>   https://rec.ustc.edu.cn/share/c8b3bbc0-32bc-11ee-98d2-2330cf7972b8

## 1. 教程目录

标黄为已更新内容

1.  电控 ( electrical )
    1.  ==电控简介 ( electrical_introduction )==
        1.  ==电控导论==
        2.  ==番外篇 如何去提问==
    2.  ==STM32CubeMX基础 ( stm32cubemx_basic )==
        1.  ==环境配置与基础教学==
        2.  ==GPIO输出电压==
        3.  ==EXTI外部中断==
        4.  ==TIM定时器中断==
            -   ==作业==
        5.  ==UART通信==
        6.  ==PWM呼吸灯==
        7.  ==ADC测电压==
    3.  ==电机控制 ( motor_control )==
        1.  ==C++==
            -   ==作业==
        2.  ==CAN通信==
        3.  ==CAN接口与大疆RM电机初步控制==
        4.  ==UART接口与串口绘图==
        5.  ==串口绘图接口与电机反馈==
        6.  ==PID算法==
        7.  ==PID接口与电机闭环控制==
        8.  ==电机接口与多环PID==
        9.  ==动态目标与前馈PID==
            - ==作业==
        10.  ==多电机控制==
        11.  ==番外篇 达妙电机控制==
        12.  ==番外篇 大疆电机进阶控制与相关库的累积更新==
    4.  ==开发工具篇 ( develop_tools )==
        1.  ==Markdown基础教学==
        2.  ==Git基础操作教学==
        3.  ==CLion开发环境配置==
        4.  ==gcc, Make与CMake==
        6.  ==番外篇 数学加速库arm_math.h的引入==
        6.  ==番外篇 开发相关的官方软件与官方文档获取==
        7.  ==番外篇 如何Debug==
    5.  底盘篇 ( chassis )
        1.  ==DR16与DT7遥控器模块==
        3.  ==底盘概述==
        4.  ==底盘的运动学与动力学解算==
        4.  ==欧拉角与旋转矩阵==
        5.  ==力补偿适配方法==
        6.  ==功率控制前瞻==
        7.  ==电机功率模型==
        8.  ==功率控制策略==
        9.  算法层的斜坡规划器
        10.  全向轮麦轮底盘适配算法
        11.  舵轮底盘适配算法
        12.  驱动层的看门狗
        13.  速度规划、小陀螺与操作逻辑
        14.  总结与致谢
    6.  云台篇 ( gimbal )
        1.  陀螺仪简介
        2.  云台概述
        3.  结合成品陀螺仪的云台控制
        4.  操作逻辑
        5.  总结与致谢
    7.  发射机构篇 ( booster )
        1.  算法层的有限自动机
        2.  卡弹处理算法
        3.  无裁判系统的热量检测与控制算法
        4.  操作逻辑
        5.  总结与致谢
    8.  滤波 ( filter )
        1.  Fourier变换
        2.  基于频率的滤波器
        3.  基于统计的Kalman滤波器
    9.  陀螺仪 ( gyroscope )
        1.  驱动接口层的SPI通信
        2.  SPI接口与陀螺仪读写
        3.  欧拉角与四元数
        4.  六轴陀螺仪姿态解算
    10.  PCB电路板设计教程 ( pcb_design )
         1.  常见线材接口简介
         2.  常见电路元件简介
         3.  嘉立创EDA基本操作教学
         4.  简易分电板
         5.  简易发光充能装置板
         6.  超级电容控制理论
         7.  超级电容开源电路板
         8.  超级电容开源代码
2.  电控眼里的比赛 ( referee )
    1.  官方软件的获取与使用
    2.  比赛通用规则解读
    3.  裁判系统接线规则
    4.  裁判系统协议
    5.  裁判系统服务器搭建
3.  电控与机械组 ( mechanical )
    1.  机械组工作内容
    2.  Solidworks建模基本方法
    3.  Solidworks导出加工图
    4.  常见的设计技巧
    5.  常见的加工工艺
    6.  兵种开源图纸
4.  电控与视觉算法组 ( vision algorithm )
    1.  视觉算法组工作内容
    3.  自瞄基本策略
    4.  能量机关基本策略
    5.  驱动接口层的USB虚拟串口
5.  宣传运营相关内容 ( operation )
    1.  从零开始建立一个战队
    2.  交流平台汇总
    3.  官方资料的获取渠道
    4.  战队资源的获取渠道
    5.  赛季重要时间点
    6.  平面设计之Ps与Ai
    7.  视频剪辑之Pr
    8.  直播推流
    9.  财务

## 2. 教程必要说明

-   前置知识需要, 且仅需要**大学数学 ( 微积分, 线性代数, 概率论与数理统计 ) , C语言**
-   **配套教学视频**链接
    -   https://space.bilibili.com/337732684/channel/collectiondetail?sid=1043942
    -   由于讲义会不定期更新或修正内容, 因此B站视频教学可供参考, 具体章节目录请依照最新版讲义内容
    -   对于必要的内容修订与增添, B站视频也会适当修改
-   **环境配置安装包**在本人的中国科大FTP个人主页
    -   http://home.ustc.edu.cn/~yssickjgd
    -   进入该网站后点击"机器人社团/战队电控环境配置"中, 即可找到

-   **配套代码与其他相关资料**一式二份在Github与中国科大Linux用户协会的Gitlab
    -   https://git.lug.ustc.edu.cn/yssickjgd/robowalker_train
    -   https://github.com/yssickjgd/robowalker_train
-   由于Gitee平台有容量限制, 但本仓库容量较大, 因此该项目于202308242000转移至Github以及中国科大Linux用户协会校内Gitlab平台中. ==原Gitee链接仍有效, 只是不保持更新==
    -   对于STM32CubeMX基础的补充

        -   为资料统一且查找方便, 该教程修订到本教程系列PPT中, 其原链接如下

        -   https://gitee.com/mcpocket/RoboWalker/blob/master/%E7%94%B5%E6%8E%A7%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B/STM32%E7%B3%BB%E5%88%97%E6%95%99%E7%A8%8B.md

-   **配套PPT资料**在中国科大Rec网盘上
    -   https://rec.ustc.edu.cn/share/c8b3bbc0-32bc-11ee-98d2-2330cf7972b8
    -   该网盘内容以B站视频的更新时间为基准进行更新与修改

-   如果您想打**RoboGame校内赛**, 请看B站中的**STM32CubeMX基础**以及**RG电控培训**系列, 也可以考虑看一下**RM相关培训的各个理论部分**, 如算法, 程序设计思想等

## 致谢与参考

1.  感谢**中国科学技术大学 RoboWalker战队** 一路走来的伙伴们, 感谢**校学生Linux用户协会**提供的代码托管平台支持!
2.  特别鸣谢 **哈尔滨工业大学 ( 威海 ) HERO战队**, **防灾科技学院 思玄战队** 对本系列电控教程的批评与指导
3.  最后, 附上各大高校的相关开源项目. 我们的**Star**与**Fork**便是最大的关注支持与谢意! ( 按参考时间排序 )

-   **华南理工大学 华南虎战队** 电控开源SRML库
    -   https://github.com/scutrobotlab/RM2021_Hero/tree/master/Chassis/SRML

-   **吉林大学 TARS_Go战队** 电控开源架构
    -   https://gitee.com/tarsgo-embedded/UIML
-   **防灾科技学院 思玄战队** 超级电容功率控制算法
    -   https://bbs.robomaster.com/forum.php?mod=viewthread&tid=22101
-   **湖南大学 跃鹿战队** 电控开源架构
    -   https://gitee.com/hnuyuelurm/basic_framework
-   **青岛大学 XRobot战队** 电控开源架构
    -   https://github.com/xrobot-org/XRobot

## 加油! 

