import numpy as np
import matplotlib.pyplot as plt
import math

gama = [ 0.5, 1, 2, 5]  # 10
x1_bound = [ 0.75, 1.3, 3, 7]  # 12
x2_bound = [ 1.5, 1.3, 3, 3.5]  # 4
for k in range(4):
    # 建立步长为0.01，即每隔0.01取一个点
    step = 0.01
    x1 = np.arange(-x1_bound[k], x1_bound[k], step)
    x2 = np.arange(-x2_bound[k], x2_bound[k], step)

    # 将原始数据变成网格数据形式
    X, Y = np.meshgrid(x1, x2)
    Z = 0.5*(X**2+gama[k]*(Y**2))

    # 画出5条线，并将颜色设置为黑色
    ax = plt.subplot(2, 2, k+1)
    contour = plt.contour(X, Y, Z, 5, colors='k', linestyles='dashed')
    # 等高线上标明z（即高度）的值，字体大小是10，颜色分别是黑色和红色
    plt.clabel(contour, fontsize=10, colors='k')
    # 设置标题
    ax.set_title('$\gamma=$'+str(gama[k]))

    # 迭代次数N
    e = 1E-10
    if gama[k] == 1:
        N = 1
    else:
        if gama[k] > 1:
            N = math.ceil(
                math.log(0.5*(pow(gama[k], 2)+gama[k])/e)/math.log(gama[k]/(gama[k]-1)))
        else:
            N = math.ceil(
                math.log(0.5*(pow(gama[k], 2)+gama[k])/e)/math.log(1/(1-gama[k])))
    x_ = [((gama[k]-1)/(gama[k]+1))**i*gama[k] for i in range(N+1)]
    y_ = [((gama[k]-1)/(gama[k]+1))**i*(-1)**i for i in range(N+1)]
    plt.plot(x_, y_, 'go--', linewidth=1, markersize=3)

plt.subplots_adjust(left=None, bottom=None, right=None,
                    top=None, wspace=None, hspace=0.5)

plt.show()
