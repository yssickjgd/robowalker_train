import numpy as np
import pandas as pd
from scipy.optimize import curve_fit

# 定义要拟合的非线性函数
def func_power(data, k0, k1, k2, k3, k4, k5):
    I, ω = data
    return (k0 + k1 * I + k2 * ω + k3 * I * ω + k4 * I**2 + k5 * ω**2)

# 读取数据
array_data = pd.read_csv('3508_power.csv').values

# 提取数据
array_power_data = array_data[:, 0]
array_I_data = array_data[:, 1]
array_ω_data = array_data[:, 2]

# 使用 curve_fit 进行最小二乘拟合
params, covariance = curve_fit(func_power, (array_I_data, array_ω_data), array_power_data, p0=[1, 1, 1, 1, 1, 1])

# 获取拟合的参数
k0, k1, k2, k3, k4, k5 = params

# 输出拟合的参数
print('k0:', k0)
print('k1:', k1)
print('k2:', k2)
print('k3:', k3)
print('k4:', k4)
print('k5:', k5)