import pandas as pd
from sklearn.svm import SVC
from matplotlib import pyplot as plt
import numpy as np  
from sklearn.model_selection import train_test_split
import joblib


# 从文件中读取数据
data = pd.read_excel("SVM输入.xlsx")
# 将读取的data划分为x,y;
col = data.columns.values.tolist()
col.remove('类型')
X = np.array(data[col].copy())
y = np.array(data['类型'])
#将数据随机的划分为测试集和训练集
train_X,test_X, train_y, test_y = train_test_split(X,y,test_size = 0.2,random_state = 0)
# 进行分类器的训练
svc = SVC(kernel='linear')
svc.fit(train_X, train_y)
print('正确率')
print(svc.score(test_X,test_y))
pred_y=svc.predict(test_X)

# from sklearn.metrics import roc_curve
# import matplotlib.pyplot as plt
 
# ## 求出ROC曲线的x轴和y轴
# fpr, tpr, thresholds = roc_curve(test_y,pred_y)
# #绘制ROC曲线
# plt.figure(figsize=(10,6))
# plt.xlim(0,1)     ##设定x轴的范围
# plt.ylim(0.0,1.1) ## 设定y轴的范围
# plt.xlabel('False Postive Rate')
# plt.ylabel('True Postive Rate')
# plt.plot(fpr,tpr,linewidth=2, linestyle="-",color='red')
# plt.show()

#获得线性分类平面的方程,WX+b=0
w = svc.coef_  
b = svc.intercept_
print("超平面方程")
print(w)
print(b)

# 保存训练好的模型
joblib.dump(svc,'SVM.pkl')

