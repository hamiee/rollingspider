t = 0:0.002:100;
t = t(1,1:length(in));

y = filter(Hd,in);
%plot(t,y)

Xsaved = zeros(length(t),1);
Zsaved = zeros(length(t),1);
 

simple_kal(1,1);

dt = 0.002;
for k=1:length(t)
    z = y(k,1);
    o = simple_kal(z,0);
    Xsaved(k)=o;
    Zsaved(k)=z;
end

plot(Xsaved);
hold on;
plot(Zsaved);
    
