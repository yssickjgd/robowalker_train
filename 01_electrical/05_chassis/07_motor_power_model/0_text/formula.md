一元函数Taylor展开
$$
\begin{align}

    f(x) & = \sum_{i=0}^{\infin} \frac{1}{i!}((x-a)\frac{d}{dx})^i f(a) \\
    
     & = \sum_{i=0}^{n} \frac{1}{i!}(x-a)^i \left . \frac{d}{dx} \right|_{x=a} + o((x-a)^n) \\
     
     & = \sum_{i=0}^{n} \frac{1}{i!}(x-a)^i f^{(n)}(a) + o((x-a)^n)
     
\end{align}
$$
二阶Maclaurin展开
$$
f(x) = f(0) + f'(0) x + \frac{1}{2} f''(0) x^2 + o(x^2)
$$
多元函数
$$
f(\overrightarrow{x}) = \sum_{i=0}^{\infin} \frac{1}{i!}((\overrightarrow{x} - \overrightarrow{a})^\mathrm{T} \overrightarrow{\nabla})^i f(\overrightarrow{a}) \\
$$
二阶Maclaurin展开
$$
f(x_1, x_2) = f(0, 0) + f_{x}'(0, 0) x + f_{y}'(0, 0) y + \frac{1}{2} f_{xx}''(0, 0) x^2 + f_{xy}''(0, 0) xy + \frac{1}{2} f_{yy}''(0, 0) y^2 + o(x^2) + o(xy) + o(y^2)
$$
最小二乘问题
$$
已知一系列的点(\overrightarrow{x}_0, y_0), (\overrightarrow{x}_1, y_1), ..., (\overrightarrow{x}_n, y_n) \\

一个除了系数之外,剩下都已知的映射关系f \\

也就是, y = f(\overrightarrow{x}; \overrightarrow{k}) \\

我们要求解这个\overrightarrow{k}, 使得f能很好描述\overrightarrow{x}和y之间的关系
$$
