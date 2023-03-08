import numpy as np
import matplotlib.pyplot as plt
import math

gama = 0.01  # 10
x1_bound = 0.015
x2_bound = 1


step = 0.001
x1 = np.arange(-x1_bound, x1_bound, step)
x2 = np.arange(-x2_bound, x2_bound, step)

# 将原始数据变成网格数据形式
X, Y = np.meshgrid(x1, x2)
Z = 0.5*(X**2+gama*(Y**2))

# 画出8条线，并将颜色设置为黑色
contour = plt.contour(X, Y, Z, 8, colors='k', linestyles='dashed')
# 等高线上标明z（即高度）的值，字体大小是10，颜色分别是黑色和红色
plt.clabel(contour, fontsize=10, colors='k')
# 设置标题
plt.title('$\gamma=$'+str(gama))

# 迭代次数N
e = 1E-10
if gama == 1:
    N = 1
else:
    if gama > 1:
        N = math.ceil(
            math.log(0.5*(pow(gama, 2)+gama)/e)/math.log(gama/(gama-1)))
    else:
        N = math.ceil(
            math.log(0.5*(pow(gama, 2)+gama)/e)/math.log(1/(1-gama)))

x_ = [((gama-1)/(gama+1))**i*gama for i in range(N+1)]
y_ = [((gama-1)/(gama+1))**i*(-1)**i for i in range(N+1)]
plt.plot(x_, y_, 'go--', linewidth=1, markersize=3)
plt.ylim(-1.01,1.01)
plt.show()
