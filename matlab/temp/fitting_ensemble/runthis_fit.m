clear;

% Setting up model and parameter distributions
a = bngModel_model();
par = a.Param;
par = par.setDistrib('A0',distUniformLog(10,1000));
par = par.setDistrib('B0',distUniformLog(10,1000));
par = par.setDistrib('C0',distUniformLog(10,1000));
par = par.setDistrib('D0',distUniformLog(10,1000));
par = par.setDistrib('kf1',distUniformLog(0.1,10));
par = par.setDistrib('kf2',distUniformLog(0.1,10));
par = par.setDistrib('kf3',distUniformLog(0.1,10));
par = par.setDistrib('kr1',distUniformLog(0.1,10));
par = par.setDistrib('kr2',distUniformLog(0.1,10));
par = par.setDistrib('kr3',distUniformLog(0.1,10));
a.Param = par;
clear par;

% setting up data object
filename_array = {'model_1.gdat' 'model_2.gdat' 'model_3.gdat'};
traj = bngTraj.trajRead_Multiple(filename_array,a.Obs);

data1 = bngData();
data1 = data1.addDataObject(traj.generateDataObject('A_B'));
data1 = data1.addDataObject(traj.generateDataObject('A_C'));
data1 = data1.addDataObject(traj.generateDataObject('A_D'));

% using fmincon
s = bngSampler(a.Param);
disp('Running fmincon');
lossfn = @ (y) fitter(data1,a,s.invcdf(y));
lb = 0.0001*ones(1,s.N);
ub = 0.9999*ones(1,s.N);
y0 = s.cdf(s.defaults);
tic;y1 = fmincon(lossfn,y0,[],[],[],[],lb,ub);toc;
x1 = s.invcdf(y1)

save bestfit.mat a s x1 y1 data1;
