classdef bngModel_model < bngModel

	properties 
	end
	
	methods
	
	function obj = bngModel_model
	obj = obj@bngModel;
	
	species_labels = { 'A(b)', 'B(a,c)', 'C(b,d)', 'D(c)', 'A(b!1).B(a!1,c)', 'B(a,c!1).C(b!1,d)', 'C(b,d!1).D(c!1)', 'A(b!1).B(a!1,c!2).C(b!2,d)', 'B(a,c!1).C(b!1,d!2).D(c!2)', 'A(b!1).B(a!1,c!2).C(b!2,d!3).D(c!3)' };
	param_labels =  { 'A0', 'B0', 'C0', 'D0', 'kf1', 'kf2', 'kf3', 'kr1', 'kr2', 'kr3' };
	param_defaults =  [ 100, 100, 100, 100, 1, 1, 1, 1, 1, 1 ] ;
	obs_labels = { 'A_B', 'A_C', 'A_D' };
	simulators = { 'model' };
	
	obj.Param = bngParam(param_labels,param_defaults);
    obj.Obs = bngObs(obs_labels);
    obj.Species = bngSpecies(species_labels,'initSpecies_model');
    obj.simulators = simulators;
	
	end
	
	end


end
	
