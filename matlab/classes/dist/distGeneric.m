classdef distGeneric 
    properties (SetAccess = private)
        type
    end
    
    methods
        function obj = distGeneric(name)
            namelist = {'const','unif','unif_log','norm','disc','disc_unif','disc_interval'};
            assert(sum(strcmp(namelist,name))>0,'Distribution not found!');
            obj.type = name;
        end
    end

end