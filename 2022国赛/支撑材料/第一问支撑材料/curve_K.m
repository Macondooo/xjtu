data=xlsread('历史预测数据输入.xlsx',4);
%输入所有待拟合的数据
X=data(:,1);
Y_K=data(:,2);
Y_Ca=data(:,3);
Y_Al=data(:,4);
Y_Fe=data(:,5);
%定义拟合的坐标范围和步长
x_min=63;
x_max=95;
x_int=0.01;
%记录所有拟合参数并且写入excel中
curve_fit=[];
%将拟合的参数写入excel
title= ['氧化钾(K2O)上界拟合  ';
        '氧化钾(K2O)下界拟合  ';
        '氧化钙(CaO)上界拟合  ';
        '氧化钙(CaO)下界拟合  ';
        '氧化铝(Al2O3)上界拟合';
        '氧化铝(Al2O3)下界拟合';
        '氧化铁(Fe2O3)上界拟合';
        '氧化铁(Fe2O3)下界拟合'];
title=cellstr(title);
xlswrite('高钾类玻璃包络线拟合结果.xlsx',title,1,'A1')


%绘制子图1
subplot(2,2,1)
n_up=1;
n_down=1;
window_n=3;
[Xup_fit,Yup_fit,Xdown_fit,Ydown_fit,delta_up,delta_down,Aup,Adown]=BD_poly(X,Y_K,n_up,n_down,x_min,x_max,x_int,window_n);
%将拟合的参数写入excel
xlswrite('高钾类玻璃包络线拟合结果.xlsx',Aup,1,'B1')
xlswrite('高钾类玻璃包络线拟合结果.xlsx',Adown,1,'B2')
%开始画图
scatter(X,Y_K,'xk');
hold on;
p1=plot(Xup_fit,Yup_fit,'r--',Xdown_fit,Ydown_fit,'r--');
p2=plot(Xup_fit,Yup_fit+2*delta_up,'b--');
%plot(Xdown_fit,Ydown_fit-2*delta_down,'b--');
legend([p1(1),p2(1)],'包络线拟合','95%置信区间');
xlabel('二氧化硅(SiO2)含量')
ylabel('氧化钾(K2O)含量')

%绘制子图2
subplot(2,2,2)
n_up=1;
n_down=1;
window_n=4;
[Xup_fit,Yup_fit,Xdown_fit,Ydown_fit,delta_up,delta_down,Aup,Adown]=BD_poly(X,Y_Ca,n_up,n_down,x_min,x_max,x_int,window_n);
%将拟合的参数写入excel
xlswrite('高钾类玻璃包络线拟合结果.xlsx',Aup,1,'B3')
xlswrite('高钾类玻璃包络线拟合结果.xlsx',Adown,1,'B4')
%开始画图
scatter(X,Y_Ca,'xk');
hold on;
p1=plot(Xup_fit,Yup_fit,'r--',Xdown_fit,Ydown_fit,'r--');
p2=plot(Xup_fit,Yup_fit+2*delta_up,'b--');
%plot(Xdown_fit,Ydown_fit-2*delta_down,'b--');
legend([p1(1),p2(1)],'包络线拟合','95%置信区间');
xlabel('二氧化硅(SiO2)含量')
ylabel('氧化钙(CaO)含量')


%绘制子图3
subplot(2,2,3)
n_up=2;
n_down=2;
window_n=3;
[Xup_fit,Yup_fit,Xdown_fit,Ydown_fit,delta_up,delta_down,Aup,Adown]=BD_poly(X,Y_Al,n_up,n_down,x_min,x_max,x_int,window_n);
%将拟合的参数写入excel
xlswrite('高钾类玻璃包络线拟合结果.xlsx',Aup,1,'B5')
xlswrite('高钾类玻璃包络线拟合结果.xlsx',Adown,1,'B6')
%开始画图
scatter(X,Y_Al,'xk');
hold on;
p1=plot(Xup_fit,Yup_fit,'r--',Xdown_fit,Ydown_fit,'r--');
p2=plot(Xup_fit,Yup_fit+delta_up,'b--');
plot(Xdown_fit,Ydown_fit-2*delta_down,'b--');
legend([p1(1),p2(1)],'包络线拟合','95%置信区间');
xlabel('二氧化硅(SiO2)含量')
ylabel('氧化铝(Al2O3)含量')


%绘制子图4
subplot(2,2,4)
n_up=2;
n_down=1;
window_n=4;
[Xup_fit,Yup_fit,Xdown_fit,Ydown_fit,delta_up,delta_down,Aup,Adown]=BD_poly(X,Y_Fe,n_up,n_down,x_min,x_max,x_int,window_n);
%将拟合的参数写入excel
xlswrite('高钾类玻璃包络线拟合结果.xlsx',Aup,1,'B7')
xlswrite('高钾类玻璃包络线拟合结果.xlsx',Adown,1,'B8')
%开始画图
hold on;
scatter(X,Y_Fe,'xk');
p1=plot(Xup_fit,Yup_fit,'r--',Xdown_fit,Ydown_fit,'r--');
p2=plot(Xup_fit,Yup_fit+delta_up,'b--');
plot(Xdown_fit,Ydown_fit-2*delta_down,'b--');
legend([p1(1),p2(1)],'包络线拟合','95%置信区间');
xlabel('二氧化硅(SiO2)含量')
ylabel('氧化铁(Fe2O3)含量')






