classdef bngTraj
    %BNGTRAJ Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (SetAccess = private)
        Labels
        timepoints
        val
        stds
    end
    
    methods
        function obj = bngTraj(labels_obj,timepoints,val,stds)
            assert(min(size(timepoints))==1,'Timepoints should be linear in dimension');
            assert((isa(labels_obj,'bngSpecies')|isa(labels_obj,'bngObs')) && isnumeric(timepoints) && isnumeric(val),'Improper types as input');
            assert(~any(size(val)~=[length(timepoints) labels_obj.N]),'Values are of improper size');
            obj.Labels  = labels_obj;
            obj.timepoints = timepoints;
            obj.val = val;
            
            if nargin > 3 && ~isempty(stds)
                assert(~any(size(stds)~=size(val)),'Improper size provided for stds');
                obj.stds = stds;
            else
                obj.stds = zeros(size(val));
            end
        end
        
        function obj = generateDataObject(obj1,label,wt)
            assert(ischar(label),'Provide a valid string as label');
            
            ind = obj1.Labels.getIndex(label);
            
            n = length(obj1.timepoints);
            val = obj1.val(:,ind)';
            stds = obj1.stds(:,ind)';
            if nargin == 2
                wt = ones(1,n);
            end
            annotate = cell(1,n);
            for i=1:1:n
                annotate{i} = strcat(label,' t=',num2str(obj1.timepoints(i)));
            end
            
            obj = bngData();
            obj = obj.addData(val,stds,annotate,wt);
            
        end
        
        function obj2 = filterTimepoints(obj1,timepoints1)
            t_inds = zeros(size(timepoints1));
            temp = 1;
            for i=1:1:length(obj1.timepoints)
                if(any(obj1.timepoints(i)==timepoints1))
                    t_inds(temp) = i;
                    temp = temp+1;
                end
            end
            obj2 =  bngTraj(obj1.Labels,obj1.timepoints(t_inds),obj1.val(t_inds,:),obj1.stds(t_inds,:));
        end 
        
        
    end
    
    methods (Static)
        function obj = trajRead(filename,labels_obj)
            [data,labels] = tblread(filename);
            %assert(~any(~strcmp(labels_obj.labels,labels)));

            c = size(data,2);
            timepoints = data(:,1);
            val = data(:,2:c);
            
            obj = bngTraj(labels_obj,timepoints,val);

        end
        function obj = trajRead_Multiple(filename_array,labels_obj)
            assert(iscellstr(filename_array),'Provide a list of filenames');
            assert(min(size(filename_array))==1,'Provide a linear list');
            num_files = length(filename_array);
            traj_obj = bngTraj.trajRead(filename_array{1},labels_obj);
            [r c] = size(traj_obj.val);
            traj_3d = zeros(r,c,num_files);
            for i=1:1:num_files
                traj_obj = bngTraj.trajRead(filename_array{i},labels_obj);
                traj_3d(:,:,i) = traj_obj.val;
            end
            m = sum(traj_3d,3)/num_files;
            s = std(traj_3d,0,3);
            t = traj_obj.timepoints;
            obj = bngTraj(labels_obj,t,m,s);
        end  
    end
    
    
end

