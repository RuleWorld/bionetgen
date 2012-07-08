classdef bngLabels
    %BNGLABELS Class used to create labels objects for observables, species, parameters and ensemble variables
    %   bngLabels Properties:
    %       labels  -    cell array of strings
    %       N       -    number of strings
    %   bngLabels Methods:
    %       getIndex    -   gets the index of a particular label
    %       getValue    -   picks the value associated with a label from a vector
    %       setValue    -   resets the value associated with a label
    %       setLabel    -   resets the label at a particular index
    %       addLabel    -   adds a label to the object
    %       removeLabel -   removes a label from the object
    
    properties (SetAccess = private)
        labels  % cell array of strings
        N       % number of strings
    end
    
    methods
        function obj = bngLabels(labels)
            assert(iscellstr(labels) && min(size(labels))==1,'Provide a one-dimensional cell array labels');
            obj.labels = labels;
            obj.N = length(labels);
        end
        
        function ind = getIndex(obj,label)
            % bngLabels.getIndex() gets the index of a particular label
            % 
            % USAGE:
            %
            % ind = obj.getIndex(label)
            %
            %   obj - any object of bngLabel class (or any derived class)
            %   label - string, label 
            assert(ischar(label),'Provide a valid label');
            assert(sum(strcmp(obj.labels,label))==1,'Label not found');
            ind = find(strcmp(obj.labels,label));
        end
        
        function val = getValue(obj,vec,label)
            % bngLabels.getValue() picks the value associated with a label
            % from a vector
            % 
            % USAGE:
            %
            % val = obj.getValue(vec,label)
            %
            %   obj - any object of bngLabel class (or any derived class)
            %   vec - vector of numbers
            %   label - string, label 
            assert(min(size(vec))==1 && length(vec) == obj.N,'Improper sizes input');
            ind = obj.getIndex(label);
            val = vec(ind);
        end
        
        function vec1 = setValue(obj,vec,label,val)
            % bngLabels.setValue() resets the value associated with a label
            % from a vector
            % 
            % USAGE:
            %
            % vec1 = obj.setValue(vec,label,val)
            %
            %   obj - any object of bngLabel class (or any derived class)
            %   vec - vector of numbers
            %   label - string, label
            %   val - number, value to reset to
            assert(any(strcmp(obj.labels,label)),'Label not found!');
            assert(isnumeric(val),'Provide a numeric value to set');
            vec1 = vec;
            vec1(obj.getIndex(label)) = val;
        end
        
        function obj1 = setLabel(obj,ind,label)
            % bngLabels.setLabel() resets the label at a particular index
            % 
            % USAGE:
            %
            % obj1 = obj.setLabel(ind,label)
            %
            %   obj - any object of bngLabel class (or any derived class)
            %   ind - number, index
            %   label - string, label

            assert(ind <= obj.N,'Index out of bounds');
            assert(ischar(label),'Label should be a string');
            obj1 = obj;
            obj1.labels(ind) = label;
        end
        
        function obj1 = addLabel(obj,label)
            % bngLabels.addLabel() adds a label to the object
            % 
            % USAGE:
            %
            % obj1 = obj.addLabel(label)
            %
            %   obj - any object of bngLabel class (or any derived class)
            %   label - string, label
            assert(~any(strcmp(label,obj.labels)),'Label already exists! Try a different label');
            obj1 = obj;
            obj1.labels{obj.N+1} = label;
            obj1.N = length(obj1.labels);
        end
        
        function obj1 = removeLabel(obj,label)
            % bngLabels.removeLabel() removes a label from the object
            % 
            % USAGE:
            %
            % obj1 = obj.removeLabel(label)
            %
            %   obj - any object of bngLabel class (or any derived class)
            %   label - string, label           
            assert(any(strcmp(label,obj.labels)),'Label does not exist.');
            new_labels = obj.labels;
            obj1 = obj;
            matches = strcmp(obj.labels,label);
            new_labels(matches) = [];
            obj1.labels = new_labels;
            obj1.N = length(obj1.labels);
        end
    end
    
end

