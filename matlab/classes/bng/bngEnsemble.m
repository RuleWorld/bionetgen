classdef bngEnsemble
    %BNGENSEMBLE Summary of this class goes here
    %   Detailed explanation goes here
    % data should be of the form: row: sample, column: feature
    
    properties (SetAccess = private)
        Labels
        N
        data
    end
    
    methods
        function obj = bngEnsemble(data,labels)
            assert(iscellstr(labels) && isnumeric(data),'Improper types input');
            assert(min(size(labels))==1 && size(data,2) == length(labels),'Improper sizes');
            obj.Labels = bngLabels(labels);
            obj.data = data;
            obj.N = size(data,1);
        end
        
        function [outs,labels] = getSubset(obj,conditionvec,featurevec)
            % conditionvec matches rows (samples)
            % featurevec matches columns (features)
            assert(islogical(unique(conditionvec)),'Condition vector should be a boolean vector');
            assert(islogical(unique(featurevec)),'Feature vector should be a boolean vector');
            assert(length(conditionvec)==obj.N,'Condition vector is of improper size');
            assert(length(featurevec)==obj.Labels.N,'Feature vector is of improper size');
            outs = obj.data(conditionvec,featurevec);
            labels = obj.Labels.labels(featurevec);
        end
        
        function [outs,labels] = getSamples(obj,conditionvec)
            featurevec = ones(1,obj.Labels.N);
            [outs,labels] = obj.getSubset(conditionvec,featurevec);
        end
        
        function [outs,labels] = getFeatures(obj,labels,conditionvec)
            assert(iscellstr(labels) && min(size(labels))==1,'Feature labels should be a linear vector');
            if nargin < 3
                conditionvec = ones(1,obj.N)>0;
            end
            assert(islogical(unique(conditionvec)),'Condition vector should be a boolean vector');
            assert(length(conditionvec)==obj.N,'Condition vector is of improper size');
            
            featurevec = zeros(1,obj.Labels.N);
            for i=1:1:length(labels)
                assert(any(strcmp(obj.Labels.labels,labels{i})),'Label not found!');
                featurevec(obj.Labels.getIndex(labels{i})) = 1;
            end
            featurevec = featurevec > 0;
            
            [outs,labels] = obj.getSubset(conditionvec,featurevec);                
        end
        
        function obj1 = getSubsetEnsemble(obj,labels,conditionvec)
            if(isempty(labels))
                labels = obj.Labels.labels;
            end
            [outs,labels] = obj.getFeatures(labels,conditionvec);
            obj1 = bngEnsemble(outs,labels);   
        end
        
        function obj1 = addSamples(obj,samples)
            data = [obj.data;samples];
            labels = obj.Labels.labels;
            obj1 = bngEnsemble(data,labels);
        end
        
        function obj1 = addFeatures(obj,features,labels)
            assert(iscellstr(labels) && min(size(labels))==1,'Labels should be a linear cell array of strings');
            assert(size(features,1) == obj.N,'Number of samples inconsistent');
            assert(size(features,2) == length(labels),'Number of new labels inconsistent');
            for i=1:1:length(labels)
                assert(~any(strcmp(obj.Labels.labels,labels{i})),'Label already exists!');
            end
            
            data = [obj.data features];
            labels = [obj.Labels.labels labels];
            obj1 = bngEnsemble(data,labels);
        end
        
        function obj1 = removeSamples(obj,conditionvec)
            assert(islogical(unique(conditionvec)),'Condition vector should be a boolean vector');
            assert(length(conditionvec)==obj.N,'Condition vector is of improper size');
            obj1 = obj.getSubsetEnsemble(obj.Labels.labels,~conditionvec);
        end
        
        function obj1 = removeFeatures(obj,labels)
            assert(iscellstr(labels) && min(size(labels))==1,'Feature labels should be a linear vector');
            for i=1:1:length(labels)
                assert(any(strcmp(obj.Labels.labels,labels{i})),'Label does not exist!');
            end
            featurevec = ones(1,length(obj.Labels.labels));
            for i=1:1:length(labels)
                ind = obj.Labels.getIndex(labels{i});
                featurevec(ind) = 0;
            end
            featurevec = featurevec > 0;
            conditionvec = 1:1:obj.N > 0;
            [outs,new_labels] = getSubset(obj,conditionvec,featurevec);
            obj1 = bngEnsemble(outs,new_labels);
        end
    end
    
end

