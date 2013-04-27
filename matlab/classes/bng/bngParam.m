classdef bngParam < bngLabels
    %BNGPARAM derived from bngLabels class, used for parameters.
    %   bngParam Properties: (unique to bngParam)
    %       defaults  -    numeric vector [...]
    %       Distribs  -    cell array of distribution objects (derived from
    %                      distGeneric class)
    %
    %   bngParam Methods:
    %       bngParam    -   creates a bngParam object 
    %
    %       setDistrib  -   assigns a distribution object from the distGeneric
    %                           class and derived subclasses
    %
    %       invcdf      -   Uses the stored cdfs to transform a numeric vector 
    %                       from unit interval space to parameter space
    %
    %       invcdf_matr -   Uses the stored cdfs to transform a matrix 
    %                       from unit interval space to parameter space
    %       
    %       genRand     -   samples using the stored distribution objects
	%
	%		paramstruct -	creates a parameter structure to be used for PTEMPEST
    %
    %   Use 'help bngLabels' for additional methods inherited from the
    %   superclass

    

        
    properties (SetAccess = private)
        defaults
        Distribs
    end
    
    methods
        function obj = bngParam(labels,defaults)
            % bngParam() creates a bngParam object
            % USAGE:
            % 
            % obj = bngParam(labels,defaults)   
            %   labels - cell array of parameter names
            %   defaults - numeric vector of default parameter values
            assert(min(size(labels))==1 && iscellstr(labels),'Labels should be a 1-D cell array of strings.');
            assert(min(size(defaults))==1 && isnumeric(defaults),'Default values should be a numeric vector.');
            assert(length(labels)==length(defaults),'Labels and default values should be equal in size.');
            obj = obj@bngLabels(labels);
            obj.defaults = defaults;
            for i=1:1:obj.N
                obj.Distribs{i} = distConstant(obj.defaults(i));
            end 
        end
        
        function obj1 = setDistrib(obj,label,distribObj)
            % obj.setDistrib() assigns a distribution object from the distGeneric class and derived subclasses
            % USAGE:
            % 
            % obj = obj.setDistrib(label,distribObj)
            %   obj         - bngParam object
            %   label       - string,label
            %   distribObj  - distribution object created from derivatives
            %                   of distGeneric class, e.g. distNormal(0,1)
            assert(isa(distribObj,'distGeneric'),'Should provide a valid distribution object')
            obj1 = obj;
            ind = obj1.getIndex(label);
            obj1.Distribs{ind} = distribObj;
        end
		
		function obj1 = createSubset(obj,labels)
			% obj.createSubset() creates another parameters object from a subset of the labels
            % USAGE:
            % 
            % obj = obj.createSubset(labels)
            %   obj         - bngParam object
            %   label       - cell array of strings
            assert(iscellstr(labels),'Provide a cell array of strings as labels');
			inds = zeros(1,length(labels));
			
			for i=1:1:obj.N
				for j=1:1:length(labels)
					if(strcmp(obj.labels{i},labels(j)))
					inds(j) = i;
					end
				end
			end
			defaults = obj.defaults(inds);
			Distribs = obj.Distribs(inds);
			obj1 = bngParam(labels,defaults);
			obj1.Distribs = Distribs;
		end
        
        function xvec = invcdf(obj,yvec)
            % obj.invcdf() uses the stored cdfs to transform a numeric vector from unit interval space to parameter space
            %
            % USAGE:
            %
            % xvec = obj.invcdf(yvec)
            %   obj  - bngParam object
            %   yvec - vector of values in unit interval  
            assert(length(yvec)==obj.N,'Wrong vector size passed to invcdf.');
            assert(sum(yvec>0 & yvec<=1)==obj.N,'Provide values in the unit interval.');
            
            xvec = zeros(1,obj.N);
            for i=1:1:obj.N
                xvec(i) = obj.Distribs{i}.invcdf(yvec(i));
            end
        end
        function xmatr = invcdf_matr(obj,ymatr)
            % obj.invcdf_matr uses the stored cdfs to transform a matrix from unit interval space to parameter space
            %
            % USAGE:
            %
            % xmatr = obj.invcdf(ymatr)
            %   obj   - bngParam object
            %   ymatr - matrix of values in unit interval
            assert(size(ymatr,2) == obj.N, 'Matrix number of columns should equal number of parameters');
            n_samples = size(ymatr,1);
            xmatr = zeros(size(ymatr));
            for i=1:1:n_samples
                xmatr(i,:) = obj.invcdf(ymatr(i,:));
            end
        end
        
        function yvec = cdf(obj,xvec)
            % obj.cdf() uses the stored cdfs to transform a numeric
            % vector from  parameter space to unit interval space
            %
            % USAGE:
            %
            % yvec = obj.cdf(xvec)
            %   obj  - bngParam object
            %   xvec - vector of values 
           
            
            yvec = zeros(1,obj.N);
            for i=1:1:obj.N
                yvec(i) = obj.Distribs{i}.cdf(xvec(i));
            end
        end
        
		function paramstruct = constructParamStruct(obj)
			
			paramstruct = cell(1,obj.N);
			for i=1:1:obj.N
				paramname = obj.labels{i};
				if isa(obj.Distribs{i},'distUniform')
					parammin = obj.Distribs{i}.Min;
					parammax = obj.Distribs{i}.Max;
					paramstruct{i} = struct('name',paramname,'prior','uniform','min',parammin,'max',parammax,'units','');
				else if isa(obj.Distribs{i},'distConstant')
					paramvalue = obj.Distribs{i}.val;
					paramstruct{i} = struct('name',paramname,'prior','point','value',paramvalue,'units','');
					end
				end
					
			end
		end
        
            
        
    end
    
end

