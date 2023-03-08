from calendar import c
import numpy as np
import pandas as pd
import math
import os

data = pd.read_excel('历史预测数据输入.xlsx', sheet_name='铅钡风化待预测')
p = pd.read_excel('铅钡类玻璃包络线拟合结果.xlsx', header=None)  # p为待拟合的参数
Si_range = [50,70]

column = ['氧化钙(CaO)',
          '氧化铅(PbO)',
          '五氧化二磷(P2O5)',
          '氧化锶(SrO)']
predict_data = pd.DataFrame()

average = [0.92721171]
for i, col in enumerate(column):
    n_up = p.iloc[2*i].count()-2  # 该行非空值的个数，最后的常数项重新计算
    n_down = p.iloc[2*i+1].count()-2

    # 计算上界的系数
    b_up = data[col].copy()
    for j in range(1, n_up+1):
        b_up -= p[j][2*i]*np.power(data['二氧化硅(SiO2)'], n_up-j+1)
    # 计算上界的上范围
    b_up1 = b_up.copy()
    b_up2 = b_up.copy()
    for j in range(1, n_up+1):
        b_up1 += p[j][2*i]*math.pow(Si_range[0], n_up-j+1)
    for j in range(1, n_up+1):
        b_up2 += p[j][2*i]*math.pow(Si_range[1], n_up-j+1)
    b_up = b_up2.combine(b_up1, np.maximum).copy()

    # 计算下界的系数
    b_down = data[col].copy()
    for j in range(1, n_down+1):
        b_down -= p[j][2*i+1]*np.power(data['二氧化硅(SiO2)'], n_down-j+1)
    # 计算下界的上范围
    b_down1 = b_down.copy()
    b_down2 = b_down.copy()
    for j in range(1, n_down+1):
        b_down1 += p[j][2*i+1]*math.pow(Si_range[0], n_down-j+1)
    for j in range(1, n_down+1):
        b_down2 += p[j][2*i+1]*math.pow(Si_range[1], n_down-j+1)
    b_down = b_down2.combine(b_down1, np.minimum).copy()

    b_up[b_up<0]=0
    predict_data[col+'上界'] = b_down.copy()
    predict_data[col+'下界'] = b_up.copy()

# 保持不变的项以及文物的标签
col_static = ['文物采样点',
              '氧化钾(K2O)',
              '氧化镁(MgO)',
              '氧化铝(Al2O3)',
              '氧化铁(Fe2O3)',
              '氧化铜(CuO)',
              '氧化钡(BaO)',
              '氧化锡(SnO2)'
              ]
for col in col_static:
    predict_data[col] = data[col]
# 求平均的项
predict_data['氧化钠(Na2O)'] = average[0]
# 二氧化硫为0
predict_data['二氧化硫(SO2)'] = 0
# 二氧化硅的范围
predict_data['二氧化硅(SiO2)上界'] = Si_range[1]
predict_data['二氧化硅(SiO2)下界'] = Si_range[0]


# 对数据按照原来的顺序重新排序
predict_data = predict_data[['文物采样点', '二氧化硅(SiO2)上界', '二氧化硅(SiO2)下界', '氧化钠(Na2O)', '氧化钾(K2O)',
                             '氧化钙(CaO)上界', '氧化钙(CaO)下界', '氧化镁(MgO)', '氧化铝(Al2O3)', '氧化铁(Fe2O3)',
                             '氧化铜(CuO)', '氧化铅(PbO)上界', '氧化铅(PbO)下界', '氧化钡(BaO)','五氧化二磷(P2O5)上界', 
                             '五氧化二磷(P2O5)下界', '氧化锶(SrO)上界', '氧化锶(SrO)下界', '氧化锡(SnO2)', '二氧化硫(SO2)']
                            ]

# 写入文件
pd.DataFrame(predict_data.to_excel('铅钡类玻璃未风化化学成分预测结果.xlsx'))
