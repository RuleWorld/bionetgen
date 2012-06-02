classdef bngExperiment < bngModel
    %BNGEXPERIMENT -  Class used to define an experiment from a model
    %   bngExperiment Properties:
    %
    %    param_init         - Initial parameter vector
    %    species_init       - Initial species vector
    %    sims_vec           - cell array of (cell array of simulators)
    %    t_sim_vec          - vector of simulation times
    %    n_steps_vec        - vector of number of simulation steps
    %    species_modifiers  - vector of bngSpeciesActions objects
    %    param_modifiers    - vector of bngParamActions objects
    %
    %   bngExperiment Methods:
    %
    %       bngExperiment   - create a bngExperiment object
    %       exec_expt       - execute the experiment given starting species and
    %                           parameter concentrations
    %           
    properties
        param_init % Initial parameter vector
        species_init % Initial species vector
        sims_vec % cell array of (cell array of simulators)
        t_sim_vec % vector of simulation times
        n_steps_vec % vector of number of simulation steps
        species_modifiers % vector of bngSpeciesActions objects
        param_modifiers % vector of bngParamActions objects
    end
    
    methods
        function obj1 = bngExperiment(obj2,species_init,param_init)
            obj1 = obj1@bngModel;
            obj1.Param = obj2.Param;
            obj1.Obs = obj2.Obs;
            obj1.Species = obj2.Species;
            obj1.simulators = obj2.simulators;
            obj1.species_init = species_init;
            obj1.param_init = param_init;
        end
        
        function [err,sp, ob] = exec_expt(obj)
            if(isempty(obj.sims_vec))
                obj.sims_vec = cell(1,length(obj.t_sim_vec));
                for i=1:1:length(obj.t_sim_vec)
                    obj.sims_vec{i} = obj.simulators;
                end
            end
            
            if(isempty(obj.species_modifiers))
                obj.species_modifiers = cell(1,length(obj.t_sim_vec));
                for i=1:1:length(obj.t_sim_vec)
                    obj.species_modifiers{i} = bngSpeciesActions(obj.Species);
                end
            end
            
            if(isempty(obj.param_modifiers))
                obj.param_modifiers = cell(1,length(obj.t_sim_vec));
                for i=1:1:length(obj.t_sim_vec)
                    obj.param_modifiers{i} = bngParamActions(obj.Param);
                end
            end
            
            p0 = obj.param_init;
            s0 = obj.species_init;
            sv = obj.sims_vec;
            tv = obj.t_sim_vec;
            ns = obj.n_steps_vec;
            sm = obj.species_modifiers;
            pm = obj.param_modifiers;
            
            
            [err,timepoints, species_out, observables_out] = obj.exec_seq(sv,s0,p0,tv,ns,sm,pm);
            
            sp = bngTraj(obj.Species,timepoints,species_out);
            ob = bngTraj(obj.Obs,timepoints,observables_out);
            
        end
    end
    
end
%[err,timepoints, species_out, observables_out] = exec_seq(obj,sims_vec, species_init,param_init,t_sim_vec,n_steps_vec,species_modifiers,param_modifiers)