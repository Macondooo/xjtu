data=xlsread('历史预测数据输入.xlsx',3);
%输入所有待拟合的数据
X=data(:,1);
Y_Ca=data(:,2);
Y_Pb=data(:,3);
Y_P=data(:,4);
Y_Sr=data(:,5);
%定义拟合的坐标范围和步长
x_min=0;
x_max=70;
x_int=0.01;
%记录所有拟合参数并且写入excel中
curve_fit=[];
%将拟合的参数写入excel
title= ['氧化钙(CaO)上界拟合   ';
        '氧化钙(CaO)下界拟合   ';
        '氧化铅(PbO)上界拟合   ';
        '氧化铅(PbO)下界拟合   ';
        '五氧化二磷(P2O5)上界拟合';
        '五氧化二磷(P2O5)下界拟合';
        '氧化锶(SrO)上界拟合   ';
        '氧化锶(SrO)下界拟合   '];
title=cellstr(title);
xlswrite('铅钡类玻璃包络线拟合结果.xlsx',title,1,'A1')


%绘制子图1
subplot(2,2,1)
n_up=2;
n_down=1;
window_n=6;
[Xup_fit,Yup_fit,Xdown_fit,Ydown_fit,delta_up,delta_down,Aup,Adown]=BD_poly(X,Y_Ca,n_up,n_down,x_min,x_max,x_int,window_n);
%将拟合的参数写入excel
xlswrite('铅钡类玻璃包络线拟合结果.xlsx',Aup,1,'B1')
xlswrite('铅钡类玻璃包络线拟合结果.xlsx',Adown,1,'B2')
%开始画图
scatter(X,Y_Ca,'xk');
hold on;
p1=plot(Xup_fit,Yup_fit,'r--',Xdown_fit,Ydown_fit,'r--');
p2=plot(Xup_fit,Yup_fit+2*delta_up,'b--');
%plot(Xdown_fit,Ydown_fit-2*delta_down,'b--');
legend([p1(1),p2(1)],'包络线拟合','95%置信区间');
xlabel('二氧化硅(SiO2)含量')
ylabel('氧化钙(CaO)含量')

%绘制子图2
subplot(2,2,2)
n_up=2;
n_down=2;
window_n=3;
[Xup_fit,Yup_fit,Xdown_fit,Ydown_fit,delta_up,delta_down,Aup,Adown]=BD_poly(X,Y_Pb,n_up,n_down,x_min,x_max,x_int,window_n);
%将拟合的参数写入excel
xlswrite('铅钡类玻璃包络线拟合结果.xlsx',Aup,1,'B3')
xlswrite('铅钡类玻璃包络线拟合结果.xlsx',Adown,1,'B4')
%开始画图
scatter(X,Y_Pb,'xk');
hold on;
p1=plot(Xup_fit,Yup_fit,'r--',Xdown_fit,Ydown_fit,'r--');
p2=plot(Xup_fit,Yup_fit+2*delta_up,'b--');
%plot(Xdown_fit,Ydown_fit-2*delta_down,'b--');
legend([p1(1),p2(1)],'包络线拟合','95%置信区间');
xlabel('二氧化硅(SiO2)含量')
ylabel('氧化铅(PbO)含量')


%绘制子图3
subplot(2,2,3)
n_up=2;
n_down=1;
window_n=6;
[Xup_fit,Yup_fit,Xdown_fit,Ydown_fit,delta_up,delta_down,Aup,Adown]=BD_poly(X,Y_P,n_up,n_down,x_min,x_max,x_int,window_n);
%将拟合的参数写入excel
xlswrite('铅钡类玻璃包络线拟合结果.xlsx',Aup,1,'B5')
xlswrite('铅钡类玻璃包络线拟合结果.xlsx',Adown,1,'B6')
%开始画图
scatter(X,Y_P,'xk');
hold on;
p1=plot(Xup_fit,Yup_fit,'r--',Xdown_fit,Ydown_fit,'r--');
p2=plot(Xup_fit,Yup_fit+2*delta_up,'b--');
%plot(Xdown_fit,Ydown_fit-2*delta_down,'b--');
legend([p1(1),p2(1)],'包络线拟合','95%置信区间');
xlabel('二氧化硅(SiO2)含量')
ylabel('五氧化二磷(P2O5)含量')


%绘制子图4
subplot(2,2,4)
n_up=1;
n_down=1;
window_n=7;
[Xup_fit,Yup_fit,Xdown_fit,Ydown_fit,delta_up,delta_down,Aup,Adown]=BD_poly(X,Y_Sr,n_up,n_down,x_min,x_max,x_int,window_n);
%将拟合的参数写入excel
xlswrite('铅钡类玻璃包络线拟合结果.xlsx',Aup,1,'B7')
xlswrite('铅钡类玻璃包络线拟合结果.xlsx',Adown,1,'B8')
%开始画图
hold on;
scatter(X,Y_Sr,'xk');
p1=plot(Xup_fit,Yup_fit,'r--',Xdown_fit,Ydown_fit,'r--');
p2=plot(Xup_fit,Yup_fit+2*delta_up,'b--');
%plot(Xdown_fit,Ydown_fit-2*delta_down,'b--');
legend([p1(1),p2(1)],'包络线拟合','95%置信区间');
xlabel('二氧化硅(SiO2)含量')
ylabel('氧化锶(SrO)含量')






