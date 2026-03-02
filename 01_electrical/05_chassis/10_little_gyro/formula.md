云台相对于底盘的旋转矩阵
$$
{^CG} = Rz(\theta)
$$
坐标变换
$$
\begin{align}

	{^C\overrightarrow{v}} & = {^CG} \ {^G\overrightarrow{v}} \\
	
    & = Rz(\theta) \ {^G\overrightarrow{v}} \\
    
    & = \begin{pmatrix}

        \cos(\theta) & -\sin(\theta) \\

        \sin(\theta) & \cos(\theta) \\
    
    \end{pmatrix} {^G\overrightarrow{v}} \\
	
\end{align}
$$
例题
$$
\begin{align}

	{^C\overrightarrow{v}} & = \begin{pmatrix}

        \cos(\theta) & -\sin(\theta) \\

        \sin(\theta) & \cos(\theta) \\
    
    \end{pmatrix} {^G\overrightarrow{v}} \\
    
    & = \begin{pmatrix}

        \cos(\frac{\pi}{2}) & -\sin(\frac{\pi}{2}) \\

        \sin(\frac{\pi}{2}) & \cos(\frac{\pi}{2}) \\
    
    \end{pmatrix} \begin{pmatrix}

        3 \\

        0 \\
    
    \end{pmatrix} \\
    
    & = \begin{pmatrix}

        0 \\

        3 \\
    
    \end{pmatrix}
	
\end{align}
$$
