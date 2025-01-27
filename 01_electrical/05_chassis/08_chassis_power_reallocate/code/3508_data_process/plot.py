import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


# 定义要绘制的3D函数
def func_power_origin(I, ω):
    # k0: 0.6641993412640775
    # k1: 0.006444284468539646
    # k2: 0.0001423857226262331
    # k3: 0.017644430204543864
    # k4: 0.1650143850678086
    # k5: 3.096721772539512e-05
    return(0.6641993412640775 + 0.006444284468539646 * I + 0.0001423857226262331 * ω + 0.017644430204543864 * I * ω + 0.1650143850678086 * I**2 + 3.096721772539512e-05 * ω**2)


# 绘制3D图, I范围在-20到20, ω范围在-600到600
def plot_3d():
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    I = np.linspace(-20, 20, 100)
    ω = np.linspace(-600, 600, 100)
    I, ω = np.meshgrid(I, ω)
    Z = func_power_origin(I, ω)
    ax.plot_surface(I, ω, Z, rstride=1, cstride=1, cmap='rainbow')
    plt.show()

# 绘制2D图, I范围在-20到20, ω由传参给定
def plot_2d(ω):
    I = np.linspace(-20, 20, 100)
    Z = func_power_origin(I, ω)
    plt.plot(I, Z)
    plt.show()

plot_3d()
# plot_2d(100)