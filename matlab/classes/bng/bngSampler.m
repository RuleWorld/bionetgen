classdef bngSampler < bngParam
    %BNGSAMPLER derived from bngParam class, used for parameters.
    %   
    %
    %   bngSampler Methods:
    %       bngSampler      -   creates a bngParam object 
    %
    %       genRand         
    %       genMove      -  
    %       calcMoveDist -  
    %   Use 'help bngLabels'  and  'help bngParam' for additional methods inherited from the
    %   superclass
    properties
    end
    
    methods
        function obj = bngSampler(obj1)
            assert(isa(obj1,'bngParam'),'Provide a bngParam object to initialize sampler');
            obj = obj@bngParam(obj1.labels,obj1.defaults);
            for i=1:1:obj1.N
                obj = obj.setDistrib(obj1.labels{i},obj1.Distribs{i});
            end
        end
        
        function [x y] = genRand( obj,n_samples )
            %  obj.genRand() samples using the stored distribution objects
            %  
            %  USAGE:
            %  [x y] = obj.genRand(n_samples)
            %   obj - bngParam object
            %   x   - samples in parameter space,   [row col] = [n_samples obj.N]
            %   y   - samples in unit interval box, [row col] = [n_samples obj.N]
            y = rand(n_samples,obj.N);
            x = obj.invcdf_matr(y);
        end
        
        function  y = genMove(obj, y0,sig)
            % obj.genMove() generates the next move in [0 1] space
            % used for Metropolis-Hastings sampling
            assert(min(size(y0))==1, 'Input should be a vector');
            assert(length(y0) == obj.N,'Vector should be same size as number of parameters');
            y = zeros(1,obj.N);
            i=1;
            proprnd = @(z) distNormal(z,sig).invcdf(rand);
            while i<= obj.N
                    newsample = proprnd(y0(i));
                    if newsample > 0 && newsample < 1
                        y(i) = newsample;
                        i = i+1;
                    end
            end
            %x = obj.invcdf(y);

        end
        
        function p = calcMoveDist(obj,y1,y2,sig)
            % calculate distance in [0 1] space for any two points 
            % use only for Metropolis-Hastings
            assert(min(size(y1)) == 1 && min(size(y2))==1,'Input should be numeric vectors');
            assert(length(y1) == length(y2),'Vectors should be of equal length');
            assert(length(y1) == obj.N,'Vectors should be of length equal to number of parameters');
            diff = zeros(1,obj.N);
            for i=1:1:obj.N
                diff(i) = distNormal(y1(i),sig).pdf(y2(i));
            end
            p = 1;
            for i=1:1:obj.N
                p = p*diff(i);
            end
        end
    end
    
end

