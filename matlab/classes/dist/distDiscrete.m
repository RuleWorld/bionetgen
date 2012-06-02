classdef distDiscrete < distGeneric
    %DISC Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (SetAccess = private)
        Categories
        PDF
        CDF
        N
    end
    
    methods
        function obj = distDiscrete(categ,probs,opt)
            assert(min(size(categ))==1,'Categories should be a vector.');
            assert(min(size(probs))==1,'Probabilities should be a vector.');
            assert(length(categ) == length(probs),'Number of categories should be equal to number of probabilities.');
            assert(length(categ) > 1, 'There should be atleast two categories.');
            assert(length(categ) == length(unique(categ)),'Repeats not allowed in categories.');
            if(nargin == 3)
                if(~strcmp(opt,'renormalize'))
                    assert(sum(probs)==1,'Probabilities should sum to one. If not, use renormalize option.');
                end
            else
                opt = 'none';
            end
            obj = obj@distGeneric('disc');
            obj.Categories = sort(categ(:))';
            obj.N = length(obj.Categories);
            obj.PDF = zeros(1,obj.N);
            for i=1:1:obj.N
                ind = find(categ == obj.Categories(i));
                obj.PDF(ind) = probs(ind);
            end
            tempsum = sum(obj.PDF);
            if(strcmp(opt,'renormalize'))
                for i=1:1:obj.N
                  obj.PDF(i) = obj.PDF(i)/tempsum;
                end
            end
            obj.CDF = obj.PDF;
            for i=2:1:obj.N
                obj.CDF(i) = obj.CDF(i) + obj.CDF(i-1);
            end
        end
        function p = pdf(obj,x)
            assert(~isempty(find(obj.Categories==x, 1)),'Category not found');
            p = obj.PDF(obj.Categories==x);
        end
        function p = cdf(obj,x)
            assert(~isempty(find(obj.Categories==x, 1)),'Category not found');
            p = obj.CDF(obj.Categories==x);
        end
        function x = invcdf(obj,y)
            assert(y>0 && y<=1,'Provide values in the unit interval.');
            for i=1:1:obj.N
                if(i==1)
                    low = 0;
                else
                    low = obj.CDF(i-1);
                end
                high = obj.CDF(i);
                if(y > low && y<=high)
                    x = obj.Categories(i);
                    return
                end
            end
        end
        
    end 
end

