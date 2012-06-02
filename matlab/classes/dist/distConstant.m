classdef distConstant < distGeneric
    %DISTCONST Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (SetAccess = private)
        val       
    end
    
    methods
        function obj = distConstant(num)
            assert(isnumeric(num) && length(num)==1,'Constant distribution takes only single numbers');
            obj = obj@distGeneric('const');
            obj.val = num;
        end
        % CDF and PDF not implemented
        function x = invcdf(obj,y)
            assert(y>0 && y<=1,'Provide values in the unit interval.');
            x = obj.val;
        end         
    end
    
end

