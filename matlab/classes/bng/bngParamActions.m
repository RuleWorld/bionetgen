classdef bngParamActions < bngParam
    %BNGPARAMACTIONS derived from bngParam, used for actions that change
    %parameters
    %   bngParamActions Properties:
    %       n_edits     - Number of edit actions
    %       edit_inds   - Indices of parameters to be edited
    %       edit_vals   - Values to which edited parameters are set
    %
    %   bngParamActions Methods:
    %       bngParamActions - Creates a bngParamActions object
    %       setParameter    - Creates a "set Parameter" action
    %       eval            - Given a vector of numbers, evaluates the
    %                           associated parameter actions
    %   Use 'help bngLabels' and 'help bngParam' for additional methods 
    %   inherited from the superclasses
    
    properties
        n_edits % Number of edit actions
        edit_inds % Indices of parameters to be edited
        edit_vals % Values to which edited parameters are set
    end
    
    methods
        function obj = bngParamActions(obj2)
            % bngParamActions() creates a bngParamActions object
            % USAGE:
            % 
            % obj = bngParamActions(obj2)   
            %   obj2 - bngParam object
            
            obj = obj@bngParam(obj2.labels,obj2.defaults);
            obj.n_edits = 0;
            obj.edit_inds = [];
            obj.edit_vals = [];
        end
        
        function obj1 = setParameter(obj,label,value)
            % obj.setParameter() creates a setParameter action that 
            % changes the value assigned to a
            % particular parameter from a vector of values.
            % USAGE:
            % 
            % obj1 = obj.setParameter(label,value)
            %   obj         - bngParamActions object
            %   label       - string,label
            %   value       - value to be set, number
            assert(isnumeric(value),'Provide only numeric values or expressions');
            n_edits = obj.n_edits + 1;
            
            obj1 = obj;
            obj1.edit_inds(n_edits) = obj.getIndex(label);
            obj1.edit_vals(n_edits) = value;
            obj1.n_edits = n_edits;
        end
        
        function out_param_vec = eval(obj,in_param_vec)
            % obj.eval() Given a vector of numbers, evaluates the
            % associated parameter actions
            % USAGE:
            % 
            % out_param_vec = obj.eval(in_param_vec)
            %   obj             - bngParamActions object
            %   in_param_vec    - vector of numbers
            
            assert(min(size(in_param_vec))==1 && length(in_param_vec) == obj.N,'Param vector not the right length');
            out_param_vec = in_param_vec;
            if obj.n_edits == 0,return,end
            
            for i=1:1:obj.n_edits
                label = obj.labels{obj.edit_inds(i)};
                out_param_vec = obj.setValue(out_param_vec,label,obj.edit_vals(i));
            end
                    
        end
        
    end
    
end

