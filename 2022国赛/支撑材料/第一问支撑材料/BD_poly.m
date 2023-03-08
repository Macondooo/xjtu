%  Find the boundary of data
%  X-input x coordinary
%  Y-input y coordinary
%  n-polyfit times
%  x_min,x_max,x_int-range of x and interval of x
function [Xup_fit,Yup_fit,Xdown_fit,Ydown_fit,delta_up,delta_down,Aup,Adown]=BD_poly(X,Y,n_up,n_down,x_min,x_max,x_int,window_n)
[X2,p]=sort(X);
Y2=Y(p);
while mod(length(Y2),window_n)~=0
    Y2(end+1)=0;
    X2(end+1)=0;
end
Y_up=max(reshape(Y2,window_n,length(Y2)/window_n));
for i = 1:length(Y_up)
    X_up(i)=X2(find(Y2==Y_up(i),1));
end
Y_down=min(reshape(Y2,window_n,length(Y2)/window_n));
for i = 1:length(Y_down)
    X_down(i)=X2(find(Y2==Y_down(i),1));
end
[Aup,Sup]=polyfit([X_up],[Y_up],n_up);
Xup_fit=x_min:x_int:x_max;
[Yup_fit,delta_up]=polyval(Aup,Xup_fit,Sup);
[Adown,Sdown]=polyfit([X_down],[Y_down],n_down);
Xdown_fit=x_min:x_int:x_max;
[Ydown_fit,delta_down]=polyval(Adown,Xdown_fit,Sdown);
end
