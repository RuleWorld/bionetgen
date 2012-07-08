s = bngSampler(a.Param);

% fmincon search
%[x0 y0] = s.genRand(1);
disp('Running fmincon');
lossfn = @ (y) fitter(data1,a,s.invcdf(y));
lb = 0.0001*ones(1,s.N);
ub = 0.9999*ones(1,s.N);
y0 = s.cdf(s.defaults);
tic;y1 = fmincon(lossfn,y0,[],[],[],[],lb,ub);toc;
x1 = s.invcdf(y1);


% MHSample

sig = 0.01;
nsamples = 1000;
kt = 100;
logpdf = @(y)  (- fitter(data1,a,s.invcdf(y))/kt);
%logpdf = @(y) 0;
proppdf = @(x,y)  s.calcMoveDist(x,y,sig);
proprnd = @ (y) s.genMove(y,sig);


x_all = zeros(nsamples,s.N,10);
y_all = zeros(nsamples,s.N,10);

ystart = y1;
for i=1:1:10


    disp('Running MH-Sampler');


    tic;y_matr = mhsample(ystart,nsamples,'logpdf',logpdf,'proprnd',proprnd,'proppdf',proppdf,'symmetric',1);toc;
    x_matr = s.invcdf_matr(y_matr);
    x_all(:,:,i) = x_matr;
    y_all(:,:,i) = y_matr;
    ystart = y_matr(nsamples,:);
end


x_new = reshape(x_all,[10000 10]);
y_new = reshape(y_all,[10000 10]);

save mh2.mat x_new y_new;


% sampling the whole space
q = qrandstream('halton',s.N,'Skip',1e3,'Leap',1e2);
nsamples = 10000;
reset(q);
y_new = qrand(q,nsamples);
x_new = s.invcdf_matr(y_new);
loss = zeros(nsamples,1);
tic;
for i=1:1:nsamples
    if mod(i,100)==0
        disp(strcat(num2str(i),' samples simulated'));toc;tic;
    end
    loss(i) = fitter( data1,a,x_new(i,:) );
end

save halton.mat x_new y_new loss;
selector = loss <4e4;
x_new = x_new(selector,:);
y_new = y_new(selector,:);
loss = loss(selector);

% % % sig = 0.01;
% % % nsamples = 1000;
% % % % xvals = zeros(nsamples,a.Param.N);
% % % % yvals = zeros(nsamples,a.Param.N);
% % % % 
% % % % for i=1:1:nsamples
% % % %     [x y] = a.Param.genMove(y0,sig);
% % % %     xvals(i,:) = x;
% % % %     yvals(i,:) = y;
% % % %     y0 = y;
% % % % end
% % % 
% % % 
% % % loss = fitter (data1,a,s.invcdf(y0));
% % % logpdf = @(y)  (- fitter(data1,a,s.invcdf(y)));
% % % proppdf = @(x,y)  s.calcMoveDist(x,y,sig);
% % % 
% % % 
% % % proprnd = @ (y) s.genMove(y,sig);
% % % smp1 = mhsample(y0,nsamples,'logpdf',logpdf,'proprnd',proprnd,'proppdf',proppdf,'symmetric',1);
% x = s.invcdf_matr(smp1);


%p0 = x0;
%s0 = a.Species.initializeAll(p0);
% sv = {{'model'}};
% tv = [0.2];
% ns = [10];
% sm = {bngSpeciesActions(a.Species)};
% pm = {bngParamActions(a.Param)};
%ex = bngExperiment(a,s0,p0);
% exec_seq(sims_vec, species_init,param_init,t_sim_vec,n_steps_vec,species_modifiers,param_modifiers)
% [err,timepoints, sp, ob] = ex.exec_seq(sv,s0,p0,tv,ns,sm,pm)
%ex.t_sim_vec = 0.2;
%ex.n_steps_vec = 10;
%obj = ex;
