clear;
%原函数
f=@(x1,x2) exp(x1+3*x2-0.1)+exp(x1-3*x2-0.1)+exp(-x1-0.1);
p=f(log(1/sqrt(2)),0);
%原函数的对x1的偏导
diff_f_1=@(x1,x2) exp(x1+3*x2-0.1)+exp(x1-3*x2-0.1)-exp(-x1-0.1);
%原函数的对x2的偏导
diff_f_2=@(x1,x2) 3*exp(x1+3*x2-0.1)-3*exp(x1-3*x2-0.1);
count_f1=1;
count_f2=1;
count_f3=1;
alpha=[0.01,0.05,0.1,0.3,0.45,0.49];
beta=[0.01,0.05,0.1,0.4,0.8,0.95,0.99];
e=1e-6;

for i = 1:length(alpha)
    for j = 1:length(beta)
        %初始的参数值
        k=1;
        x=[100,200]';
        gap=[];
        %计算x0处的情况
        nabla_f=[diff_f_1(x(1),x(2)),diff_f_2(x(1),x(2))]';
        norm_f=norm(nabla_f);
        gap(k)=f(x(1),x(2))-p;
        
        %进行回溯直线搜索
        while norm_f>e
            t=1;
            x_=x-t*nabla_f;
            while f(x_(1),x_(2)) > f(x(1),x(2))-alpha(i)*t*norm_f^2
                t=beta(j)*t;
                x_=x-t*nabla_f;
            end
            %更新参量
            x=x-t*nabla_f; 
            nabla_f=[diff_f_1(x(1),x(2)),diff_f_2(x(1),x(2))]';
            norm_f=norm(nabla_f);
            k=k+1;
            gap(k)=f(x(1),x(2))-p;
        end
        
        if k<100
            figure(1);
            semilogy(1:length(gap),gap,'--.','Markersize',5);
            leg_str_f1{count_f1} = ['\alpha=',num2str(alpha(i)),',\beta=',num2str(beta(j))]; 
            count_f1=count_f1+1;
        elseif k>400
            figure(2);
            semilogy(1:length(gap),gap,'--.','Markersize',5);
            leg_str_f2{count_f2} = ['\alpha=',num2str(alpha(i)),',\beta=',num2str(beta(j))]; 
            count_f2=count_f2+1;
        else  
            figure(3);
            semilogy(1:length(gap),gap,'--.','Markersize',5);
            leg_str_f3{count_f3} = ['\alpha=',num2str(alpha(i)),',\beta=',num2str(beta(j))]; 
            count_f3=count_f3+1;
        end
        hold on;
    end
end

figure(1);
legend(leg_str_f1);
figure(2);
legend(leg_str_f2);
figure(3);
legend(leg_str_f3);