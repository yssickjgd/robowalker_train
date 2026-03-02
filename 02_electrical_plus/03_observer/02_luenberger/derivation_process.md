# Luenberger观测器

## 1 观测器的模型

继续拿举物体来举例, 上节课我们得到了如下过程模型和测量模型
$$
\begin{align}

	\begin{pmatrix}

        x_{n + 1} \\

        v_{n + 1}

    \end{pmatrix} & = \begin{pmatrix}

        1 & \Delta t \\

        0 & 1 + \frac{k \Delta t}{m}

    \end{pmatrix} \begin{pmatrix}

        x_{n} \\

        v_{n}

    \end{pmatrix} + \begin{pmatrix}

        0 \\

        \frac{\Delta t}{m}F_n + \frac{G}{m}\Delta t

    \end{pmatrix}
    
    \label{过程模型}
    
    \\

    x_{n} & = \begin{pmatrix}

        1 & 0

    \end{pmatrix} \begin{pmatrix}

        x_{n} \\

        v_{n}

    \end{pmatrix}
    
    \label{测量模型}

\end{align}
$$
这两个模型, 可以刻画我们要观测的状态变量信息

## 2 数据的迭代与修正思想

- ==规范一下符号, 极其重要!!!==

  对于传感器测得的数据, 我们用breve来表示这是个瞬时量, 如$\breve x$

  对于过程模型估计出的数据, 我们用tilde来表示这是个先验估计量, 如$\tilde x$

  对于测量模型估计出的数据, 我们用hat来表示这是个传感器计算得来的量, 如$\hat x$

  对于数据融合后的数据, 我们用bar来表示这是个融合了过程模型和传感器噪声的加权平均过的后验估计量, 如$\bar x$

- 我们用过程模型和测量模型都能观测到物体的位置和速度信息, 

  高中数学都学过数学归纳法, 我们先从0推到1, 而后再从n推到n+1, 后续小节干的就是这两件事, 而后总结一下

### 2.1 从0推到1

- 兵分两路, 也就是过程模型一条路和测量模型一条路

- 对于过程模型, 我们根据最开始传感器测得的数据作为位置信息$x_0 = \breve x_0$, 并假定速度初始信息$v_0=0\mathrm{m/s}$

  > 当然, 也可以观察两个时间点, 通过位置作差再除以计算频率生成速度初始信息

  先代入过程模型进行估计, 从而得到估计值$\begin{pmatrix} \tilde x_{1} \\ \tilde v_{1} \end{pmatrix}$

  由于该状态估计值是基于系统模型和输入信息这些已知且可控的信息得到的, 因此也叫作先验状态
  $$
  \begin{pmatrix}
  
      \tilde x_{1} \\
  
      \tilde v_{1}
  
  \end{pmatrix} = \begin{pmatrix}
  
      1 & \Delta t \\
  
      0 & 1 + \frac{k \Delta t}{m}
  
  \end{pmatrix} \begin{pmatrix}
  
      x_{0} \\
  
      v_{0}
  
  \end{pmatrix} + \begin{pmatrix}
  
      0 \\
  
      \frac{\Delta t}{m}F_0 + \frac{G}{m}\Delta t
  
  \end{pmatrix}
  $$
  
- 对于测量模型, 求公式$\eqref{测量模型}$的逆形式, 也就是将矩阵$H$求逆矩阵, 将形如$\overrightarrow{m} = H \overrightarrow{s}$的等式改为$H^{-1}\overrightarrow{m} = \overrightarrow{s}$, 那么直接就能拿到测量模型对应的状态信息$\overrightarrow s$

  可惜大多数的测量模型中, $H$并非一个方阵, 无法求逆矩阵

  > 你会算三行四列矩阵的行列式吗
  >
  > 这里可以插个眼, 感兴趣的同学可以搜一搜 " 伪逆矩阵 " 这个概念, 会有一定的启发

  求逆矩阵这种事情行不通了, 我们换一个思路, 一步一步来. ==注意! 一定要记住前面的上标符号的含义!!!==

  - 我们显然可以直接获取到位置信息
    $$
    \hat x_1 = \breve x_1
    $$
    而后, 将过程模型的位置信息$\tilde x_{1}$与测量模型的位置信息$\hat x_1$进行以$1-k_x$和$k_x$为权重的融合, 有
    $$
    \bar x_1 = (1-k_x) \tilde x_{1} + k_x \hat x_1
    $$
    上式还是过于简单粗暴, 我们换一种形式
    $$
    \bar x_1 = \tilde x_{1} + k_x (\hat x_1 - \tilde x_{1})
    $$
    这个形式, 就是一种互补滤波反馈修正思想的体现! 

    > 大家可以思考一下P控制器的原理

    我们用系统模型估计了一个位置值$\tilde x_{1}$, 而后再基于这个值, 向传感器测得的位置值$\hat x_{1}$靠近一段距离, 得到了一个 " 更令人信服 " 的值, 也就是$\bar x_1$

    最终, 也得到了一个 " 较优 " 的位置观测值$\bar x_1$作为$x_1$的最终值

    这里也不难发现, $\bar x$的上标 " bar " 其实带不带都一个意思, 后续为表示方便, 在公式中==非必要不体现==后验的 " bar " 属性

  - 对于速度信息, 我们是无法直接获取的, 也就是这个传感器的位置信息作为零阶信息损失了速度信息

    因此, 我们可以 " 形式化地 " 类比一下位置的观测方式, 直接拿位置信息来进行反馈修正
    $$
    \bar v_1 = \tilde v_{1} + \frac{k_v}{\Delta t} (\hat x_1 - \tilde x_{1})
    $$
    其中, $k_v$也是一个加权系数, 用不同的下角标, 只是为了区别且说明: 位置和速度的加权系数$k_x$和$k_v$可以不同

    > 有同学可能有疑问, 为什么我们能用位置的信息来反馈给速度
    >
    > 其实这个操作是可以理解的
    >
    > 比如: 当我们传感器反馈的位置值超过了我们根据系统模型预测的位置时, 这其实说明我们预测的速度偏低, 所以预测的位置才偏低, 因此真实的速度是比预测的速度要偏高的

### 2.2 从$n$推到$n+1$

- 同样, 还是过程模型和测量模型兵分两路

- 对于过程模型, 我们已知迭代的位置信息$x_n$, 速度信息$v_n$

  先代入过程模型进行估计, 从而得到先验状态$\begin{pmatrix} \tilde x_{n+1} \\ \tilde v_{n+1} \end{pmatrix}$
  $$
  \begin{pmatrix}
  
      \tilde x_{n+1} \\
  
      \tilde v_{n+1}
  
  \end{pmatrix} = \begin{pmatrix}
  
      1 & \Delta t \\
  
      0 & 1 + \frac{k \Delta t}{m}
  
  \end{pmatrix} \begin{pmatrix}
  
      x_{n} \\
  
      v_{n}
  
  \end{pmatrix} + \begin{pmatrix}
  
      0 \\
  
      \frac{\Delta t}{m}F_n + \frac{G}{m}\Delta t
  
  \end{pmatrix}
  $$
  
- 对于测量模型, 求逆矩阵还是行不通的

  - 我们显然可以直接获取到位置信息
    $$
    \hat x_{n+1} = \breve x_{n+1}
    $$
    而后, 将过程模型的位置信息$\tilde x_{n+1}$与测量模型的位置信息$\hat x_{n+1}$进行以$1-k$和$k$为权重的融合, 有
    $$
    \bar x_{n+1} = (1-k) \tilde x_{n+1} + k \hat x_{n+1}
    $$
    换成反馈修正思想的形式
    $$
    x_{n+1} = \tilde x_{n+1} + k (\hat x_{n+1} - \tilde x_{n+1})
    $$

  - 对于速度信息, 我们继续 " 形式化地 " 类比一下位置的观测方式
    $$
    \bar v_{n+1} = \tilde v_{n+1} + \frac{k_v}{\Delta t} (\hat x_{n+1} - \tilde x_{n+1})
    $$
    这就是一个 " 较优 " 的速度观测值

- 令$n=0$, 不难发现其照常成立, 因此迭代得到统一

## 3 总结

### 3.1 举物体模型的观测器设计

- 预测过程模型
  $$
  \begin{pmatrix}
  
      \tilde x_{n+1} \\
  
      \tilde v_{n+1}
  
  \end{pmatrix} = \begin{pmatrix}
  
      1 & \Delta t \\
  
      0 & 1 + \frac{k \Delta t}{m}
  
  \end{pmatrix} \begin{pmatrix}
  
      x_{n} \\
  
      v_{n}
  
  \end{pmatrix} + \begin{pmatrix}
  
      0 \\
  
      \frac{\Delta t}{m}F_n + \frac{G}{m}\Delta t
  
  \end{pmatrix}
  $$
  
- 计算测量模型
  $$
  \hat x_{n+1} = \breve x_{n+1}
  $$
  
- 修正过程
  $$
  \begin{pmatrix}
  
      x_{n+1} \\
  
      v_{n+1}
  
  \end{pmatrix} = \begin{pmatrix}
  
      \tilde x_{n+1} \\
  
      \tilde v_{n+1}
  
  \end{pmatrix} + \begin{pmatrix}
  
      k_x \\
  
      \frac{k_v}{\Delta t}
  
  \end{pmatrix} (\hat x_{n+1} - \tilde x_{n+1})
  $$
  

### 3.2 一般模型的观测器设计

- 此时, 我们要将观测器设计的流程推广到一般

  我们==沿用上一节课, 观测器概述中关于观测器的模型的定义和符号==, 继续推导离散时间系统的观测器设计方法

  已知的模型为过程模型和测量模型
  $$
  \begin{align}
  
  	\overrightarrow s_{n + 1} & = \overrightarrow f(\overrightarrow s_{n}, \overrightarrow c_{n}) \\
  	
  	\overrightarrow m_n & = \overrightarrow h(\overrightarrow s_n)
  
  \end{align}
  $$
  

- 预测过程模型
  $$
  \tilde{\overrightarrow s}_{n + 1} = \overrightarrow f(\overrightarrow s_{n}, \overrightarrow c_{n})
  $$
  

- 计算测量模型
  $$
  \hat{\overrightarrow s}_{n+1, proj} = \overrightarrow h^{-}(\overrightarrow m_{n+1})
  $$
  其中

  - $\overrightarrow h^{-}$是$\overrightarrow h$的逆函数
  - $\hat{\overrightarrow s}_{n, proj}$是根据传感器数据$\overrightarrow m_n$恢复出的状态信息的某几个投影分量, 不一定是全部的状态$\hat{\overrightarrow s}_{n}$. 而proj就是投影projection的缩写

- 修正过程
  $$
  \overrightarrow s_{n + 1} = \tilde{\overrightarrow s}_{n + 1} + K(\hat{\overrightarrow s}_{n+1, proj} - \tilde{\overrightarrow s}_{n+1, proj})
  
  \label{修正过程}
  $$
  其中

  - $K$就是我们要调的参数矩阵, 其行数与$\overrightarrow s$的维度一致, 列数与$\overrightarrow s_{proj}$的维度一致

- 修正过程新说

  我们的修正过程其实也可以换个思路调参, 也就是不需要操心用$\overrightarrow h^{-}$转一遍, 而是直接利用传感器的信息$\overrightarrow m$
  $$
  \overrightarrow s_{n + 1} = \tilde{\overrightarrow s}_{n + 1} + L(\overrightarrow m_{n+1} - h(\tilde{\overrightarrow s}_{n+1}))
  
  \label{修正过程新说}
  $$
  其中

  - $L$就是我们要调的参数矩阵, 其行数与$\overrightarrow s$的维度一致, 列数与$\overrightarrow m$的维度一致

  公式$\eqref{修正过程新说}$与$\eqref{修正过程}$在一阶近似的意义上是等价的, 也就是 " 当测量模型$\overrightarrow{h}(\overrightarrow s)$近似为一个形如$A\overrightarrow s + \overrightarrow{b}$的函数时, 这两个公式等价 " 

  > 证明过程如下
  >
  > - 在数学分析中我们学过, 将$\overrightarrow m = \overrightarrow h(\overrightarrow s)$在$\overrightarrow s_n$处作一阶泰勒展开, 得到
  >   $$
  >   \overrightarrow m = \overrightarrow h(\overrightarrow s) = \overrightarrow h(\overrightarrow s_n) + H_s(\overrightarrow s_n) (\overrightarrow s - \overrightarrow s_n)
  >   $$
  >   其中
  >
  >   - $H_s$是向量函数$\overrightarrow h$在$\overrightarrow s_0$点展开的Jacobi矩阵, 是一个矩阵函数. 这个也是我们数学分析中学过的内容, 其具体形式为
  >     $$
  >     \begin{pmatrix}
  >     
  >     	\frac{\partial h_1}{\partial s_1} & \frac{\partial h_1}{\partial s_2} & ... \\
  >     
  >     	\frac{\partial h_2}{\partial s_1} & \frac{\partial h_2}{\partial s_2} & ... \\
  >     
  >     	... \\
  >     
  >     \end{pmatrix}
  >     $$
  >
  > - 有了这些前置工作, 我们接下来要做的是, 将公式$\eqref{修正过程新说}$向公式$\eqref{修正过程}$的过程靠拢
  >
  >   将传感器与状态信息的映射关系, 也就是$\overrightarrow m_{n+1} = \overrightarrow h(\hat {\overrightarrow s}_{n+1})$代入$\eqref{修正过程新说}$
  >   $$
  >   \overrightarrow s_{n + 1} = \tilde{\overrightarrow s}_{n + 1} + L(\overrightarrow h(\hat {\overrightarrow s}_{n+1}) - h(\tilde{\overrightarrow s}_{n+1}))
  >   $$
  >   
  >
  >   将$\eqref{修正过程新说}$的$\overrightarrow h$在$\overrightarrow s_n$处泰勒展开
  >   $$
  >   \overrightarrow s_{n + 1} = \tilde{\overrightarrow s}_{n + 1} + L((\overrightarrow h(\overrightarrow s_n) + H_s(\overrightarrow s_n) (\hat{\overrightarrow s}_{n+1} - \overrightarrow s_n)) - (\overrightarrow h(\overrightarrow s_n) + H_s(\overrightarrow s_n) (\tilde{\overrightarrow s}_{n+1} - \overrightarrow s_n)))
  >   $$
  >   整理一下, 有
  >   $$
  >   \overrightarrow s_{n + 1} = \tilde{\overrightarrow s}_{n + 1} + L H_s(\overrightarrow s_n)(\hat{\overrightarrow s}_{n+1} - \tilde{\overrightarrow s}_{n+1})
  >   $$
  >   由于一阶近似, 因此Jacobi矩阵$H_s(\overrightarrow s)$可以被认为是一个常数, 不妨直接退化掉自变量, 将Jacobi矩阵记作$H_s$, 因此有
  >   $$
  >   \overrightarrow s_{n + 1} = \tilde{\overrightarrow s}_{n + 1} + L H_s(\hat{\overrightarrow s}_{n+1} - \tilde{\overrightarrow s}_{n+1})
  >   $$
  >   而公式$\eqref{修正过程}$为
  >   $$
  >   \overrightarrow s_{n + 1} = \tilde{\overrightarrow s}_{n + 1} + K(\hat{\overrightarrow s}_{n+1, proj} - \tilde{\overrightarrow s}_{n+1, proj})
  >   $$
  >   显然, 这两个公式在此处得到了统一, 只需$K = (L H_s)_{proj}$即可