classdef distUniformLog < distGeneric
    %UNIF_LOG Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (SetAccess = private)
        Min
        Max
        LogMin
        LogMax
        Base
    end
    
    methods
        function obj = distUniformLog(min,max,base)
            assert(nargin >= 2 && nargin <=3,'Wrong number of parameters');
            assert( min > 0 && max > 0,'Unif_Log takes only positive numbers as input')
            assert(min < max, 'Min should be less than Max');
            
            obj = obj@distGeneric('unif_log');
            if nargin == 2
                obj.Base = 10;
            else
                assert( base > 0,'Unif_Log takes only positive numbers as input')
                obj.Base = base;
            end

            obj.Min = min;
            obj.Max = max;
            obj.LogMin = log10(obj.Min)/log10(obj.Base);
            obj.LogMax = log10(obj.Max)/log10(obj.Base);
           
               
        end
        function p = pdf(obj,x)
            if(x < obj.Min || x > obj.Max)
                p = 0;
            else
                p = 1/(obj.LogMax - obj.LogMin);
            end
        end
        function y = cdf(obj,x)
            if(x <= obj.Min)
                y = 0;
            else
                if(x > obj.Min && x < obj.Max)
                    logx = log10(x)/log10(obj.Base);
                    y = (logx - obj.LogMin)/(obj.LogMax - obj.LogMin);
                else
                    y = 1;
                end
            end
        end
        function x = invcdf(obj,y)
            assert(y>0 && y<=1,'Provide values in the unit interval.');
            x = obj.Base^(obj.LogMin + y*(obj.LogMax - obj.LogMin));
        end
        function r = log_range(obj)
            r1 = obj.LogMax - obj.LogMin;
            r2 = obj.Base;
            r = [r1 r2];
        end
    end
    
end