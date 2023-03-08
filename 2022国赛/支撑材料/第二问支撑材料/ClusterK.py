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
data=pd.read_excel("Cluster输入.xlsx",sheet_name="高钾")
col = data.columns.values.tolist()
col.remove('表面风化')
col.remove('采样类型')
col.remove('文物采样点')
#获得待聚类的数据
X = data[col].copy()
#plt.scatter(X[col[0]],X[col[1]])
#plt.show()

sse = []
for k in range(1,9):
    estimator = KMeans(n_clusters=k)
    estimator.fit(X)
    sse.append(estimator.inertia_)
fig,ax = plt.subplots()
ax.plot(np.arange(1,9),sse,marker='o')
plt.xlabel('分类数目')
plt.ylabel('SSE')
plt.show()

model = KMeans(n_clusters=3)
model.fit(X)
# 为每个示例分配一个集群
yhat = model.predict(X)
plt.scatter(X[col[0]],X[col[1]],c=yhat,cmap='Dark2')
plt.xlabel('二氧化硅(SiO2)')
plt.ylabel('氧化钾(K2O)')
label = data['文物采样点'].tolist()
for i in range(len(label)):
    plt.text(X[col[0]][i], X[col[1]][i], label[i],fontsize = 10)
plt.show()

data.insert(1,'类别',yhat)
#写入文件i
pd.DataFrame(data.to_excel("高钾类玻璃聚类结果.xlsx"))

# 保存训练好的模型
joblib.dump(model,'ClusterK.pkl')