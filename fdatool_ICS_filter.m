t = 0:0.002:100;
t = t(1,1:length(in));
%Hd = fda;
%y = filter_j(in);
y = filter(Hd,in);
y2= in-y;
%plot(t,y,t,in,t,y2)
plot(y2)
grid on;
%hold on;
%plot(t,oo);
