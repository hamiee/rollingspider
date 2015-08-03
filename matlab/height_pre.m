clear

data = load('raw_press')';
data = data./10;
N = length(data);
Ts = 0.01;
t = linspace(0,N*Ts,N);



n=4;
p = polyfit(t,data,n);

y = polyval(p,t);


poly2str(p,'x')

%for N=1:12
%    y(1,N)=0;
%end

diff_y = diff(y);

%plot(data)
hold on
%plot(y);
hold on
%plot(diff_y);
hold on
diff_sq_y = diff(diff_y);
plot(diff_sq_y)
grid on



fd = fopen('godo_data','w');
fprintf(fd,'%g\n',y);

%% 

in = load('input2');
out = load('godo_data');


%%
%step(tf1)

