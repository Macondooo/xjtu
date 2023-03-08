clear;
syms x1 x2;
f= exp(x1+3*x2-0.1)+exp(x1-3*x2-0.1)+exp(-x1-0.1);%原函数
grad_f=[diff(f,x1),diff(f,x2)]';%函数f的梯度
hessian_f=hessian(f,[x1,x2]);%函数f的hessian矩阵

%将上面的符号函数都转为匿名函数
f=matlabFunction(f);
grad_f=matlabFunction(grad_f);
hessian_f=matlabFunction(hessian_f);

p=f(log(1/sqrt(2)),0);%最优值

%初始的参数的设置
alpha=0.1;
beta=0.7;
e=1e-6;
k=1;
x=[2,5]';%2,5
gap=[];
%计算x0处的情况
gap(k)=f(x(1),x(2))-p; 
d = -hessian_f(x(1),x(2))\grad_f(x(1),x(2));%下降方向
lamda_2= d'*hessian_f(x(1),x(2))* d; %牛顿减少量的平方

%进行回溯直线搜索
while 0.5*lamda_2>e
    t=1;
    x_=x+t*d;
    while (f(x_(1),x_(2)) > (f(x(1),x(2))-alpha*t*lamda_2))
        t=beta*t;
        x_=x+t*d;
    end
    %更新参量
    x=x+t*d;
    d=-hessian_f(x(1),x(2))\grad_f(x(1),x(2));%下降方向
    lamda_2= d'*hessian_f(x(1),x(2))* d; %牛顿减少量的平方
    k=k+1;
    gap(k)=f(x(1),x(2))-p;
end

semilogy(1:length(gap),gap,'--.','Markersize',5);
legend('\alpha=0.1,\beta=0.7'); 

