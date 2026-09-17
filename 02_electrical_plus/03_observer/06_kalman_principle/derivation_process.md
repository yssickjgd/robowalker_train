# Kalman滤波器优化电机反馈速度

## 1 复习

## 2 适用的系统

### 2.1 过程模型

- 一般过程模型
  $$
  \overrightarrow s_{n + 1} = \overrightarrow f(\overrightarrow s_{n}, \overrightarrow c_{n})
  $$

- 线性过程模型
  $$
  \overrightarrow s_{n + 1} = A\overrightarrow s_{n}+B\overrightarrow c_{n}
  $$

- 举物体过程模型
  $$
  \begin{pmatrix}
  x_{n + 1} \\
  v_{n + 1}
  \end{pmatrix} = \begin{pmatrix}
  1 & \Delta t \\
  0 & 1 + \frac{k \Delta t}{m}
  \end{pmatrix} \begin{pmatrix}
  x_{n} \\
  v_{n}
  \end{pmatrix} + \begin{pmatrix}
  0 \\
  \frac{\Delta t}{m}F_n + \frac{G}{m}
  \end{pmatrix}
  $$

- 电机反馈速度过程模型
  $$
  \begin{pmatrix}
  x_{n + 1} \\
  v_{n + 1}
  \end{pmatrix} = \begin{pmatrix}
  1 & \Delta t \\
  0 & 1
  \end{pmatrix} \begin{pmatrix}
  x_{n} \\
  v_{n}
  \end{pmatrix}
  $$

### 2.2 测量模型

- 一般测量模型
  $$
  \overrightarrow m_n = \overrightarrow h(\overrightarrow s_n)
  $$

- 线性测量模型
  $$
  \overrightarrow m_n = H \overrightarrow s_n
  $$

- 举物体测量模型
  $$
  x_{n} = \begin{pmatrix}
  1 & 0
  \end{pmatrix} \begin{pmatrix}
  x_{n} \\
  v_{n}
  \end{pmatrix}
  $$

- 电机反馈速度测量模型
  $$
  \begin{pmatrix}
  x_{n} \\
  v_{n}
  \end{pmatrix} = \begin{pmatrix}
  1 & 0 \\
  0 & 1 \\
  \end{pmatrix} \begin{pmatrix}
  x_{n} \\
  v_{n}
  \end{pmatrix}
  $$
  

## 3 系统模型

接下来的公式推导过程, 我们将研究如下两个模型

- 线性过程模型
  $$
  \overrightarrow s_{n + 1} = A\overrightarrow s_{n}+B\overrightarrow c_{n}
  $$

- 线性测量模型
  $$
  \overrightarrow m_n = H \overrightarrow s_n
  $$

## 4 数据融合

- ==复习一下Luenberger观测器时约定的符号==

  - 关于模型与融合
    - 对于绝对真实的数据, 我们不带角标
    - 对于过程模型估计出的数据, 我们用tilde来表示这是个先验估计量, 如$\tilde x$
    - 对于测量模型估计出的数据, 我们用hat来表示这是个传感器计算得来的量, 如$\hat x$
    - 对于数据融合后的数据, 我们用bar来表示这是个融合了过程模型和传感器噪声的加权平均过的后验估计量, 如$\bar x$
    - 对于传感器测得的数据, 我们用breve来表示这是个瞬时量, 如$\breve x$

### 4.1 融合公式的产生

- 和Kalman滤波器优化电机反馈速度章节一样, 我们兵分两路, 也就是过程模型一条路和测量模型一条路

- 对于过程模型, 我们已知迭代的信息$\bar{\overrightarrow s}_{n}$, 还有控制器在此时此刻的输出$\overrightarrow c_{n}$

  先代入过程模型进行估计, 从而得到先验状态$\tilde{\overrightarrow s}_{n+1}$
  $$
  \tilde{\overrightarrow s}_{n+1} = A \bar{\overrightarrow s}_{n} + B \overrightarrow c_{n}
  \label{过程模型}
  $$

- 对于测量模型, 我们有一个矩阵$H$, 用来表征将状态向量通过传感器投影到传感器采集数据所在的线性空间
  $$
  \breve{\overrightarrow m}_{n+1} = H \hat{\overrightarrow s}_{n+1}
  \label{测量模型}
  $$
  由于我们观测的状态向量一般是全状态的, 而传感器不一定能获取全部的状态, 因此矩阵$H$不一定是可逆矩阵, 且秩情况也是未知的, 这里形式化地设一个伪逆矩阵$H^{-}$, 得到
  $$
  \hat{\overrightarrow s}_{n+1} = H^{-} \breve{\overrightarrow m}_{n+1}
  $$

- 而后, 将过程模型与测量模型进行以$I-G$和$G$为权重的融合, 有
  $$
  \bar{\overrightarrow s}_{n+1} = (I-G) \tilde{\overrightarrow s}_{n+1} + G \hat{\overrightarrow s}_{n+1}
  $$

  我们换一种形式
  $$
  \bar{\overrightarrow s}_{n+1} = \tilde{\overrightarrow s}_{n+1} + G (\hat{\overrightarrow s}_{n+1} - \tilde{\overrightarrow s}_{n+1})
  $$

- 如果我们是Luenberger观测器, 那么到这一步后就结束了. 接下来的工作就是硬着头皮调参这个$G$矩阵

  但我们不是Luenberger, 我们是Kalman

  - 这个$G$矩阵, 在Kalman滤波器中, 叫做状态增益矩阵

    但实际工程中, 考虑到传感器的数据是从状态空间选取一定的维度投影而来的, 数据维度和信息量都会有损失, 伪逆矩阵$H^-$不一定能完全恢复出现场状态, 也就意味着$\hat{\overrightarrow s}_{n+1}$不一定能求出

    因此我们会避免涉及$H^-$的情况, 下面我们将消掉$H^-$与$\hat{\overrightarrow s}_{n+1}$

  - 首先, 将测量模型$\eqref{测量模型}$代入, 有
    $$
    \bar{\overrightarrow s}_{n+1} = \tilde{\overrightarrow s}_{n+1} + G (H^{-} \breve{\overrightarrow m}_{n+1} - \tilde{\overrightarrow s}_{n+1})
    $$
    而后, 将$H^-$提出来, 也就是括号外左乘$H^-$, 括号内左乘$H$, 有
    $$
    \bar{\overrightarrow s}_{n+1} = \tilde{\overrightarrow s}_{n+1} + G H^{-} (\breve{\overrightarrow m}_{n+1} - H\tilde{\overrightarrow s}_{n+1})
    $$
    令$K = GH^-$, 则得到观测增益矩阵$K$表达的数据融合形式
    $$
    \bar{\overrightarrow s}_{n+1} = \tilde{\overrightarrow s}_{n+1} + K (\breve{\overrightarrow m}_{n+1} - H\tilde{\overrightarrow s}_{n+1})
    $$

  - 接下来我们就需要用一种方式来设计最优的$K$矩阵, 让这个$K$变得有数学依据起来

- 和$G$矩阵一样, 我们的$K$矩阵也是每一步每一步迭代算出来, 而且时刻保持更新的, 因此就不能像Luenberger观测器一样写死了, 因此也需要有一个角标

  最终的最终, 我们的融合公式形式为
  $$
  \bar{\overrightarrow s}_{n+1} = \tilde{\overrightarrow s}_{n+1} + K_{n+1} (\breve{\overrightarrow m}_{n+1} - H\tilde{\overrightarrow s}_{n+1})
  \label{融合公式}
  $$

### 4.2 增益矩阵的设计

- 根据一开始的假设, 我们这里需要引入模型化的噪声

  - 过程模型的噪声源于系统动力学演化时叠加的随机扰动, 是个随机向量$\overrightarrow{w}$, 其噪声满足期望为$\overrightarrow{0}$, 协方差矩阵为$Q$的正态分布
    $$
    \overrightarrow s_{n+1} = A \overrightarrow s_{n} + B \overrightarrow c_{n} + \overrightarrow{w}_n
    \label{带噪过程模型}
    $$

  - 测量模型的噪声源于传感器采集后叠加的随机扰动, 是个随机向量$\overrightarrow{v}$, 其噪声满足期望为$\overrightarrow{0}$, 协方差矩阵为$R$的正态分布
    $$
    \breve{\overrightarrow m}_n = H \overrightarrow s_n + \overrightarrow{v}_n
    \label{带噪测量模型}
    $$

  - 随机向量$\overrightarrow{w}$和$\overrightarrow{v}$相互独立

- 因此, 我们需要了解融合后数据$\bar{\overrightarrow s}_{n+1}$相较于真实值的概率分布, 也就是后验误差$\bar{\overrightarrow{e}}_{n+1}$的概率分布

  找到这个分布协方差矩阵$\bar P_{n+1}$的对角元素之和 ( 相当于所有被观测状态的方差之和 )

  当融合后的方差最小时, 我们融合后的结果也就取到了最优

  说白了, 得先算下述算式的概率分布再谈后面的
  $$
  \bar{\overrightarrow{e}}_{n+1} = \overrightarrow s_{n+1} - \bar{\overrightarrow s}_{n+1}
  $$

- 将融合公式$\eqref{融合公式}$代入到上式, 整理, 有
  $$
  \bar{\overrightarrow{e}}_{n+1} = \overrightarrow s_{n+1} - (\tilde{\overrightarrow s}_{n+1} + K_{n+1} (\breve{\overrightarrow m}_{n+1} - H\tilde{\overrightarrow s}_{n+1}))
  $$

  将带噪测量模型$\eqref{带噪测量模型}$代入到上式, 整理, 有
  $$
  \bar{\overrightarrow{e}}_{n+1} = (I - K_{n+1}H)(\overrightarrow s_{n+1} - \tilde{\overrightarrow s}_{n+1}) - K_{n+1} \overrightarrow{v}_{n+1}
  $$
  再将带噪过程模型$\eqref{带噪过程模型}$和过程模型$\eqref{过程模型}$代入到上式, 整理, 有
  $$
  \bar{\overrightarrow{e}}_{n+1} = (I - K_{n+1}H)(A (\overrightarrow s_{n} - \bar{\overrightarrow s}_{n}) + \overrightarrow{w}_{n}) - K_{n+1}  \overrightarrow{v}_{n+1}
  $$
  也就是
  $$
  \bar{\overrightarrow{e}}_{n+1} = (I - K_{n+1}H)(A \bar{\overrightarrow{e}}_{n} + \overrightarrow{w}_{n}) - K_{n+1}  \overrightarrow{v}_{n+1}
  $$
  上式看起来虽然直观, 但不利于推演, 因为它是三个随机向量线性叠加共同作用的结果

  不妨定义一个新的随机变量$\tilde{\overrightarrow e}_{n+1}$, 这样就相当于把涉及了三个随机向量的一步随机过程拆分成了两步, 每一步随机过程都是两个正态分布随机向量的线性叠加

  - 第一步, 预测步骤, 在上一步后验误差基础上, 引入过程模型的随机性误差
    $$
    \tilde{\overrightarrow e}_{n+1} = A \bar{\overrightarrow{e}}_{n} + \overrightarrow{w}_{n}
    \label{先验误差计算公式}
    $$

  - 第二步, 修正步骤, 在预测步骤先验误差基础上, 引入测量模型的随机性误差
    $$
    \bar{\overrightarrow{e}}_{n+1} = (I - K_{n+1}H)\tilde{\overrightarrow e}_{n+1} - K_{n+1}  \overrightarrow{v}_{n+1}
    \label{后验误差计算公式}
    $$

  - 我们在这一小节要研究的还是融合后的误差$\bar{\overrightarrow e}_{n+1}$, 最终目的是最小化这个误差的方差

- 显然, $\tilde{\overrightarrow e}_{n+1}$的随机性来源于第$n$步自带的误差与$n$到$n+1$步过程模型随机性带来的误差, 而$\overrightarrow{v}_{n+1}$的随机性来源于第$n+1$步传感器自身的误差, 这两个随机变量是相互独立的

  根据独立正态分布的线性叠加公式, 该随机向量也是一个正态分布, 且期望为$\overrightarrow{0}$, 协方差矩阵为
  $$
  \bar P_{n+1}= (I - K_{n+1}H)\tilde P_{n+1}(I - K_{n+1}H)^T + K_{n+1}RK_{n+1}^T
  \label{后验误差协方差矩阵}
  $$

- 我们现在就是得设计$K_{n+1}$, 使协方差矩阵$\bar P_{n+1}$的迹最小

  具体地, 我们相当于要最小化函数$f(K_{n+1})$, 其中
  $$
  f(K_{n+1}) = \mathrm{tr} ((I - K_{n+1}H)\tilde P_{n+1}(I - K_{n+1}H)^T + K_{n+1}RK_{n+1}^T)
  $$
  求导! 利用矩阵迹的性质和矩阵求导公式, 我们有
  $$
  D_{K_{n+1}}(f(K_{n+1})) = 2(K_{n+1}(R+H \tilde P_{n+1}H^T) - \tilde P_{n+1}H^T)
  $$
  令$D_{K_{n+1}}(f(K_{n+1})) = O$, 求解得到
  $$
  K_{n+1} = \tilde P_{n+1}H^T(R+H \tilde P_{n+1}H^T)^{-1}
  
  \label{增益矩阵}
  $$

### 4.3 先验误差协方差矩阵的求解

- 事到如今, 我们已经有了融合公式$\eqref{融合公式}$, 并知道了增益矩阵$\eqref{增益矩阵}$的计算方法

  然而, 增益矩阵$K_{n+1}$中, 仍旧有未知数需要求解, 那就是先验误差的协方差矩阵$\tilde P_{n+1}$

  这一小节, 我们要研究$\tilde{\overrightarrow e}_{n+1}$的协方差矩阵
  $$
  \tilde{\overrightarrow e}_{n+1} = A \bar{\overrightarrow{e}}_{n} + \overrightarrow{w}_{n}
  $$

- 根据独立正态分布的线性叠加公式, 该随机向量也是一个正态分布, 且期望为$\overrightarrow{0}$, 协方差矩阵为
  $$
  \tilde P_{n+1} = A \bar P_{n} A^T + Q
  \label{先验误差协方差矩阵}
  $$

### 4.4 公式总结

==虽然下面的话在上一期视频也已经讲过了, 但我仍保留了下来, 因为推一遍公式之后, 再读一遍经验总结, 仍旧会有不一样的收获. 经典公式, 常看常新==

- 目前, 我们已经有了融合公式$\eqref{融合公式}$, 增益矩阵$\eqref{增益矩阵}$, 还知道了先验误差协方差矩阵$\eqref{先验误差协方差矩阵}$的计算方法

  与此同时, 先验误差协方差矩阵$\eqref{先验误差协方差矩阵}$中, $\bar P_{n}$也是已知的, 根据后验误差协方差矩阵$\eqref{后验误差协方差矩阵}$可以求出

- 过程模型和测量模型协方差矩阵$Q, R$在原理上, 是随机变量的协方差矩阵, 可以通过采集数据得到, 但实际上我们仍旧可以进行手动调参来操作数据融合的偏好程度

  > - 如若系统的过程模型极其精准, 则可以降低过程模型的协方差矩阵数值. 显然本实验无法这么做, 因为我们没有对电机输出的扭矩进行建模
  >
  > - 再比如, 调过IMU的同学可能有一定的感受, 加速度计噪声比陀螺仪要大得多, 但如若想让Pitch和Roll角度快速收敛, 我们仍旧要手动调整, 适当降低加速度计的协方差矩阵

- 如若需要开启递推模式, 我们需要知道初始的状态估计值$\bar{\overrightarrow s}_0$和初始后验误差协方差矩阵$\bar P_{0}$

  初始状态估计值$\bar{\overrightarrow s}_0$是可以通过传感器直接获取或直接赋值为0, 而后逐步收敛的

  > - 如果系统传感器可以直接锁定一部分状态信息, 则可以根据最初始传感器的状态直接反解出部分状态信息用于初始化. 比如调过IMU的同学可能有一定经验, IMU可以通过加速度计的初始状态来反解出默认状态下 ( Yaw角度为0 ) 的四元数信息用于初始化状态

  初始后验误差协方差矩阵$\bar P_{0}$是调参得到的, 数量级一般和协方差矩阵$Q, R$接近

  $\bar P_{0}$越大, 则说明观测器越不信任最初始的估计, 越迫切请求传感器或者说测量模型来修正它

  > - 如果系统已有知识对某个观测的量拥有一定的物理直觉, 如初始角度误差在$\pm 5$rad, 角速度误差在$\pm 1$rad/s, 那么初始的协方差矩阵可以设置为$\bar P_{0} = \begin{pmatrix} 5^2 & 0 \\ 0 & 1^2\end{pmatrix} = \begin{pmatrix} 25 & 0 \\ 0 & 1\end{pmatrix}$
  >
  > - 如果系统对初始状态完全不了解, 则可以直接无脑设置一个较大的值, 如$\bar P_{0} = 10^6I$
  >
  > - 如果系统对初始状态极其信任, 则可以设置一个较小的值, 如$\bar P_{0} = 10^{-6}I$, 但尽量避免设置为零矩阵, 否则系统会在很长时间内无脑相信过程模型, 观测器收敛极慢甚至发散
  >
  > - 当然, 系统的不同状态可信任程度也可以不同. 比如我们本节课研究的电机角度与角速度观测器系统, 其角度误差很低, 而角速度误差较高, 因此在初始化时可以更多信任角度而非角速度, 如$\bar P_{0} = \begin{pmatrix} 1 & 0 \\ 0 & 100\end{pmatrix}$
  > - 这个值意思意思即可, 因为$\bar P_{0}$对系统的作用会随着时间的推移而逐渐衰减, 最终起到融合作用的仍旧是过程模型和测量模型的协方差矩阵$Q, R$

- 最终, 摆一下本节课Kalman滤波器的通用递推公式, 已知$\bar {\overrightarrow s}_0, \bar P_{0}$

  - predict

    - 预测过程
      $$
      \tilde{\overrightarrow s}_{n+1} = A \bar{\overrightarrow s}_{n} + B \overrightarrow c_{n}
      $$

    - 先验误差估计
      $$
      \tilde P_{n+1} = A \bar P_{n} A^T + Q
      $$

  - update

    - 计算增益

      $$
      K_{n+1} = \tilde P_{n+1}H^T(R+H \tilde P_{n+1}H^T)^{-1}
      $$

    - 修正过程
      $$
      \bar{\overrightarrow s}_{n+1} = \tilde{\overrightarrow s}_{n+1} + K_{n+1} (\breve{\overrightarrow m}_{n+1} - H\tilde{\overrightarrow s}_{n+1})
      $$

    - 后验误差估计
      $$
      \bar P_{n+1}= (I - K_{n+1}H)\tilde P_{n+1}(I - K_{n+1}H)^T + K_{n+1}RK_{n+1}^T
      $$

  - 一步一步走, 一环扣一环, 最终实现了Kalman滤波器的功能

## 5 总结与分析

- 关于后验误差估计, 其实它仍旧能进一步化简

  展开后验误差协方差矩阵$\eqref{后验误差协方差矩阵}$, 有
  $$
  \bar P_{n+1}= \tilde P_{n+1} - \tilde P_{n+1}H^TK_{n+1}^T - K_{n+1}H\tilde P_{n+1} + K_{n+1}HPH^TK^T + K_{n+1}RK_{n+1}^T
  $$
  将最左侧的$K_{n+1}$和最右侧的$K_{n+1}^T$同时提出来, 有
  $$
  \bar P_{n+1}= \tilde P_{n+1} - \tilde P_{n+1}H^TK_{n+1}^T - K_{n+1}H\tilde P_{n+1} + K_{n+1}(HPH^T + R)K^T_{n+1}
  $$
  由于
  $$
  K_{n+1} = \tilde P_{n+1}H^T(R+H \tilde P_{n+1}H^T)^{-1}
  $$
  则有
  $$
  K_{n+1} (R+H \tilde P_{n+1}H^T) = \tilde P_{n+1}H^T
  $$
  代入, 有
  $$
  \bar P_{n+1}= \tilde P_{n+1} - \tilde P_{n+1}H^TK_{n+1}^T - K_{n+1}H\tilde P_{n+1} + \tilde P_{n+1}H^TK^T_{n+1}
  $$
  也就是
  $$
  \bar P_{n+1}= (I - K_{n+1}H)\tilde P_{n+1}
  
  \label{化简的后验误差协方差矩阵}
  $$
  显然, 这种形式是更简洁的, 其计算量更小. 但其弊端也极其明显!

  由于计算机处理的浮点数等数据都是有误差和近似的, 经过大量迭代, 误差会被放大极其严重, 导致数据失真

  公式$\eqref{后验误差协方差矩阵}$虽然计算量更大, 但利用了正定矩阵的特性, 从而保证其经过大量迭代后仍旧对称且正定, 观测器会正常工作

  而公式$\eqref{化简的后验误差协方差矩阵}$的形式, 无法从数学原理上保证协方差矩阵的正定性, 经过大量迭代后协方差矩阵会加剧其非对称非正定性, 导致观测器发散, 甚至引发中间结果的矩阵不可逆, 造成观测器崩溃

  > 求稳的长期工程应用中, 用$\eqref{后验误差协方差矩阵}$, 也叫作==Joseph形式==
  >
  > 考试和随手推公式， 以及短时间需要快速计算的代码验证, 用$\eqref{化简的后验误差协方差矩阵}$

- 未来展望
  
  - 扩展Kalman滤波器
  - 介绍频选滤波器
  - 以六轴IMU解算为例讲述扩展Kalman滤波器
  - 介绍同样以状态空间为研究对象的LQR控制算法
  - ...