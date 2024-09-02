

# 舵轮

$$
\frac{1}{s} \begin{pmatrix}
    1 & 0 & -\frac{\sqrt{2}}{2}r \\
    0 & 1 & \frac{\sqrt{2}}{2}r \\
    1 & 0 & -\frac{\sqrt{2}}{2}r \\
    0 & 1 & \frac{\sqrt{2}}{2}r \\
    1 & 0 & -\frac{\sqrt{2}}{2}r \\
    0 & 1 & \frac{\sqrt{2}}{2}r \\
    1 & 0 & -\frac{\sqrt{2}}{2}r \\
    0 & 1 & \frac{\sqrt{2}}{2}r \\
\end{pmatrix} \begin{pmatrix}
    v_x \\
    v_y \\
    \omega \\
\end{pmatrix} = \begin{pmatrix}
	\omega_{0x} \\
	\omega_{0y} \\
	\omega_{1x} \\
	\omega_{1y} \\
	\omega_{2x} \\
	\omega_{2y} \\
	\omega_{3x} \\
	\omega_{3y} \\
\end{pmatrix}
$$

运动学解算公式
$$
\frac{s}{4}\begin{pmatrix}
    1 & 0 & 1 & 0 & 1 & 0 & 1 & 0 \\
    0 & 1 & 0 & 1 & 0 & 1 & 0 & 1 \\
    -\frac{\sqrt{2}}{2 r} & \frac{\sqrt{2}}{2 r} & -\frac{\sqrt{2}}{2 r} & -\frac{\sqrt{2}}{2 r} & \frac{\sqrt{2}}{2 r} & -\frac{\sqrt{2}}{2 r} & \frac{\sqrt{2}}{2 r} & \frac{\sqrt{2}}{2 r} \\
\end{pmatrix}
$$


# 全向轮

$$
\frac{1}{s} \begin{pmatrix}
    -\frac{\sqrt{2}}{2} & \frac{\sqrt{2}}{2} & r \\
    -\frac{\sqrt{2}}{2} & -\frac{\sqrt{2}}{2} & r \\
    \frac{\sqrt{2}}{2} & -\frac{\sqrt{2}}{2} & r \\
    \frac{\sqrt{2}}{2} & \frac{\sqrt{2}}{2} & r \\
\end{pmatrix} \begin{pmatrix}
    v_x \\
    v_y \\
    \omega \\
\end{pmatrix} = \begin{pmatrix}
	\omega_{0} \\
	\omega_{1} \\
	\omega_{2} \\
	\omega_{3} \\
\end{pmatrix}
$$

运动学解算公式
$$
\frac{s}{4}\begin{pmatrix}
    -\sqrt{2} & -\sqrt{2} & \sqrt{2} & \sqrt{2} \\
    \sqrt{2} & -\sqrt{2} & -\sqrt{2} & \sqrt{2} \\
    \frac{1}{r} & \frac{1}{r} & \frac{1}{r} & \frac{1}{r} \\
\end{pmatrix}
$$

# 麦轮

$$
\frac{1}{s} \begin{pmatrix}
    -1 & 1 & a+b \\
    -1 & -1 & a+b \\
    1 & -1 & a+b \\
    1 & 1 & a+b \\
\end{pmatrix} \begin{pmatrix}
    v_x \\
    v_y \\
    \omega \\
\end{pmatrix} = \begin{pmatrix}
	\omega_{0} \\
	\omega_{1} \\
	\omega_{2} \\
	\omega_{3} \\
\end{pmatrix}
$$

运动学解算公式
$$
\frac{s}{4}\begin{pmatrix}
    -1 & -1 & 1 & 1 \\
    1 & -1 & -1 & 1 \\
    \frac{1}{a + b} & \frac{1}{a + b} & \frac{1}{a + b} & \frac{1}{a + b} \\
\end{pmatrix}
$$


# 通用

$$
A \overrightarrow{v_{chassis}} = \overrightarrow{\omega_{wheel}}
$$

$$
B A \overrightarrow{v_{chassis}} = \overrightarrow{v_{chassis}} = B \overrightarrow{\omega_{wheel}}
$$

