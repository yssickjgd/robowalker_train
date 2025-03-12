# 云台自稳

## 1 文中数据的符号与含义

- 这一部分源自6.2结合陀螺仪的云台控制中, 但有些许改动
  
- 我们要明确
  
  - 只有在同一个坐标系下的矢量才可以进行加减运算, 否则需要转换到同一个坐标系下
- $^A_C\overrightarrow{q}_B$表示$A$坐标系参考情况下, $B$相对于$C$下的任一矢量, C若和A相同则可以删去C
  
- 相关坐标系定义

  - 大地坐标系为$O$, 即Odometry
  - 底盘坐标系为$C$, 即Chassis
  - 云台Yaw电机转过后的坐标系为$Y$, 即Yaw
  - 云台Pitch电机转过后的坐标系为$P$, 即Pitch
  - 如下图所示为除Odometry坐标系外, 其余三个坐标系的具体位姿

- 云台电机角度

  - 旋转正方向需要符合右手定则

  - 以云台Yaw电机左转为正方向, Yaw轴电机角度为$\alpha$, 设==目标角速度==为$\dot{\alpha}$

  - 以云台Pitch电机低头为正方向, Pitch轴电机角度为$\beta$, 设==目标角速度==为$\dot{\beta}$

    > 注: 一个物理量上方带一个点表示其对时间求一阶导数, 带两个点表示求二阶导数, 以此类推

  - 如图所示为$\alpha = -\frac{\pi}{2}, \beta= -\frac{\pi}{4}$的情形

    <img src="markdown.assets/坐标系示意图.jpg" alt="坐标系示意图" style="zoom: 33%;" />

- 陀螺仪数据
  - 底盘陀螺仪绑定在底盘$C$上, 当前角速度为$^C_O\overrightarrow{\omega}_C$, 三个轴的角速度分量分别简写为${^C\omega_x}, \ {^C\omega_y}, \ {^C\omega_z}$
  - 云台陀螺仪绑定在云台Pitch端$P$上, 当前角速度为$^P_O\overrightarrow{\omega}_P$, 三个轴的角速度分量分别简写为${^P\omega_x}, \ {^P\omega_y}, \ {^P\omega_z}$

## 2 云台自稳的分析


- 各个坐标系间旋转矩阵变换分析
  $$
  ^CY = Yaw(\alpha) \\
  ^YP = Pitch(\beta) \\
  ^CP = Yaw(\alpha) Pitch(\beta)
  
  \label{旋转矩阵变换分析}
  $$
  
  
- 自稳角速度分析

  
  - 将底盘陀螺仪的角速度转换到云台Yaw轴电机所在的坐标系$Y$, 保证云台Yaw轴始终相对于坐标系$O$朝向不变, 也就是相对于坐标系$O$, 角速度为0
  
  - 将底盘陀螺仪的角速度转换到云台Pitch轴电机所在的坐标系$Y$, 保证云台Pitch轴始终相对于坐标系$O$朝向不变, 也就是相对于坐标系$O$, 角速度为0
    $$
    \ {^YC} \ {^C_O\overrightarrow{\omega}_C} + \dot{\alpha} \overrightarrow{e}_z = \overrightarrow{0} \\
      
      \ {^PC} \ {^C_O\overrightarrow{\omega}_C} + \dot{\beta} \overrightarrow{e}_y = \overrightarrow{0} \\
    
    \label{角度补偿}
    $$
  
- 公式$\eqref{角度补偿}$中

  - $^C_O\overrightarrow{\omega}_C$和$^P_O\overrightarrow{\omega}_P$由陀螺仪获取
  - $^YC$是$^CY$的逆矩阵, 或者说转置, $^PC$是$^CP$的逆矩阵, 也是转置, 均可由云台电机编码器读取角度后利用公式$\eqref{旋转矩阵变换分析}$计算即可
  - $\overrightarrow{e}_y$就是单位向量$(0, 1, 0)^T$, $\overrightarrow{e}_z$就是单位向量$(0, 0, 1)^T$
  - 代入具体值, 联立方程即可求解出$\dot{\alpha}, \dot{\beta}$

- 对于单片机嵌入式系统, 此处可能不太适合大量矩阵进行运算, 因此需要求得最终结果

  - 自行求解即可, 锻炼一下计算能力

  - 放个结果在这
    $$
    \dot{\alpha} = -{^C\omega_z} \\
    
    \dot{\beta} = \sin(\alpha) \ {^C\omega_x} - \cos(\alpha) \ {^C\omega_y} \\
    $$
    