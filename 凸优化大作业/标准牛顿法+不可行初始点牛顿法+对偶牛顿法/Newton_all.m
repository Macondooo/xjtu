%% 这里包括了大作业3全部代码
% 预定义函数以及参数
% 初始化
% 标准Newton方法
% 不可行初始点Newton方法
% 对偶Newton方法

%% 预定义函数以及参数
clc;
clear;
n=100;p=30;

%随机生成满秩矩阵A
A=rand(p,n);
while rank(A) ~= p
    A=0.1+rand(p,n);
end

%随机生成正向量 x0
x0=rand(n,1);
b=A*x0;  %x0是可行的

%随机生成向量v0
v0=rand(p,1);

%生成原函数以及其他计算中用到的函数
syms X [n,1] matrix 
syms V [p,1] matrix
f=X.'* log(X);%原函数f
H_f=diff(f,X,X.');%函数f的hessian矩阵
G_f=diff(f,X)';%函数f的梯度
r=[G_f+A'*V;A*X-sym(b)];%原对偶残差
g=sym(ones(1,n))*exp(-sym(ones(n,1))-sym(A')*V)+sym(b')*V; %对偶函数的负数，加负号是为了变成凸优化问题

%设置其他参数的值
e=1e-10;
alpha=0.1;
beta=0.7;


%% 初始化

% 为标准Newton方法计算初始点处的情况
value_G_f=double(sym(subs(G_f,x0)));
value_H_f=double(sym(subs(H_f,x0)));
matrix_A=[value_H_f,A';A,zeros(p,p)];
matrix_b=[-value_G_f;zeros(p,1)];
dr=matrix_A\matrix_b;
d=dr(1:100);%牛顿方向
w=dr(101:130);%对偶变量
lamda_2=d'*value_H_f*d;%牛顿减少量


% 为不可行初始点Newton方法计算初始点处的情况
matrix_b_2=-[value_G_f+A'*v0;A*x0-b];
value_r=double(sym(subs(r,{X,V},{x0,v0})));
norm_r=norm(value_r);%初始点处原对偶残差的范数
dr_2=matrix_A\matrix_b_2;
dx=dr_2(1:100);%牛顿方向
dv=dr_2(101:130);%对偶变量


% 对偶Newton方法计算初始点处的情况
g_v=b-A*exp(-1-A'*v0);
hessian_v=A*diag(exp(-1-A'*v0))*A';
d_v = -hessian_v\g_v;%下降方向
lamda_v= d_v'*hessian_v* d_v; %牛顿减少量的平方
%% 标准Newton方法

x=x0;
k=1;

%进行回溯直线搜索
while 0.5*lamda_2>e
    t=1;
    x_=x+t*d;
    while double(sym(subs(f,x_))) > (double(sym(subs(f,x)))-alpha*t*lamda_2)
        t=beta*t;
        x_=x+t*d;
    end
    %更新参量
    x=x+t*d;%新的x
    value_G_f=double(sym(subs(G_f,x)));
    value_H_f=double(sym(subs(H_f,x)));
    matrix_A=[value_H_f,A';A,zeros(p,p)];
    matrix_b=[-value_G_f;zeros(p,1)];
    dr=matrix_A\matrix_b;
    d=dr(1:100);%新的牛顿方向
    w=dr(101:130);%新的对偶变量
    lamda_2=d'*value_H_f*d;%新的牛顿减少量
   
    %gap_1(k)=0.5*lamda_2;
    k=k+1;
end

% figure(1)
% semilogy(1:length(gap_1),gap_1,'--.','Markersize',5);
fprintf('标准Newton方法的迭代次数为%u\n',k)
%保存变量
x_a=x;
v_a=w;

%% 不可行初始点Newton方法

x=x0;
v=v0;
k=1;

%进行回溯直线搜索
while norm_r>e || norm(A*x-b)<1e-16
    t=1;
    x_=x+t*dx;
    v_=v+t*dv;
    while norm(double(sym(subs(r,{X,V},{x_,v_})))) > (1-alpha*t)*norm_r
        t=beta*t;
        x_=x+t*dx;
        v_=v+t*dv;
    end
    %更新参量
    x=x+t*dx;%新的x
    v=v+t*dv;% 新的v

    value_G_f=double(sym(subs(G_f,x)));
    value_H_f=double(sym(subs(H_f,x)));
    matrix_A=[value_H_f,A';A,zeros(p,p)];
    matrix_b_2=-[value_G_f+A'*v;A*x-b];
    value_r=double(sym(subs(r,{X,V},{x,v})));
    norm_r=norm(value_r);%初始点处原对偶残差的范数
    dr_2=matrix_A\matrix_b_2;
    dx=dr_2(1:100);%牛顿方向
    dv=dr_2(101:130);%对偶变量更改量
   
    %gap_2(k)=norm_r;
    k=k+1;
end

% figure(2)
% semilogy(1:length(gap_2),gap_2,'--.','Markersize',5);
fprintf('不可行初始点Newton方法的迭代次数为%u\n',k)
%保存变量
x_b=x;
v_b=v;

%% 对偶Newton方法

v=v0;
k=1;

%进行回溯直线搜索
while 0.5*lamda_v>e
    t=1;
    v_=v+t*d_v;
    while double(sym(subs(g,v_))) > (double(sym(subs(g,v)))-alpha*t*lamda_v)
        t=beta*t;
        v_=v+t*d_v;
    end
    %更新参量
    v=v+t*d_v;
    g_v=b-A*exp(-1-A'*v);
    hessian_v=A*diag(exp(-1-A'*v))*A';
    d_v = -hessian_v\g_v;%下降方向
    lamda_v= d_v'*hessian_v* d_v; %牛顿减少量的平方
    k=k+1;
end

fprintf('对偶Newton方法的迭代次数为%u\n',k)
%保存变量
x_c=exp(-1-A'*v);
v_c=v;

%% 验证是否是相同的最优点
norm(x_a-x_b)
norm(x_b-x_c)
norm(x_c-x_a)

norm(v_a-v_b)
norm(v_b-v_c)
norm(v_c-v_a)

