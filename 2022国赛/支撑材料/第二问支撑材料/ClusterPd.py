import pandas as pd
import numpy as np
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import os
import joblib

plt.style.use('seaborn')
# 显示汉字 SimHei黑体，STsong 华文宋体还有font.style  font.size等
plt.rcParams['font.family'] = 'STsong'
plt.rcParams['axes.unicode_minus'] = False

# 定义数据集
data = pd.read_excel("Cluster输入.xlsx", sheet_name="铅钡")
col = data.columns.values.tolist()
col.remove('表面风化')
col.remove('采样类型')
col.remove('文物采样点')
# 获得待聚类的数据
X = data[col].copy()

# 使用SSE考虑应该将样本分成几类
sse = []
for k in range(1, 10):
    estimator = KMeans(n_clusters=k)
    estimator.fit(X)
    sse.append(estimator.inertia_)
fig, ax = plt.subplots()
ax.plot(np.arange(1, 10), sse, marker='o')
plt.xlabel('分类数目')
plt.ylabel('SSE')
plt.show()


model = KMeans(n_clusters=5)
model.fit(X)
# 为每个示例分配一个集群
yhat = model.predict(X)

# 开始绘图
plt.figure(figsize=(10, 10))
ax = plt.subplot(projection='3d')  # 创建一个三维的绘图工程
ax.scatter(X[col[0]], X[col[1]], X[col[2]], c=yhat, cmap='Dark2')
ax.set_xlabel('二氧化硅(SiO2)')  # 设置x坐标轴
ax.set_ylabel('氧化铅(PbO)')  # 设置y坐标轴
ax.set_zlabel('氧化钡(BaO)')  # 设置z坐标轴
label = data['文物采样点'].tolist()
for i in range(int(len(label)/2)):
    ax.text(X[col[0]][i]+0.5, X[col[1]][i]+0.5, X[col[2]][i]+2, label[i],fontsize = 10)
for i in range(int(len(label)/2),len(label),1):
    ax.text(X[col[0]][i]-0.5, X[col[1]][i]-0.5, X[col[2]][i]-2, label[i],fontsize = 10)

plt.show()

data.insert(1,'类别',yhat)
#写入文件i

pd.DataFrame(data.to_excel('铅钡类玻璃聚类结果.xlsx'))

# 保存训练好的模型
joblib.dump(model,'ClusterPd.pkl')