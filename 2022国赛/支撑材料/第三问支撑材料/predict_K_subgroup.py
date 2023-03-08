import joblib
import pandas as pd

# 调用保存好的模型(训练好的)去做预w
model = joblib.load("ClusterK.pkl")
data = pd.read_excel('表单3.xlsx',sheet_name='高钾类玻璃')
#获得待分析的数据
col = data.columns.values.tolist()
col.remove('文物编号')
col.remove('表面风化')
X = data[col].copy()

y_predict = model.predict(X)
print(y_predict)
