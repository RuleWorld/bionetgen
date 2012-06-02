clear;
a = bngModel_Name();
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

%n_samples = 1000;
%[x, y] = a.Param.genRand( n_samples );

filename_array = {'model_1.gdat' 'model_2.gdat' 'model_3.gdat'};
traj = bngTraj.trajRead_Multiple(filename_array,a.Obs);

data1 = bngData();
data1 = data1.addDataObject(traj.generateDataObject('A_B'));
data1 = data1.addDataObject(traj.generateDataObject('A_C'));
data1 = data1.addDataObject(traj.generateDataObject('A_D'));

% loss = zeros(1,n_samples);
% tic;
% for i=1:1:n_samples
%     loss(i) = fitter(data1,a,x(i,:));
%     if(mod(i,10)==0)
%         disp(strcat(num2str(i),' samples simulated'));
%     end
% end
% toc;
% loss1 = loss;
% 
% ens = bngEnsemble(x,a.Param.labels);
% ens = ens.addFeatures(loss(:),{'loss'});
% loss2 = sort(ens.getFeatures({'loss'}));
% limit = loss2(101);
% conditionvec = ens.getFeatures({'loss'}) < limit;
% ens2 = ens.getSubsetEnsemble([],conditionvec);
% 
% 
