# 最小二乘问题求解过程

## 1 明确问题

​		已知一系列的点$(\overrightarrow{x}_0, y_0), (\overrightarrow{x}_1, y_1), ..., (\overrightarrow{x}_n, y_n)$, 还有一个除了系数$\overrightarrow{k}$之外, 剩下都已知的映射关系, 或者说估计方法$f(\overrightarrow{x}; \overrightarrow{k})$

​		我们要求解$\overrightarrow{k}$的具体值, 使得估计方法$f$能很好描述$\overrightarrow{x}$和$y$之间的关系

​		在解决问题之前, 我们先定义一个符号$\hat{y}$, 即$\hat{y} = f(\overrightarrow{x}; \overrightarrow{k})$

## 2 解决方法

​		我们接下来就要想, 什么样的待定系数$\overrightarrow{k}$是“好”的

​		很自然地想到，我们可以测量根据任意已知点的$\overrightarrow{x}$估计出来的$\hat{y}$到已知点本身的$y$之间的距离$\hat{y}-y$. 这个距离是有正负号区分的. 但实际上, 对于这种偏差, 大了小了都是偏, 过犹不及

​		因此, 我们可以选择取绝对值, 也就是用$|\hat{y}-y|$衡量这个距离. 然而, 绝对值是有问题的, 问题就在于它在$\hat{y}-y = 0$时不可导, 容易造成奇怪的错误

​		再进一步, 我们想到了平方, 也就是用$(\hat{y}-y)^2$衡量这个距离. 平方是一个很美妙的方法, 只要$f$可导, 那么这个距离就也是可导的

​		因此, 我们现在解决的问题就变成了
$$
\underset{\overrightarrow{k}}{\arg \min} \sum_{i=0}^{n}(\hat{y_i} - y_i)^2
$$
​		进一步代入, 也就是
$$
\underset{\overrightarrow{k}}{\arg \min} \sum_{i=0}^{n}(f(\overrightarrow{x}_i; \overrightarrow{k}) - y_i)^2
$$
​		利用数学分析下册学过的Lagrange乘数法, 即可轻松解决. 同学们也可以指定$f(\overrightarrow{x}; \overrightarrow{k}) = k_0 + k_1 x$来求解高中就学过的一次函数最小二乘法公式