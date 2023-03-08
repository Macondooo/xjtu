import pandas as pd
import numpy as np
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import random
from scipy.spatial.distance import cdist

plt.style.use('seaborn')
# 显示汉字 SimHei黑体，STsong 华文宋体还有font.style  font.size等
plt.rcParams['font.family'] = 'STsong'
plt.rcParams['axes.unicode_minus'] = False


def Hausdorff_distance(X1, X2):
    # 计算集合每两点之间的欧式距离
    dis = cdist(X1, X2, metric='euclidean')
    # 求出每一列的最小值，即min{d(a,b)}
    temp1 = np.min(dis, axis=1)
    # 求出每一行的最大值，即max{min{d(a,b)}}
    temp2 = np.max(temp1, axis=0)
    return temp2


# 定义数据集
data = pd.read_excel("Cluster输入.xlsx", sheet_name="高钾")
col = data.columns.values.tolist()
col.remove('表面风化')
col.remove('采样类型')
col.remove('文物采样点')
# 获得待聚类的数据
X = data[col].copy()

# 对数据进行直接聚类的结果
model_origin = KMeans(n_clusters=3)
model_origin.fit(X)
# 为每个示例分配一个集群
yhat_origin = model_origin.predict(X)
X['cluster'] = yhat_origin

instability = []
for sigma in np.arange(0.01, 5, 0.1):
    # 给数据添加高斯噪声，以检验分类模型的灵敏度
    X_noise = X.copy()
    # 未每一个数据添加高斯噪声
    for u in col:
        for i in range(X_noise.shape[0]):
            mu = 0
            X_noise[u][i] += random.gauss(mu, sigma)

    model_noise = KMeans(n_clusters=3)
    model_noise.fit(X_noise)
    # 为每个示例分配一个集群
    yhat_noise = model_noise.predict(X_noise)
    X_noise['cluster'] = yhat_noise

    # 求出最小的Hausdorff距离，即求出了分类最为接近的点集，将所有集合间两两最小的Hausdorff距离相加，即可以评价两个聚类模型的相似性
    dis = []
    for i in range(3):
        select_rows_origin = [c for c in X.index if X['cluster'][c] == i]
        select_cluster_origin = np.array(X.loc[select_rows_origin, :])
        line = []
        for j in range(3):
            select_rows_noise = [
                c for c in X_noise.index if X_noise['cluster'][c] == j]
            select_cluster_noise = np.array(X_noise.loc[select_rows_noise, :])
            d = Hausdorff_distance(select_cluster_noise, select_cluster_origin)
            line.append(d)
        dis.append(line)

    # 找到距离最近的集合
    temp = np.min(dis, axis=1)
    # 将最近的集合距离进行相加
    instability.append(np.sum(temp))

# 求出原集合cluster彼此的最近距离
dis_ = []
for i in range(3):
    select_rows = [c for c in X.index if X['cluster'][c] == i]
    select_cluster = np.array(X.loc[select_rows, :])
    for j in range(i+1, 3):
        select_rows_ = [c for c in X.index if X['cluster'][c] == j]
        select_cluster_ = np.array(X.loc[select_rows_, :])
        d = Hausdorff_distance(select_cluster_, select_cluster)
        dis_.append(d)

min_dis_ = np.min(dis_)

# 做出sigma和Instab的关系，并且最后将分界线min_dis_画在图上
plt.plot(np.arange(0.01, 5, 0.1), instability)
line1 = plt.axhline(min_dis_, linestyle='--')
plt.legend(handles=[line1], labels=[r'$H_d$'], loc="upper left",
           fontsize=15)
plt.xlabel(r'$\sigma$')
plt.ylabel(r'$Instab$', rotation=0, labelpad=20)
plt.show()

print(min_dis_)
