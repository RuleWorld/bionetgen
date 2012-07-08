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
    end
    
end

