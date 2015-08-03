function out = simple_kal(in,first)

%
%
persistent A H Q R x P firstRun

dt = 0.002;

if isempty(firstRun)
    A=[1 dt; 0 1];
    H=[dt^2/2 dt];
    
    Q=[1 0 ;0 1];
    R=1;
    
    x = [0 0]';
    P = 1*eye(2);
    firstRun =1;
end

xp=A*x;
Pp= A*P*A' + Q;
K = Pp*H'*inv(H*Pp*H'+R);

x = xp+K*(in-H*xp);
P= Pp-K*H*Pp;

out = x(1);