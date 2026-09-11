# Kalman滤波器之矩阵分析基础

## 1 矩阵的运算与逆

- 本小节内容为复习线性代数知识
- 先复习与约定一下符号
  - 共识
    - 大小写与箭头, 指示该量的类型, 带括号的下标表明对该量的处理方式
    - 原则上, 一个字母仅表示一个标量, 向量或矩阵
    - 标量是特殊的向量, 向量是特殊的矩阵
    - 所有的取子元素的编号, 都是从1开始
  - 各种量的表示
    - 标量, 小写字母: $a, x$
    - 向量, 带箭头的小写字母: $\overrightarrow{a}, \overrightarrow{x}$, 且默认都是列向量
    - 矩阵, 大写字母: $A, B$
    - 向量的某个元素: $\overrightarrow a_{(1)}$, 无歧义时, 亦可简化为$a_1$
    - 矩阵的某个子向量
      - 列向量: $A_{(,2)}$, 无歧义时, 亦可简化为$\overrightarrow{a}_1$
      - 行向量: $B_{(2,)}$
    - 矩阵的某个元素
      - $A_{(3,1)}$, 无歧义时, 亦可简化为$a_{(3,1)}, a_{3, 1}$等
  - 标量是特殊的向量
  - 标量和向量都是特殊的矩阵
- 矩阵的运算
  - 数乘: $(kA)_{(i, j)} = kA_{(i, j)}$
  - 转置: $A^T_{(i, j)} = A_{(j, i)}$
  - 方阵的迹: $\mathrm{tr} (A) = \sum_{k=1}^{n} A_{(k, k)}$
  - 加法: $(A + B)_{(i, j)} = A_{(i, j)} + B_{(i, j)}$
  - 乘法: $(AB)_{(i, j)} = \sum_{k=1}^{n} A_{(i, k)} * B_{(k, j)}$, 其中$A$为$m \times n$, $B$为$n \times p$
  - 同型矩阵的内积: $<A, B> = \mathrm{tr}({A^TB})$
  - 满秩方阵的逆
  - 方阵的行列式
  - 秩
- 性质不再一一赘述, 自行复习即可

## 2 伪逆矩阵

<a id="结论1"></a>

**结论1:**

**设矩阵$A$的形状为$m \times n$, 且矩阵A列满秩, 即$m \ge n$. 则有如下结论:** 

1. **存在矩阵$X$, 使得$XA = I$**
2. **当$X=(A^TA)^{-1}A^T$时, $XA = I$成立**

**其中, $X$作为左乘的矩阵, 因此$X$也叫作$A$的左逆, 记作$A^{-left}$**

> 证明:
>
> 显然, 只要证明第二条, 第一条就连带证明出来了
>
> 因此, 接下来直接证明第二条
>
> 要证明第二条, 则需要拆分任务
>
> 1. 证明$A^TA$可逆, 也就意味着$X$是可以合法求出来的
> 2. 计算$XA$, 验证是否为$I$
>
> 对于第一步, 只需要证明以$A^TA$为系数矩阵的线性方程组只有零解即可
>
> 我们设关于$\overrightarrow{b}$的线性方程组
> $$
> (A^TA)\overrightarrow{b} = \overrightarrow 0
> $$
> 两边左乘$\overrightarrow{b} ^T$, 有
> $$
> \overrightarrow{b} ^T(A^TA)\overrightarrow{b} = \overrightarrow 0
> $$
> 根据结合律, 我们可以调整顺序, 有
> $$
> (\overrightarrow{b} ^T A^T) (A \overrightarrow{b}) = \overrightarrow 0
> $$
> 根据矩阵乘法与转置公式, 有
> $$
> (A \overrightarrow{b})^T (A \overrightarrow{b}) = \overrightarrow 0
> $$
> 根据向量模长公式, 有
> $$
> ||A \overrightarrow{b}|| = 0
> $$
> 向量模长为0, 说明向量为零向量, 也就是
> $$
> A \overrightarrow{b} = \overrightarrow 0
> $$
> 由于$A$是列满秩矩阵, 也就意味着列向量组线性无关, 线性方程组$A \overrightarrow{b} = \overrightarrow 0$只有零解, 则必然有
> $$
> \overrightarrow{b} = \overrightarrow 0
> $$
> 回过头来, 这也说明了关于$\overrightarrow{b}$的线性方程组$(A^TA)\overrightarrow{b} = \overrightarrow 0$只有零解, 也就是$A^TA$是可逆的
>
> 既然$A^TA$可逆, 说明$(A^TA)^{-1}A^T$是可以合法求出来的, 不妨令
> $$
> X=(A^TA)^{-1}A^T
> $$
> 代入到$XA$, 有
> $$
> \begin{align}
> 
> 	XA & = (A^TA)^{-1}A^TA \\
> 	
> 	& = (A^TA)^{-1}(A^TA) \\
> 	
> 	& = I
> 	
> \end{align}
> $$
> 因此, 当$X=(A^TA)^{-1}A^T$时, $XA = I$成立

左逆矩阵有了, 右逆矩阵也得安排上

<a id="结论2"></a>

**结论2:**

**设矩阵$A$的形状为$m \times n$, 且矩阵A行满秩. 则有如下结论:** 

1. **存在矩阵$X$, 使得$AX = I$**
2. **当$X=A^T(AA^T)^{-1}$时, $AX = I$成立**

**其中, $X$作为右乘的矩阵, 因此$X$也叫作$A$的右逆, 记作$A^{-right}$**

> 此处证明与上一个结论同理, 自行证明即可

**练习1:**

**根据麦克纳姆轮底盘运动学逆解算的公式, 写出其运动学正解算的公式**

> 根据逆解算公式
> $$
> \frac{1}{s} \begin{pmatrix}
>     -1 & 1 & a+b \\
>     -1 & -1 & a+b \\
>     1 & -1 & a+b \\
>     1 & 1 & a+b \\
> \end{pmatrix} \begin{pmatrix}
>     v_x \\
>     v_y \\
>     \omega \\
> \end{pmatrix} = \begin{pmatrix}
> 	\omega_{0} \\
> 	\omega_{1} \\
> 	\omega_{2} \\
> 	\omega_{3} \\
> \end{pmatrix}
> $$
> 我们可以分析发现, 其相当于将整个底盘的目标速度向量, 左乘一个矩阵$A$, 从而得到的每个轮组的转速向量, 即
> $$
> A \overrightarrow{v_{chassis}} = \overrightarrow{\omega_{wheel}}
> $$
> 其中
> $$
> A = \frac{1}{s} \begin{pmatrix}
>     -1 & 1 & a+b \\
>     -1 & -1 & a+b \\
>     1 & -1 & a+b \\
>     1 & 1 & a+b \\
> \end{pmatrix}
> $$
> 显然, 该矩阵是一个列满秩的矩阵, 我们直接代入[结论1](#结论1), 有
> $$
> \begin{align}
> 
> 	A^{-left} & = (A^TA)^{-1}A^T \\
>     
>     & = \frac{s}{4} \begin{pmatrix}
>         -1 & -1 & 1 & 1 \\
>         1 & -1 & -1 & 1 \\
>         \frac{1}{a + b} & \frac{1}{a + b} & \frac{1}{a + b} & \frac{1}{a + b} \\
>     \end{pmatrix}
> 
> \end{align}
> $$

## 3 向量与矩阵函数及其导数

再复习一下数学分析知识

单变量函数的求导
$$
\frac{d}{dx}(f(x))
$$
其中, 我刻意将被求导的函数单独拎出来, 没有放到分子处, 写成了如上所示的形式

从这一视角来看, 我们可以认为求导是一种==算子==, 构成了从一个函数集合到另一个函数集合的映射关系

> 函数: 将, 一个数的集合, 映射到, 另一个数的集合
>
> 泛函: 将, 一个函数的集合, 映射到 ,一个数的集合
>
> 算子: 将, 一个函数的集合, 映射到, 另一个函数的集合

而后, 我们还要承认, 向量和矩阵也是可以作为函数的自变量, 当然也可以作为函数的因变量

> 我们标量就是一个数, 向量是一维延展的, 矩阵是二维延展的, 我们有没有更高阶的维度对应的量呢?
>
> 显然是有的! 更高维度延展的量统称为张量

为对学习者友好, ==本教程暂不涉及泛函分析和张量分析相关内容==, 下文各种表达形式也仅作形式化展示, 不作证明. 感兴趣可以直接去系统学习泛函分析和张量分析

我们继续思考, 我们将标量, 向量, 矩阵两两组合, 能形成9种函数. 而求导需要知道每个自变量和每个因变量之间的变化影响关系

这些关系的表示方法如下表所述

| 纵轴自变量<br />横轴因变量<br />表格内容是导数的形式 | 标量             | 向量             | 矩阵             |
| ---------------------------------------------------- | ---------------- | ---------------- | ---------------- |
| 标量                                                 | 标量 ( 0维张量 ) | 向量 ( 1维张量 ) | 矩阵 ( 2维张量 ) |
| 向量                                                 | 向量( 1维张量 )  | 矩阵 ( 2维张量 ) | 3维张量          |
| 矩阵                                                 | 矩阵 ( 2维张量 ) | 3维张量          | 4维张量          |

对于具体的公式, 其核心只要掌握一条: **固定其它自变量, 研究某一个自变量的变化对每一个因变量的作用效果**

> 剩下的都是形式化推导的工作, 只要逻辑自洽
>
> 如若后文理解困难, 可以直接记结论, 在推导Kalman滤波器公式时, 代入即可

### 3.1 n维向量到标量的映射

基于标量到标量的传统映射, 我们将函数的自变量由标量增广为$n$维向量, 形式化地表示为
$$
\frac{d}{d\overrightarrow{x}}(f(\overrightarrow{x})) = \begin{pmatrix}

	\frac{\partial}{\partial x_1}(f(x)) \\
	
	...\\

	\frac{\partial}{\partial x_n}(f(x)) \\

\end{pmatrix}
$$

> 由于$\frac{d}{d\overrightarrow{x}}$表示过于繁琐, 因此后续将该表示简化为$D_{\overrightarrow x}$

顺理成章地, 我们的增量公式会有
$$
f(\overrightarrow{x} + \overrightarrow{h}) = f(\overrightarrow{x}) + <\mathrm D_{\overrightarrow{x}}(f(\overrightarrow{x})), \overrightarrow{h}>
$$

### 3.2 标量到m维向量的映射

基于标量到标量的传统映射, 我们再将函数的因变量由标量增广为$m$维向量, 形式化地表示为
$$
D_x(\overrightarrow f(x)) = \begin{pmatrix}

	\frac{d}{d x}(f_1(x)) \\
	
	...\\

	\frac{d}{d x}(f_m(x)) \\

\end{pmatrix}
$$
顺理成章地, 我们的增量公式会有
$$
\overrightarrow f(x+h) = \overrightarrow f(x) + \mathrm D_{x}(\overrightarrow f(x))h
$$

### 3.3 n维向量到m维向量的映射

在前两者的基础上, 我们结合一下, 形式化地表示为
$$
\mathrm D_{\overrightarrow{x}}(\overrightarrow{f}(\overrightarrow{x})) = \begin{pmatrix}

	\frac{\partial}{\partial x_1}(f_1(\overrightarrow{x})) & ... & \frac{\partial}{\partial x_n}(f_1(\overrightarrow{x})) \\

	... \\

	\frac{\partial}{\partial x_1}(f_m(\overrightarrow{x})) & ... & \frac{\partial}{\partial x_n}(f_m(\overrightarrow{x})) \\

\end{pmatrix}
$$

> 这个矩阵其实就是数学分析下册时所学过的Jacobi矩阵

顺理成章地, 我们的增量公式会有
$$
\overrightarrow f(\overrightarrow{x} + \overrightarrow{h}) = \overrightarrow f(\overrightarrow{x}) + \mathrm D_{\overrightarrow{x}}(\overrightarrow f(\overrightarrow{x}))\overrightarrow{h}
$$

> $m\times n$矩阵$A$对一个$n$维向量$\overrightarrow{x}$的乘法, 就是标题的这种映射, 其Jacobi矩阵就是$A$

### 3.4 m*n维矩阵到标量的映射

基于n维向量到标量的映射, 我们不难得出
$$
D_X(f(X)) = \begin{pmatrix}

	\frac{\partial}{\partial x_{1,1}}(f(X))& ... & \frac{\partial}{\partial x_{1,n}}(f(X)) \\
	
	... \\

	\frac{\partial}{\partial x_{m,1}}(f(X))& ... & \frac{\partial}{\partial x_{m,n}}(f(X)) \\

\end{pmatrix}
$$
在这种表示下, 我们的增量公式会有
$$
f(X + H) = f(X) + <\mathrm D_{X}(f(X)), H>
$$

### 3.5 标量到m*n维矩阵的映射

基于标量到m维向量的映射, 我们不难得出
$$
D_x(F(x)) = \begin{pmatrix}

	\frac{d}{d x}(f_{1,1}(x))& ... & \frac{d}{d x}(f_{1,n}(x)) \\
	
	... \\

	\frac{d}{d x}(f_{m,1}(x))& ... & \frac{d}{d x}(f_{m,n}(x)) \\

\end{pmatrix}
$$
在这种表示下, 我们的增量公式会有
$$
F(x+h) = F(x) + D_x(F(x)) h
$$

### 3.6 小结

在向量和矩阵, 乃至更高阶的张量函数中, 标量函数具有的性质仍旧有效的, 但具体表现形式较为复杂, 此处不作赘述

- 线性
- 链式法则
- ......

在文中末尾, 我们摆出在Kalman滤波中常用的向量求导公式, 感兴趣同学自行证明
$$
\begin{align}

    D_X(\mathrm{tr}(XAX^T)) & = X(A+A^T) \\

    D_X(\mathrm{tr}(AXB)) & = A^TB^T \\

\end{align}
$$