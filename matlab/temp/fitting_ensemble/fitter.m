function [ loss ] = fitter( data_obj,model_obj,param_init )
%FITTER Summary of this function goes here
%   Detailed explanation goes here
assert(isa(data_obj,'bngData'));
assert(isa(model_obj,'bngModel'));


species_init = model_obj.Species.initializeAll(param_init);
ex = bngExperiment(model_obj,species_init,param_init);
ex.t_sim_vec = 0.2;
ex.n_steps_vec = 10;

[err, ~,ob] = ex.exec_expt();
data2 = bngData();
data2 = data2.addDataObject(ob.generateDataObject('A_B'));
data2 = data2.addDataObject(ob.generateDataObject('A_C'));
data2 = data2.addDataObject(ob.generateDataObject('A_D'));

if err
    loss = 1e23;
else
    loss = data_obj.computeFit(data2);
end


end

