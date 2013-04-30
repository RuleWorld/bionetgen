classdef bngObs < bngLabels
    %BNGOBS derived from bngLabels class, used for observables.
    %   Use 'help bngLabels' for additional properties and methods inherited from the
    %   superclass
    
    properties
    end
    
    methods
        function obj = bngObs(labels)
            obj = obj@bngLabels(labels);
        end
		
		function obs_struct = createObsStruct(obj)
			obs_struct = cell(1,obj.N);
			for i=1:1:obj.N
				obsname = obj.labels{i};
				obs_struct{i} = struct('name',obsname,'units','','display',1);		
			end
		end
    end
    
end

