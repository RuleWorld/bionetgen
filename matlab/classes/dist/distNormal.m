classdef distNormal < distGeneric
    %GAUSS Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (SetAccess = private)
        Mu
        Sigma
    end
    
    methods
        function obj = distNormal(mu,sigma)
            assert(nargin == 2,'Wrong number of parameters');
            assert(sigma > 0,'Sigma has to be positive');
            obj = obj@distGeneric('norm');
            obj.Mu = mu;
            obj.Sigma = sigma;            
        end
        function p = pdf(obj,x)
            p = normpdf(x,obj.Mu,obj.Sigma);
        end
        function p = cdf(obj,x)
            p = normcdf(x,obj.Mu,obj.Sigma);
        end
        function x = invcdf(obj,y)
            assert(y>0 && y<1,'Provide values in the unit interval.');
            x = norminv(y,obj.Mu,obj.Sigma);
        end
        
    end
    
end

