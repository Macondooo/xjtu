import joblib
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import random
from sklearn.metrics import accuracy_score

plt.style.use('seaborn')
# 显示汉字 SimHei黑体，STsong 华文宋体还有font.style  font.size等
plt.rcParams['font.family'] = 'STsong'
plt.rcParams['axes.unicode_minus'] = False
random.seed(61)

# 调用保存好的模型(训练好的)去做预测
model_K_Pd = joblib.load("SVM.pkl")
model_K = joblib.load('ClusterK.pkl')
model_Pd = joblib.load('ClusterPd.pkl')
data1 = pd.read_excel('表单3.xlsx', sheet_name='归一化的表单3')
data2 = pd.read_excel('表单3.xlsx', sheet_name='高钾类玻璃')
data3 = pd.read_excel('表单3.xlsx', sheet_name='铅钡类玻璃')
# 删除无关数据的列
col1 = data1.columns.values.tolist()
col1.remove('文物编号')
col1.remove('表面风化')
col2 = ['二氧化硅(SiO2)', '氧化钾(K2O)']
col3 = ['二氧化硅(SiO2)', '氧化铅(PbO)', '氧化钡(BaO)']
# 拷贝待分析的数据
X1 = data1[col1].copy()  # X1为分为高钾和铅钡类玻璃所需要的数据
X2 = data2[col2].copy()  # X2为高钾玻璃分亚类所需要的数据
X3 = data3[col3].copy()  # X3为铅钡玻璃分亚类所需要的数据

y1_pred = model_K_Pd.predict(np.array(X1))
y2_pred = model_K.predict(X2)
y3_pred = model_Pd.predict(X3)


sensitivity1 = []
sensitivity2 = []
sensitivity3 = []


for sigma in np.arange(0.01, 10, 0.1):
    # 给数据添加高斯噪声，以检验分类模型的灵敏度
    X1_noise = X1.copy()
    # 为每一个数据添加高斯噪声
    for u in col1:
        for i in range(X1_noise.shape[0]):
            X1_noise[u][i] += random.gauss(0, sigma)
    # 为每个示例分配一个集群
    y1_pred_noise = model_K_Pd.predict(np.array(X1_noise))
    # 计算加入噪声后的预测准确率
    acc1 = accuracy_score(y1_pred, y1_pred_noise)
    sensitivity1.append(acc1)


for sigma in np.arange(0.01, 5, 0.1):
    # 给数据添加高斯噪声，以检验分类模型的灵敏度
    X2_noise = X2.copy()
    X3_noise = X3.copy()
    # 为每一个数据添加高斯噪声
    for u in col2:
        for i in range(X2_noise.shape[0]):
            X2_noise[u][i] += random.gauss(0, sigma)
    for u in col3:
        for i in range(X3_noise.shape[0]):
            X3_noise[u][i] += random.gauss(0, sigma)

    # 为每个示例分配一个集群
    y2_pred_noise = model_K.predict(X2_noise)
    y3_pred_noise = model_Pd.predict(X3_noise)

    # 计算加入噪声后的预测准确率
    acc2 = accuracy_score(y2_pred, y2_pred_noise)
    acc3 = accuracy_score(y3_pred, y3_pred_noise)
    sensitivity2.append(acc2)
    sensitivity3.append(acc3)

# 开始绘图
plt.figure()
x = np.arange(0.01, 10, 0.1)
ax1, = plt.plot(x, sensitivity1,'#1f77b4')
plt.legend(handles=[ax1], labels=[r'$Sensitivity$'], loc="best",
           fontsize=15)
plt.xlabel(r'$\sigma$', fontsize=15)
plt.ylabel(r'$Accuracy$', fontsize=15)
# 亚类画做一张图
plt.figure()
x1 = np.arange(0.01, 5, 0.1)
ax2, = plt.plot(x1, sensitivity2,'r',linestyle='--')
ax3, = plt.plot(x1, sensitivity3,'#1f77b4')
plt.legend(handles=[ax2, ax3], labels=[r'$Sensitivity\_K$', r'$Sensitivity\_Pb$'], loc="best",
           fontsize=15)
plt.xlabel(r'$\sigma$', fontsize=15)
plt.ylabel(r'$Accuracy$',  labelpad=20, fontsize=15)
plt.show()
