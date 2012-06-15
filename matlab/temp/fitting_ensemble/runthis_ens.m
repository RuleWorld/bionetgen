% Metropolis Hastings sampling
clear; load bestfit.mat;

sig = 0.01;
nsamples = 1000;
kt = 100;
logpdf = @(y)  (- fitter(data1,a,s.invcdf(y))/kt);
%logpdf = @(y) 0;
proppdf = @(x,y)  s.calcMoveDist(x,y,sig);
proprnd = @ (y) s.genMove(y,sig);


% Getting nsamples 10 times
x_all = zeros(nsamples,s.N,10);
y_all = zeros(nsamples,s.N,10);
ystart = y1;


for i=1:1:10
    disp(strcat('Running MH-Sampler batch: ',num2str(i)));
    tic;y_matr = mhsample(ystart,nsamples,'logpdf',logpdf,'proprnd',proprnd,'proppdf',proppdf,'symmetric',1);toc;
    x_matr = s.invcdf_matr(y_matr);
    x_all(:,:,i) = x_matr;
    y_all(:,:,i) = y_matr;
    ystart = y_matr(nsamples,:);
end
x_new = reshape(x_all,[10*nsamples 10]);
y_new = reshape(y_all,[10*nsamples 10]);
logloss = zeros(size(x_new,1),1);
for i=1:1:size(x_new,1)
    if(mod(i,100)==0)
        disp(strcat('Computing loss done for samples:',num2str(i)));
    end
    logloss(i) = log10(fitter(data1,a,x_new(i,:)));
end

save mh.mat x_new y_new logloss;

e = bngEnsemble(y_new,s.labels);
e = e.addFeatures(logloss,{'logloss'});
e.plotQuantiles('logloss',100);

conditionvec = e.getFeatures({'logloss'}) < 4;
e1 = e.getSubsetEnsemble([],conditionvec);
e1.plotQuantiles('logloss',100);
e1.scatter_3d({'A0' 'kf1' 'logloss'});

[pc,score,latent,tsquare] = princomp(e1.data);
