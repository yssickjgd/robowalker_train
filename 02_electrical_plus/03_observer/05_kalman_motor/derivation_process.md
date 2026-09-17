# Kalman滤波器优化电机反馈速度

## 1 数据特点

## 2 观测器设计

### 2.1 过程模型

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

### 2.3 数据融合

- ==复习一下Luenberger观测器时约定的符号==

  - 关于模型与融合
    - 对于绝对真实的数据, 我们不带角标
    - 对于过程模型估计出的数据, 我们用tilde来表示这是个先验估计量, 如$\tilde x$
    - 对于测量模型估计出的数据, 我们用hat来表示这是个传感器计算得来的量, 如$\hat x$
    - 对于数据融合后的数据, 我们用bar来表示这是个融合了过程模型和传感器噪声的加权平均过的后验估计量, 如$\bar x$
    - 对于传感器测得的数据, 我们用breve来表示这是个瞬时量, 如$\breve x$

- 由于电机反馈的频率是1kHz, 因此$\Delta t=0.001$

  代入, 得到具体的过程模型和测量模型
  $$
  \begin{align}
  \begin{pmatrix}
  x_{n + 1} \\
  v_{n + 1}
  \end{pmatrix} & = \begin{pmatrix}
  1 & 0.001 \\
  0 & 1
  \end{pmatrix} \begin{pmatrix}
  x_{n} \\
  v_{n}
  \end{pmatrix} \\
  \begin{pmatrix}
  x_{n} \\
  v_{n}
  \end{pmatrix} & = \begin{pmatrix}
  x_{n} \\
  v_{n}
  \end{pmatrix}
  \end{align}
  $$
  
- 对于过程模型, 由于需要初始状态完成递推, 因此我们选择最开始电机反馈的数据作为角度和角速度信息$x_0 = \breve x_0, v_0 = \breve v_0$

#### 2.3.1 融合公式的产生

- 兵分两路, 也就是过程模型一条路和测量模型一条路

- 对于过程模型, 我们已知迭代的位置信息$\bar x_n$, 速度信息$\bar v_n$

  先代入过程模型进行估计, 从而得到先验状态$\begin{pmatrix} \tilde x_{n+1} \\ \tilde v_{n+1} \end{pmatrix}$
  
  $$
  \begin{pmatrix}
  \tilde x_{n+1} \\
  \tilde v_{n+1}
  \end{pmatrix} = \begin{pmatrix}
  1 & 0.001 \\
  0 & 1
  \end{pmatrix} \begin{pmatrix}
  \bar x_{n} \\
  \bar v_{n}
  \end{pmatrix}
  \label{过程模型}
  $$
  
- 对于测量模型, 很巧是一个满秩方阵, 甚至还是个单位矩阵

  很简单, 直接得到
  $$
  \begin{pmatrix}
  \hat x_{n+1} \\
  \hat v_{n+1}
  \end{pmatrix} = \begin{pmatrix}
  \breve x_{n+1} \\
  \breve v_{n+1}
  \end{pmatrix}
  \label{测量模型}
  $$
  
- 而后, 将过程模型与测量模型进行以$I-G$和$G$为权重的融合, 有
  $$
  \begin{pmatrix}
  \bar x_{n+1} \\
  \bar v_{n+1}
  \end{pmatrix} = (I-G) \begin{pmatrix}
  \tilde x_{n+1} \\
  \tilde v_{n+1}
  \end{pmatrix} + G \begin{pmatrix}
  \hat x_{n+1} \\
  \hat v_{n+1}
  \end{pmatrix}
  $$
  
  我们换一种形式
  $$
  \begin{pmatrix}
  \bar x_{n+1} \\
  \bar v_{n+1}
  \end{pmatrix} = \begin{pmatrix}
  \tilde x_{n+1} \\
  \tilde v_{n+1}
  \end{pmatrix} + G (\begin{pmatrix}
  \hat x_{n+1} \\
  \hat v_{n+1}
  \end{pmatrix} - \begin{pmatrix}
  \tilde x_{n+1} \\
  \tilde v_{n+1}
  \end{pmatrix})
  $$
  
- 如果我们是Luenberger观测器, 那么到这一步后就结束了. 接下来的工作就是硬着头皮调参这个$G$矩阵

  但我们不是Luenberger, 我们是Kalman

  - 这个$G$矩阵, 在Kalman滤波器中, 叫做状态增益矩阵
    - 已有Kalman滤波器基础的同学可能更熟悉另一个矩阵, 也就是观测增益矩阵$K$
    - 用$G$还是$K$表达融合公式都是等价的, 用观测增益矩阵$K$的表达形式将在下一节课进行讲解
  - 接下来我们就需要用一种方式来设计最优的$G$矩阵, 让这个$G$变得有数学依据起来

- 既然我们的$G$矩阵是每一步每一步迭代算出来, 而且时刻保持更新的, 因此就不能像Luenberger观测器一样写死了, 因此也需要有一个角标

  最终的最终, 我们的融合公式形式为
$$
\begin{pmatrix}
\bar x_{n+1} \\
\bar v_{n+1}
\end{pmatrix} = \begin{pmatrix}
\tilde x_{n+1} \\
\tilde v_{n+1}
\end{pmatrix} + G_{n+1} (\begin{pmatrix}
\hat x_{n+1} \\
\hat v_{n+1}
\end{pmatrix} - \begin{pmatrix}
\tilde x_{n+1} \\
\tilde v_{n+1}
\end{pmatrix})
\label{融合公式}
$$

#### 2.3.2 增益矩阵的设计

- 根据一开始的假设, 我们这里需要引入模型化的噪声

  - 过程模型的噪声源于系统动力学演化时叠加的随机扰动, 是个随机向量$\overrightarrow{w}$, 其噪声满足期望为$\overrightarrow{0}$, 协方差矩阵为$Q$的正态分布
    $$
    \begin{pmatrix}
    x_{n + 1} \\
    v_{n + 1}
    \end{pmatrix} = \begin{pmatrix}
    1 & 0.001 \\
    0 & 1
    \end{pmatrix} \begin{pmatrix}
    x_{n} \\
    v_{n}
    \end{pmatrix} + \overrightarrow{w}_n
    \label{带噪过程模型}
    $$
  
  - 测量模型的噪声源于传感器采集后叠加的随机扰动, 是个随机向量$\overrightarrow{v}$, 其噪声满足期望为$\overrightarrow{0}$, 协方差矩阵为$R$的正态分布
    $$
    \begin{pmatrix}
    \breve x_{n} \\
    \breve v_{n}
    \end{pmatrix} = \begin{pmatrix}
    x_{n} \\
    v_{n}
    \end{pmatrix} + \overrightarrow{v}_n
    \label{带噪测量模型}
    $$
    
  - 随机向量$\overrightarrow{w}$和$\overrightarrow{v}$相互独立

- 因此, 我们需要了解融合后数据$\begin{pmatrix}\bar x_{n+1} \\ \bar v_{n+1} \end{pmatrix}$相较于真实值的概率分布, 也就是后验误差$\bar{\overrightarrow{e}}_{n+1}$的概率分布

  找到这个分布协方差矩阵$\bar P_{n+1}$的对角元素之和 ( 相当于所有被观测状态的方差之和 )
  
  当融合后的方差最小时, 我们融合后的结果也就取到了最优
  
  说白了, 得先算下述算式的概率分布再谈后面的
  $$
  \bar{\overrightarrow{e}}_{n+1} = \begin{pmatrix}x_{n+1} \\ v_{n+1} \end{pmatrix} - \begin{pmatrix}\bar x_{n+1} \\ \bar v_{n+1} \end{pmatrix}
  $$
  
- 将融合公式$\eqref{融合公式}$代入到上式, 整理, 有
  $$
  \bar{\overrightarrow{e}}_{n+1} = \begin{pmatrix}
  x_{n+1} \\
  v_{n+1} 
\end{pmatrix} - \begin{pmatrix}
  \tilde x_{n+1} \\
  \tilde v_{n+1}
  \end{pmatrix} + G_{n+1}(\begin{pmatrix}
  \tilde x_{n+1} \\
  \tilde v_{n+1}
  \end{pmatrix} - \begin{pmatrix}
  \hat x_{n+1} \\
  \hat v_{n+1}
  \end{pmatrix}
  )
  $$
  
  将带噪测量模型$\eqref{带噪测量模型}$和测量模型$\eqref{测量模型}$代入到上式, 整理, 有
  $$
  \bar{\overrightarrow{e}}_{n+1} = (I - G_{n+1})(\begin{pmatrix}
  x_{n+1} \\
  v_{n+1} 
  \end{pmatrix} - \begin{pmatrix}
  \tilde x_{n+1} \\
  \tilde v_{n+1}
  \end{pmatrix}) - G_{n+1} \overrightarrow{v}_{n+1}
  $$
  再将带噪过程模型$\eqref{带噪过程模型}$和过程模型$\eqref{过程模型}$代入到上式, 整理, 有
  $$
  \bar{\overrightarrow{e}}_{n+1} = (I - G_{n+1})(\begin{pmatrix}
  1 & 0.001 \\
  0 & 1
  \end{pmatrix} (\begin{pmatrix}
  x_{n} \\
  v_{n} 
  \end{pmatrix} - \begin{pmatrix}
  \bar x_{n} \\
  \bar v_{n}
  \end{pmatrix}) + \overrightarrow{w}_{n}) - G_{n+1} \overrightarrow{v}_{n+1}
  $$
  也就是
  $$
  \bar{\overrightarrow{e}}_{n+1} = (I - G_{n+1})(\begin{pmatrix} 1 & 0.001 \\ 0 & 1\end{pmatrix} \bar{\overrightarrow{e}}_{n} + \overrightarrow{w}_{n}) - G_{n+1} \overrightarrow{v}_{n+1}
  $$
  上式看起来虽然直观, 但不利于推演, 因为它是三个随机向量线性叠加共同作用的结果
  
  不妨定义一个新的随机变量$\tilde{\overrightarrow e}_{n+1}$, 这样就相当于把涉及了三个随机向量的一步随机过程拆分成了两步, 每一步随机过程都是两个正态分布随机向量的线性叠加
  
  - 第一步, 预测步骤, 在上一步后验误差基础上, 引入过程模型的随机性误差
    $$
    \tilde{\overrightarrow e}_{n+1} = \begin{pmatrix} 1 & 0.001 \\ 0 & 1 \end{pmatrix} \bar{\overrightarrow{e}}_{n} + \overrightarrow{w}_{n}
    \label{先验误差计算公式}
    $$
  
  - 第二步, 修正步骤, 在预测步骤先验误差基础上, 引入测量模型的随机性误差
    $$
    \bar{\overrightarrow e}_{n+1} = (I - G_{n+1})\tilde{\overrightarrow e}_{n+1} - G_{n+1} \overrightarrow{v}_{n+1}
    \label{后验误差计算公式}
    $$
  
  - 我们在这一小节要研究的还是融合后的误差$\bar{\overrightarrow e}_{n+1}$, 最终目的是最小化这个误差的方差
  
- 显然, $\tilde{\overrightarrow e}_{n+1}$的随机性来源于第$n$步自带的误差与$n$到$n+1$步过程模型随机性带来的误差, 而$\overrightarrow{v}_{n+1}$的随机性来源于第$n+1$步传感器自身的误差, 这两个随机变量是相互独立的

  根据独立正态分布的线性叠加公式, 该随机向量也是一个正态分布, 且期望为$\overrightarrow{0}$, 协方差矩阵为
  $$
  \bar P_{n+1}= (I-G_{n+1})\tilde P_{n+1}(I-G_{n+1})^T + G_{n+1}RG_{n+1}^T
  \label{后验误差协方差矩阵}
  $$

- 我们现在就是得设计$G_{n+1}$, 使协方差矩阵$\bar P_{n+1}$的迹最小

  具体地, 我们相当于要最小化函数$f(G_{n+1})$, 其中
  $$
  f(G_{n+1}) = \mathrm{tr} ((I-G_{n+1})\tilde P_{n+1}(I-G_{n+1})^T + G_{n+1}RG_{n+1}^T)
  $$
  求导! 利用矩阵迹的性质和矩阵求导公式, 我们有
  $$
  D_{G_{n+1}}(f(G_{n+1})) = 2(G_{n+1}(R+\tilde P_{n+1}) - \tilde P_{n+1})
  $$
  令$D_{G_{n+1}}(f(G_{n+1})) = O$, 求解得到
  $$
  G_{n+1} = \tilde P_{n+1} (R+\tilde P_{n+1})^{-1}
  
  \label{增益矩阵}
  $$

#### 2.3.3 先验误差协方差矩阵的求解

- 事到如今, 我们已经有了融合公式$\eqref{融合公式}$, 并知道了增益矩阵$\eqref{增益矩阵}$的计算方法

  然而, 增益矩阵$G_{n+1}$中, 仍旧有未知数需要求解, 那就是先验误差的协方差矩阵$\tilde P_{n+1}$

  这一小节, 我们要研究$\tilde{\overrightarrow e}_{n+1}$的协方差矩阵
  $$
  \tilde{\overrightarrow e}_{n+1} = \begin{pmatrix} 1 & 0.001 \\ 0 & 1\end{pmatrix} \bar{\overrightarrow{e}}_{n} + \overrightarrow{w}_{n}
  $$

- 根据独立正态分布的线性叠加公式, 该随机向量也是一个正态分布, 且期望为$\overrightarrow{0}$, 协方差矩阵为
  $$
  \tilde P_{n+1} = \begin{pmatrix} 1 & 0.001 \\ 0 & 1\end{pmatrix} \bar P_{n} \begin{pmatrix} 1 & 0.001 \\ 0 & 1\end{pmatrix}^T + Q
  \label{先验误差协方差矩阵}
  $$

#### 2.3.4 公式总结

- 目前, 我们已经有了融合公式$\eqref{融合公式}$, 增益矩阵$\eqref{增益矩阵}$, 还知道了先验误差协方差矩阵$\eqref{先验误差协方差矩阵}$的计算方法

  与此同时, 先验误差协方差矩阵$\eqref{先验误差协方差矩阵}$中, $\bar P_{n}$也是已知的, 根据后验误差协方差矩阵$\eqref{后验误差协方差矩阵}$可以求出

- 过程模型和测量模型协方差矩阵$Q, R$在原理上, 是随机变量的协方差矩阵, 可以通过采集数据得到, 但实际上我们仍旧可以进行手动调参来操作数据融合的偏好程度

  > - 如若系统的过程模型极其精准, 则可以降低过程模型的协方差矩阵数值. 显然本实验无法这么做, 因为我们没有对电机输出的扭矩进行建模
  >
  > - 再比如, 调过IMU的同学可能有一定的感受, 加速度计噪声比陀螺仪要大得多, 但如若想让Pitch和Roll角度快速收敛, 我们仍旧要手动调整, 适当降低加速度计的协方差矩阵

- 此外, 目前仍旧有个问题, 我们只规定了系统被观测状态的初始状态直接拿电机反馈的数据作为角度和角速度信息, 也就是$\bar x_0 = \breve x_0, \bar v_0 = \breve v_0$

  但如若需要开启递推模式, 还需要知道初始后验误差协方差矩阵$\bar P_{0}$, 这个矩阵是调参得到的, 数量级一般和协方差矩阵$Q, R$接近

  $\bar P_{0}$越大, 则说明观测器越不信任最初始的估计, 越迫切请求传感器或者说测量模型来修正它

  > - 如果系统已有知识对某个观测的量拥有一定的物理直觉, 如初始角度误差在$\pm 5$rad, 角速度误差在$\pm 1$rad/s, 那么初始的协方差矩阵可以设置为$\bar P_{0} = \begin{pmatrix} 5^2 & 0 \\ 0 & 1^2\end{pmatrix} = \begin{pmatrix} 25 & 0 \\ 0 & 1\end{pmatrix}$
  >
  > - 如果系统对初始状态完全不了解, 则可以直接无脑设置一个较大的值, 如$\bar P_{0} = 10^6I$
  >
  > - 如果系统对初始状态极其信任, 则可以设置一个较小的值, 如$\bar P_{0} = 10^{-6}I$, 但尽量避免设置为零矩阵, 否则系统会在很长时间内无脑相信过程模型, 观测器收敛极慢甚至发散
  >
  > - 当然, 系统的不同状态可信任程度也可以不同. 比如我们本节课研究的电机角度与角速度观测器系统, 其角度误差很低, 而角速度误差较高, 因此在初始化时可以更多信任角度而非角速度, 如$\bar P_{0} = \begin{pmatrix} 1 & 0 \\ 0 & 100\end{pmatrix}$
  > - 这个值意思意思即可, 因为$\bar P_{0}$对系统的作用会随着时间的推移而逐渐衰减, 最终起到融合作用的仍旧是过程模型和测量模型的协方差矩阵$Q, R$

- 最终, 摆一下本节课Kalman滤波器优化电机反馈速度的迭代递推公式, 已知$\bar x_0, \bar v_0, \bar P_{0}$

  - predict
    
    - 预测过程
      $$
  \begin{pmatrix}\tilde x_{n+1} \\\tilde v_{n+1}\end{pmatrix} = \begin{pmatrix}1 & 0.001 \\0 & 1\end{pmatrix} \begin{pmatrix}\bar x_{n} \\\bar v_{n}\end{pmatrix}
      $$
    
    - 先验误差估计
      $$
    \tilde P_{n+1} = \begin{pmatrix} 1 & 0.001 \\ 0 & 1\end{pmatrix} \bar P_{n} \begin{pmatrix} 1 & 0.001 \\ 0 & 1\end{pmatrix}^T + Q
      $$
    
  - update
  
    - 计算增益
    
      $$
      G_{n+1} = \tilde P_{n+1} (R+\tilde P_{n+1})^{-1}
      $$
    
    - 修正过程
      $$
      \begin{pmatrix}
        \bar x_{n+1} \\
        \bar v_{n+1}
        \end{pmatrix} = \begin{pmatrix}
        \tilde x_{n+1} \\
        \tilde v_{n+1}
        \end{pmatrix} + G_{n+1} (\begin{pmatrix}
        \hat x_{n+1} \\
        \hat v_{n+1}
      \end{pmatrix} - \begin{pmatrix}
        \tilde x_{n+1} \\
        \tilde v_{n+1}
        \end{pmatrix})
      $$
    
    - 后验误差估计
      $$
      \bar P_{n+1}= (I-G_{n+1})\tilde P_{n+1}(I-G_{n+1})^T + G_{n+1}RG_{n+1}^T
      $$
  - 一步一步走, 一环扣一环, 最终实现了Kalman滤波器的功能