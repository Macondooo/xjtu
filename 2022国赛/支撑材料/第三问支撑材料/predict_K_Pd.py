import joblib
import pandas as pd
import numpy as np

# plt.style.use('seaborn')
# # 显示汉字 SimHei黑体，STsong 华文宋体还有font.style  font.size等
# plt.rcParams['font.family'] = 'STsong'
# plt.rcParams['axes.unicode_minus'] = False

# 调用保存好的模型(训练好的)去做预测
model = joblib.load("SVM.pkl")
data = pd.read_excel('表单3.xlsx',sheet_name='归一化的表单3')
#获得待分析的数据
col = data.columns.values.tolist()
col.remove('文物编号')
col.remove('表面风化')
X = data[col].copy()

y_predict = model.predict(np.array(X))
print(y_predict)
