classdef distDiscreteUniform < distGeneric
    %DISC_UNIF Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (SetAccess = private)
        Categories
        N
    end
    
    methods
        function obj = distDiscreteUniform(arg)
            assert(min(size(arg)) == 1 && max(size(arg)) > 1, 'Categories should be a vector greater than 1');
            assert(isnumeric(arg), 'Categories should be numeric');
            assert(length(unique(arg)) == length(arg), 'Categories are not unique');
            obj = obj@distGeneric('disc_unif');
            obj.Categories = sort(arg(:))';
            obj.N = length(obj.Categories);
        end
        function p = pdf(obj,x)
            assert(~isempty(find(obj.Categories==x, 1)),'Category not found');
            p = 1/obj.N;
        end
        function p = cdf(obj,x)
            assert(~isempty(find(obj.Categories==x, 1)),'Category not found');
            ind = find(obj.Categories==x);
            p = ind/obj.N;
        end
        function x = invcdf(obj,y)
            ind = ceil(y*obj.N);
            x = obj.Categories(ind);
        end           
    end
    
end

