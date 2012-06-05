classdef bngSpeciesActions < bngSpecies
    %BNGSPECIESACTIONS derived from bngSpecies, used for actions that change
    %species concentrations
    %   bngSpeciesActions Properties:
    %       n_edits     - Number of edit actions
    %       opns        - Available operations: add remove change
    %       edit_inds   - Indices of species to be edited
    %       edit_opns   - Specific operations to be performed
    %       edit_vals   - Values to which edited parameters are set
    %
    %   bngParamActions Methods:
    %       bngSpeciesActions - Creates a bngParamActions object
    %       addGenericModifier - Adds a generic action
    %       addConcentration - Increments species concentration by a value
    %       removeConcentration - Decrements species concentration by a
    %                               value
    %       changeConcentration - Changes species concentration to a new
    %                               value
    %       eval            - Given a vector of numbers, evaluates the
    %                           associated parameter actions
    %   Use 'help bngLabels' and 'help bngSpecies' for additional methods 
    %   inherited from the superclasses
    
    properties
        n_edits     % Number of edit actions
        opns        % Available operations: add remove change
        edit_inds   % Indices of species to be edited
        edit_opns   % Specific operations to be performed
        edit_vals   % Values to which edited parameters are set
    end
    
    methods
        function obj = bngSpeciesActions(obj2)
            % bngSpeciesActions() creates a bngSpeciesActions object
            % USAGE:
            % 
            % obj = bngSpeciesActions(obj2)   
            %   obj2 - bngSpecies object
            obj = obj@bngSpecies(obj2.labels,obj2.initMethod);
            obj.n_edits = 0;
            obj.edit_inds = [];
            obj.edit_opns = {};
            obj.edit_vals = [];
            obj.opns = {'add','remove','change'};
        end
        
        function obj = addGenericModifier(obj,opn,label,value)
            % obj.addGenericModifier() creates a generic modifier action
            % USAGE:
            % 
            % obj1 = obj.addGenericModifier(opn,label,value)
            %   obj         - bngSpeciesActions object
            %   opn         - string, operation name
            %   label       - string,label
            %   value       - value, number
            assert(sum(strcmp(obj.opns,opn))>0,'SpeciesModifier operation not found!');
            assert(isnumeric(value),'Provide only numeric values or expressions');
            n_edits = obj.n_edits + 1;
            
            obj.edit_inds(n_edits) = obj.getIndex(label);
            obj.edit_opns{n_edits} = opn;
            obj.edit_vals(n_edits) = value;
            obj.n_edits = n_edits;
        end
        function obj = addConcentration(obj,label,value)
            % obj.addConcentration() increments a species concentration
            % USAGE:
            % 
            % obj1 = obj.addConcentration(label,value)
            %
            %   label       - string,label
            %   value       - value by which to increment, number
            assert(value>=0,'Only positive concentrations should be provided');
            obj = obj.addGenericModifier('add',label,value);
        end
        
        function obj = removeConcentration(obj,label,value)
            % obj.removeConcentration() decrements a species concentration
            % USAGE:
            % 
            % obj1 = obj.removeConcentration(label,value)
            %
            %   label       - string,label
            %   value       - value by which to decrement, number
            assert(value>=0,'Only positive concentrations should be provided');
            obj = obj.addGenericModifier('remove',label,value);
        end
        
        function obj = changeConcentration(obj,label,value)
            % obj.changeConcentration() changes a species concentration to
            % an arbitrary value
            % USAGE:
            % 
            % obj1 = obj.changeConcentration(label,value)
            %
            %   label       - string,label
            %   value       - value to be set, number
            assert(value>=0,'Only positive concentrations should be provided');
            obj = obj.addGenericModifier('change',label,value);
        end
        % Capacity to update species_init based on seed species definitions
        % is not provided due to complications
        % manually evaluate changed parameters, then use
        % changeConcentration and reinitalizeOne methods as follows:
        % obj = obj.changeConcentration(label,obj.initializeOne(label,params);

        function out_species_vec = eval(obj,in_species_vec)
            % obj.eval() Given a vector of numbers, evaluates the
            % associated species actions
            % USAGE:
            % 
            % out_species_vec = obj.eval(in_species_vec)
            %   obj             - bngSpeciesActions object
            %   in_species_vec    - vector of numbers
            
            assert(min(size(in_species_vec))==1 && length(in_species_vec) == obj.N,'Species vector not the right length');
            out_species_vec = in_species_vec;
            if obj.n_edits == 0,return,end
            
            for i=1:1:obj.n_edits
                if(strcmp(obj.edit_opns{i},'add'))
                    out_species_vec(obj.edit_inds(i)) = out_species_vec(obj.edit_inds(i)) + obj.edit_vals(i);
                end
                if(strcmp(obj.edit_opns{i},'remove'))
                    out_species_vec(obj.edit_inds(i)) = out_species_vec(obj.edit_inds(i)) - obj.edit_vals(i);
                    if (out_species_vec(obj.edit_inds(i))<0)
                        out_species_vec(obj.edit_inds(i)) = 0;
                    end
                end
                if(strcmp(obj.edit_opns{i},'change'))
                    out_species_vec(obj.edit_inds(i)) = obj.edit_vals(i);
                end
            end
                    
        end
        
        
    end
    
    
    
end

