classdef bngModel
    %BNGMODEL -  Model class from which specific BNG models are derived
    %   bngModel Properties:
    %       Param       - bngParam object
    %       Obs         - bngObs object
    %       Species     - bngSpecies object
    %       simulators  - cell array of names of simulator m-files
    %   bngModel Methods:
    %       exec        - executes a single simulation
    %       exec_seq    - executes a sequence of simulations
    
    
    properties
        Param       % bngParam object
        Obs         % bngObs object
        Species     % bngSpecies object
        simulators  % cell array of names of simulator m-files
    end
    
    methods
        function obj = bngModel()
        end
        
        function [err, species_out, observables_out] = exec(obj,sims,t_sim,n_steps, species_init,parameters)
            %bngModel.exec() executes a single simulation.
            %USAGE: 
            %
            %[err, species_out, observables_out] = obj.exec(sims,t_sims,n_steps,species_init,parameters)
            %   obj     -       any object of bngModel (or any class derived from bngModel)   
            %
            %
            %   sims    -       cell array of simulator names {'sim1' 'sim2' }
            %                   must be subset of property bngModel.simulators
            %                   simulator function must exist and be accessible by MatLab
            %                   if empty [], uses property bngModel.simulators
            %
            %   t_sims  -       number, time of simulation
            %   n_steps -       number, number of steps to record
            %
            %   species_init -  row vector of species concentrations [ ... ]
            %                   if empty [], uses initialization defined in
            %                   the model using the passed parameter vector
            %
            %   parameters -    row vector of parameter values [ ... ]
            %                   if empty [], uses default model parameters 
            
            
            
            % Checking and setting up input options
            assert(~isempty(t_sim),'Must provide simulation time');
            assert(~isempty(n_steps),'Must provide number of steps');
            if(isempty(sims))
                sims = obj.simulators;
            else
                for i=1:1:length(sims)
                   assert(min(size(sims))==1,'Simulators provided should be one-dimensional array of strings');
                   assert(any(strcmp(sims{i},obj.simulators)),'Simulator not found');
                end
            end
            if(isempty(parameters))
                parameters = obj.Param.defaults;
            end
            if(isempty(species_init))
                species_init = obj.Species.initializeAll(parameters);
            end
            
            
            for i=1:1:length(sims)
                err = 0;
                fhandle = str2func(obj.simulators{i});
                timepoints = linspace(0,t_sim,n_steps+1)';
                [err, ~, species_out, observables_out ] = model( timepoints, species_init, parameters, 1 );
                if(~err), break,end
            end
        end
        
         
        function [err,timepoints, species_out, observables_out] = exec_seq(obj,sims_vec, species_init,param_init,t_sim_vec,n_steps_vec,species_modifiers,param_modifiers)

            %bngModel.exec_seq() executes a sequence of simulations
            %USAGE: 
            %
            %[err,timepoints, species_out, observables_out] = obj.exec_seq(sims_vec, species_init,param_init,t_sim_vec,n_steps_vec,species_modifiers,param_modifiers)
            %   obj     -       any object of bngModel (or any class derived from bngModel)   
            %
            %   sims_vec        cell array of (cell array of names of
            %                   simulators) { {'' ''}  {'' ''} }
            %
            %   species_init    row vector of species concentrations [ ... ]
            %                   cannot be empty
            %
            %   param_init  -   row vector of parameter values [ ... ]
            %                   cannot be empty
            %
            %
            %   t_sim_vec   -   vector of numbers [...], times of simulation
            %
            %   n_steps_vec -   vector of numbers [...], number of steps to
            %                   record for each simulation
            %
            %   species_modifiers -     row { } of bngSpeciesActions
            %                           objects
            %
            %   param_modifiers -     row { } of bngParamActions
            %                           objects
            %

            
            

            
            x = min(size(t_sim_vec))==1 && min(size(n_steps_vec))==1 && min(size(species_modifiers))==1&& min(size(param_modifiers))==1;
            assert(x==1,'Times, steps, init-species and species modifiers should be linear in dimension');
            x = length(t_sim_vec)==length(n_steps_vec) && length(t_sim_vec)==length(species_modifiers) && length(t_sim_vec)==length(param_modifiers);
            assert(x==1,'Times, steps, init-species and species modifiers and parameter vectors should be consistent in one dimension');
            
            
            n_simulations = length(t_sim_vec);
            % Setting up inputs
            if isempty(sims_vec)
                sims_vec = cell(1,n_simulations);
                for i=1:1:n_simulations
                    sims_vec{i} = obj.simulators;
                end
            end
            
            if isempty(param_init)
                param_init = obj.Param.defaults;
            end
            
            if isempty(species_init)
                species_init = obj.Species.initializeAll(param_init);
            end
            
           
            assert(length(species_init)==obj.Species.N,'Species-init vector is of improper length');
            assert(length(param_init)==obj.Param.N,'Param-init vector is of improper length');
            for i=1:1:n_simulations
                assert(isa(param_modifiers{i},'bngParamActions'),'Parameter modifiers are improper');
                assert(isa(species_modifiers{i},'bngSpeciesActions'),'Species modifiers are improper');
            end
            
            
            % setting up output sizes
            n_steps_tot = sum(n_steps_vec);
            timepoints = zeros(1,n_steps_tot+1)';
            err = zeros(1,n_steps_tot+1)';
            species_out = zeros(n_steps_tot+1,obj.Species.N);
            observables_out = zeros(n_steps_tot+1,obj.Obs.N);
            
            
            % setting up simulation inputs
            t_end = 0;
            endpoint = 1;
            for i=1:1:n_simulations
                sims = sims_vec{i};
                t_sim = t_sim_vec(i);
                n_steps = n_steps_vec(i);
                
                t_start = t_end;
                startpoint = endpoint;
                
                t_end = t_start + t_sim;
                endpoint = startpoint + n_steps;
                
                % setting species and parameter inputs
                if i==1
                    species_init1 = species_modifiers{i}.eval(species_init);
                else
                    species_init1 = species_modifiers{i}.eval(species_out(startpoint,:));
                end
                parameters1 = param_modifiers{i}.eval(param_init);
                
                % setting timepoints outputs
                timepoints(startpoint:endpoint) = linspace(t_start,t_end,n_steps+1)';
                % simulating 
                [err1, species1, observables1] = exec(obj,sims,t_sim,n_steps, species_init1,parameters1);
                % setting other outputs
                err(startpoint:endpoint) = err1;
                species_out(startpoint:endpoint,:) = species1;
                observables_out(startpoint:endpoint,:) = observables1;
                
            end                        
        end
        
    end
    
end

