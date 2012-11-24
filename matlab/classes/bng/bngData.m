classdef bngData
    %BNGDATA object used for handling data
    % bngData Properties:
    %   N           - number of data points
    %   val         - numeric vector, data values
    %   stds        - numeric vector, standard deviations (default = 0)
    %   annotate    - cell array of strings, anotations
    %   wt          - numeric vector, weights (default = 1)
    % bngData Methods:
    %   bngData         - creates an empty bngData object
    %   addDataObject   - concatenates a bngData object to another
    %   addData         - adds a vector of data-points
    %   computeFit      - computes fit between two data objects
    
    properties
       N           % number of data points
       val         % numeric vector, data values
       stds        % numeric vector, standard deviations
       annotate    % cell array of strings, anotations
       wt          % numeric vector, weights (default = 1)
    end
    
    methods
        function obj = bngData()
            %bngData() creates an empty data oobject.
            %USAGE: 
            %
            %obj = bngData()
            
            obj.N = 0;
            obj.annotate = {};
        end
        
        function obj = addDataObject(obj1,obj2)
            %bngData.addDataObject() concatenates a bngData object to another
            %USAGE: 
            %
            %obj = obj1.addDataObject(obj2)
            %   obj1,obj2   - bngData objects
            obj = bngData();
            assert(isa(obj1,'bngData')&&isa(obj2,'bngData'),'Should provide bngData objects as input');
            obj.N = obj1.N + obj2.N;
            obj.val = [obj1.val obj2.val];
            obj.stds = [obj1.stds obj2.stds];
            obj.annotate = cell(1,obj1.N+obj2.N);
            if obj1.N > 0
                obj.annotate(1:obj1.N) = obj1.annotate;
            end
            obj.annotate((obj1.N+1):(obj1.N+obj2.N)) = obj2.annotate;
            obj.wt = [obj1.wt obj2.wt];
            
        end
        
        function obj = addData(obj1,val,stds,annotate,wt)
            %bngData.addData() adds a vector of data-points
            %USAGE: 
            %
            %obj = obj1.addData(val,stds,annotate,wt)
            %   obj1    - bngData object
            %   val     - numeric vector of values to be added
            %   stds    - numeric vector of standard deviations, default=0
            %   annotate- cell array of strings, annotations
            %   wt      - numeric vector of weights, default = 1
            obj = bngData();
            assert(isnumeric(val) && min(size(val))==1,'Values should be a vector of numeric values');
            N = length(val);
            
            if(isempty(stds))
                stds = zeros(size(val));
            end
            if(isempty(annotate))
                annotate = cell(size(val));
                for i=1:1:N
                    annotate{i} = '';
                end
            end
            if (isempty(wt))
                wt =ones(size(val));
            end
            
            
            assert(isnumeric(stds) && min(size(stds))==1 && ~any(stds<0),'Standard deviations should be a vector of non-negative numbers');
            assert(iscellstr(annotate) & min(size(annotate))==1,'Annotations should be a cell array of strings');
            assert(isnumeric(wt) && min(size(wt))==1 && ~any(wt<0),'Weights should be a vector of non-negative numbers');
            assert(length(stds)==N && length(annotate)==N && length(wt)==N,'All vectors should be of equal length');
            
            obj.N = obj1.N + N;
            obj.val = [obj1.val val(:)'];
            obj.stds = [obj1.stds stds(:)'];
            obj.annotate(1:obj1.N) = obj1.annotate;
            obj.annotate((obj1.N+1):(obj1.N + N))= annotate;
            obj.wt = [obj1.wt wt(:)'];  
        end
        
        function fit = computeFit(obj1,obj2)
            %bngData.computeFit() computes fit between two data objects
            %USAGE: 
            %
            %fit = obj1.addData(obj2)
            %   obj1,obj2    - bngData objects
            
            assert(isa(obj1,'bngData') && isa(obj2,'bngData'));
            assert(any(obj1.stds~=0),'Provide non-zero standard deviations');
            assert(obj1.N==obj2.N,'Data objects are not compatible');
            fit = 0;
            for i=1:1:obj1.N
                if(obj1.stds(i) ~= 0)
                    fit = fit + obj1.wt(i)*(((obj1.val(i) - obj2.val(i))./obj1.stds(i))^2);
                end
            end
        end
      
    end
    
    methods (Static)
        function obj = computeMean(bngDataCellArray,conditionvec)
            assert(iscell(bngDataCellArray), 'Provide a cell array of bngData objects');
            assert(length(conditionvec)==length(bngDataCellArray));
            
            inds = find(conditionvec);
            N_dataobj = length(bngDataCellArray);
            N_points = bngDataCellArray{1}.N;
            val_new = zeros(length(inds),N_points);
            for j=1:1:length(inds)
                i = inds(j);
                val_new(j,:) = bngDataCellArray{i}.val;
            end
            vals1 = mean(val_new,1);
            stds1 = std(val_new,0,1);
            annotate1 = bngDataCellArray{1}.annotate;
            obj = bngData();
            obj = obj.addData(vals1,stds1,annotate1,[]);
        end
        
    end
    
end

