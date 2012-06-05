classdef distDiscreteInterval < distGeneric
    %DISC_INTERVAL Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (SetAccess = private)
        PDF
        CDF
        N
        Lower
        Upper
    end
    
    methods
        function obj = distDiscreteInterval(lower,upper,probs,opt)
            assert(min(size(lower))==1 && min(size(upper))==1,'Lower and upper interval bounds should be a vector.');
            assert(min(size(probs))==1,'Probabilities should be a vector.');
            assert(length(lower)==length(upper),'Lower and upper interval bound vectors should be equal in size');
            assert(length(lower) == length(probs),'Number of intervals should be equal to number of probabilities.');
            assert(length(lower) > 1, 'There should be atleast two intervals.');
            assert(length(lower) == length(unique(lower)),'Intervals cannot overlap.');
            assert(length(upper) == length(unique(upper)),'Intervals cannot overlap.');
            assert(isempty(find(lower~=sort(lower))),'Lower bounds need to be ordered');
            assert(isempty(find(upper~=sort(upper))),'Upper bounds need to be ordered');
            if(nargin == 3)
                assert(sum(probs)==1,'Probabilities should sum to one. If not, use renormalize option.');
            else
                if(~strcmp(opt,'renormalize'))
                    assert(sum(probs)==1,'Probabilities should sum to one. If not, use renormalize option.');
                end
                
            end
            obj = obj@distGeneric('disc_interval');
            obj.Lower = lower;
            obj.Upper = upper;
            obj.N = length(lower);
            obj.PDF = zeros(1,obj.N);
            for i=1:1:obj.N
                obj.PDF(i) = probs(i);
            end
            tempsum = sum(obj.PDF);
            if(strcmp(opt,'renormalize'))
                disp('Hi')
                for i=1:1:obj.N
                  obj.PDF(i) = obj.PDF(i)/obj.N;
                end
            end
            obj.CDF = obj.PDF;
            for i=2:1:obj.N
                obj.CDF(i) = obj.CDF(i) + obj.CDF(i-1);
            end
        end
        
        function p = pdf(obj,x)
            for i=1:l:obj.N
                if checkInterval(x,lower(i),upper(i))
                    p = obj.PDF(i);
                    return
                end
            end
        end
       
        function p = cdf(obj,x)
            for i=1:l:obj.N
                if checkInterval(x,obj.Lower(i),obj.Upper(i))
                    p = obj.CDF(i);
                    return
                end
            end
        end
        
       function [x1 x2] = invcdf(obj,y)
            assert(y>0 && y<=1,'Provide values in the unit interval.');
            for i=1:1:obj.N
                if(i==1)
                    low = 0;
                else
                    low = obj.CDF(i-1);
                end
                high = obj.CDF(i);
                if(y > low && y<=high)
                    x1 = obj.Lower(i);
                    x2 = obj.Upper(i);
                    return
                end
            end
        end
        
        
    end
    
end

function y = checkInterval(x,lower,upper)
    if x >= lower && x < upper
        y =1;
    else
        y=0;
    end
end