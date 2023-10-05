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

1.  电控 ( electrical )
    1.  电控简介 ( electrical introduction )
        1.  电控导论
    2.  STM32CubeMX基础 ( stm32cubemx basic )
        1.  环境配置与基础教学
        2.  GPIO输出电压
        3.  EXTI外部中断
        4.  TIM定时器中断
            -   作业
        5.  UART通信
        6.  PWM呼吸灯
        7.  ADC测电压
    3.  电机控制 ( motor control )
        1.  C++
            -   作业
        2.  CAN通信
        3.  CAN接口与大疆RM电机初步控制
        4.  UART接口与串口绘图
        5.  串口绘图接口与电机反馈
        6.  PID算法
        7.  PID接口与电机闭环控制
        8.  电机接口与多环PID
        9.  动态目标与前馈PID
        11.  番外篇 达妙电机控制
    4.  架构开源代码教学 ( code framework )
        1.  Git基础操作教学
        2.  电控嵌入式架构分析
        3.  驱动接口层之USB虚拟串口
        5.  算法层之斜坡控制器
        8.  设备层之遥控器模块
        7.  设备层之裁判系统学生串口
        8.  模块层之底盘
        9.  模块层之云台
        10.  模块层之发射机构
        11.  交互层
        12.  任务层
    5.  滤波 ( filter )
        1.  卡尔曼滤波器
        2.  傅里叶变换
        3.  傅里叶滤波器
    6.  陀螺仪 ( gyroscope )
        1.  陀螺仪简介
        2.  SPI通信
        3.  SPI接口与陀螺仪读写
        4.  欧拉角、四元数及其物理意义
        5.  三轴陀螺仪姿态解算
        6.  六轴陀螺仪姿态解算
        7.  九轴陀螺仪姿态解算
    7.  PCB电路板设计教程 ( pcb design )
        1.  立创EDA基本操作教学
        2.  简易分电板
        3.  超级电容控制理论
        4.  超级电容开源电路
        5.  超级电容开源代码
2.  电控眼里的比赛 ( referee )
    1.  官方软件的获取与使用
    2.  比赛通用规则解读
    3.  裁判系统接线规则
    4.  裁判系统协议补充说明
3.  电控与机械组 ( mechanical )
    1.  机械组工作内容
    2.  Solidworks建模基本方法
    3.  Solidworks导出加工图
    4.  常见的设计技巧
    5.  常见的加工工艺
    6.  兵种开源图纸
4.  电控与视觉算法组 ( vision algorithm )
    1.  视觉算法组工作内容
    2.  环境配置
    3.  自瞄基本策略
    4.  能量机关基本策略
    5.  视觉开源代码
    6.  哨兵巡航与击打策略
    7.  哨兵上位机开源代码
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

-   前置知识需要, 且仅需要**C语言**
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
-   由于Gitee平台有容量限制, 但本仓库容量较大, 因此该项目于202308242000转移至Github以及中国科大Linux用户协会校内Gitlab平台中. *原Gitee链接仍有效, 只是不保持更新*
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

