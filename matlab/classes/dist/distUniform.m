classdef distUniform < distGeneric
    %UNIF Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (SetAccess = private)
        Min
        Max
    end
    
    methods
        function obj = distUniform(min,max)
            assert(nargin == 2,'Wrong number of parameters');
            assert(min < max, 'Min should be less than Max');
            obj = obj@distGeneric('unif');
            obj.Min = min;
            obj.Max = max;
        end
        function p = pdf(obj,x)
            if(x < obj.Min || x > obj.Max)
                p = 0;
            else
                p = 1/(obj.Max - obj.Min);
            end
        end
        function y = cdf(obj,x)
            if(x <= obj.Min)
                y = 0;
            else
                if(x > obj.Min && x < obj.Max)
                    y = (x - obj.Min)/(obj.Max - obj.Min);
                else
                    y = 1;
                end
            end
        end
        function x = invcdf(obj,y)
            assert(y>0 && y<=1,'Provide values in the unit interval.');
            x = obj.Min + y*(obj.Max - obj.Min);
        end
        function r = range(obj)
            r = obj.Max - obj.Min;
        end
    end
    
end

