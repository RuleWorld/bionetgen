package BNGModel;
# Author: John Sekar
# contains writeMfile_all
# output: model m-file, model class m-file, species-init m-file


# pragmas
use strict;
use warnings;

# Write model to a MATLAB M-file
sub writeMfile_all
{	
	my $model = shift @_;
	my $params = @_ ? shift @_ : {};

    # a place to hold errors
    my $err;

    # nothing to do if NO_EXEC is true
	return '' if $BNGModel::NO_EXEC;

    # nothing to do if there are no reactions
	unless ( $model->RxnList )
	{
	    return ( "writeMfile() has nothing to do: no reactions in current model.\n"
	            ."  Did you remember to call generate_network() before attempting to\n"
	            ."  write network output?");
	}

    # get reference to parameter list
	my $plist = $model->ParamList;

	
	# get model name
	my $model_name = $model->Name;

    # Build output file name
	# ..use prefix if defined, otherwise use model name
	my $prefix = ( defined $params->{prefix} ) ? $params->{prefix} : $model->getOutputPrefix();
	# ..add suffix, if any
	my $suffix = ( defined $params->{suffix} ) ? $params->{suffix} : undef;
	if ( $suffix )
	{   $prefix .= "_${suffix}";   }

    # split prefix into volume, path and filebase
    my ($vol, $path, $filebase) = File::Spec->splitpath($prefix);
    
	# define m-script file name
    my $mscript_filebase = "${filebase}";
    my $mscript_filename = "${mscript_filebase}.m";
	my $mscript_path     = File::Spec->catpath($vol,$path,$mscript_filename);
    my $mscript_filebase_caps = uc $mscript_filebase;

    # configure options (see Matlab documentation of functions ODESET and ODE15S)
    my $odeset_abstol = 1e-4;
    if ( exists $params->{'atol'} )
    {   $odeset_abstol = $params->{'atol'};  }
    
    my $odeset_reltol = 1e-8;
    if ( exists $params->{'rtol'} )
    {   $odeset_reltol = $params->{'rtol'};  } 

    my $odeset_stats = 'off';
    if ( exists $params->{'stats'} )
    {   $odeset_stats = $params->{'stats'};  } 

    my $odeset_bdf = 'off';
    if ( exists $params->{'bdf'} )
    {   $odeset_bdf = $params->{'bdf'};  }

    my $odeset_maxorder = 5;
    if ( exists $params->{'maxOrder'} )
    {   $odeset_maxorder = $params->{'maxOrder'};  } 

    # time options for mscript
    my $t_start = 0;
    if ( exists $params->{'t_start'} )
    {   $t_start = $params->{'t_start'};  }  

    my $t_end = 10;
    if ( exists $params->{'t_end'} )
    {   $t_end = $params->{'t_end'};  } 

    my $n_steps = 20;
    if ( exists $params->{'n_steps'} )
    {   $n_steps = $params->{'n_steps'};  } 

    # configure time step dependent options
    my $odeset_maxstep = undef;
    if ( exists $params->{'max_step'} )
    {   $odeset_maxstep = $params->{'max_step'};  }     
    
    # construct ODESET function call
    my $mscript_call_odeset;
    if ( defined $odeset_maxstep )
    {
        $mscript_call_odeset = "opts = odeset( 'RelTol',   $odeset_reltol,   ...\n"
                              ."               'AbsTol',   $odeset_abstol,   ...\n"
                              ."               'Stats',    '$odeset_stats',  ...\n"
                              ."               'BDF',      '$odeset_bdf',    ...\n"
                              ."               'MaxOrder', $odeset_maxorder, ...\n"
                              ."               'MaxStep',  $odeset_maxstep    );\n";
    }
    else
    {
        $mscript_call_odeset = "opts = odeset( 'RelTol',   $odeset_reltol,   ...\n"
                              ."               'AbsTol',   $odeset_abstol,   ...\n"
                              ."               'Stats',    '$odeset_stats',  ...\n"
                              ."               'BDF',      '$odeset_bdf',    ...\n"
                              ."               'MaxOrder', $odeset_maxorder   );\n";    
    }

    # Index parameters associated with Constants, ConstantExpressions and Observables
    ($err) = $plist->indexParams();
    if ($err) { return $err };

    # and retrieve a string of expression definitions
    my $n_parameters = $plist->countType( 'Constant' );
    my $n_expressions = $plist->countType( 'ConstantExpression' ) + $n_parameters;
    (my $calc_expressions_string, $err) = $plist->getMatlabExpressionDefs();    
    if ($err) { return $err };

    # get list of parameter names and defintions for matlab
	my $mscript_param_names;
	my $mscript_param_values;
	($mscript_param_names, $mscript_param_values, $err) = $plist->getMatlabConstantNames();
    if ($err) { return $err };

    # get number of species
    my $n_species = scalar @{$model->SpeciesList->Array};
     
	# retrieve a string of observable definitions
    my $n_observables = scalar @{$model->Observables};
    my $calc_observables_string;
    ($calc_observables_string, $err) = $plist->getMatlabObservableDefs();
    if ($err) { return $err };
    
    # get list of observable names for matlab
	my $mscript_observable_names;
	($mscript_observable_names, $err) = $plist->getMatlabObservableNames();
    if ($err) { return $err };
    
    # Construct user-defined functions
    my $user_fcn_declarations = '';
    my $user_fcn_definitions  = '';
	foreach my $param ( @{ $model->ParamList->Array } )
	{
		if ( $param->Type eq 'Function' )
		{
		    # get reference to the actual Function
		    my $fcn = $param->Ref;
		    
		    # don't write function if it depends on a local observable evaluation (this is useless
		    #   since CVode can't do local evaluations)
		    next if ( $fcn->checkLocalDependency($plist) );
		    		    
		    # get function definition			    
		    my $fcn_defn = $fcn->toMatlabString( $plist, {fcn_mode=>'define', indent=>''} );

		    # add definition to the user_fcn_definitions string
		    $user_fcn_definitions .= $fcn_defn . "\n";
        }
	}
	
    # index reactions
    ($err) = $model->RxnList->updateIndex( $plist );
    if ($err) { return $err };

	# retrieve a string of reaction rate definitions
	my $n_reactions = scalar @{$model->RxnList->Array};
    my $calc_ratelaws_string;
    ($calc_ratelaws_string, $err) = $model->RxnList->getMatlabRateDefs( $plist );
    if ($err) { return $err };
    

    # get stoichiometry matrix (sparse encoding in a hashmap)
	my $stoich_hash = {};
	($err) = $model->RxnList->calcStoichMatrix( $stoich_hash );

	# retrieve a string of species deriv definitions
    my $calc_derivs_string;
    ($calc_derivs_string, $err) = $model->SpeciesList->toMatlabString( $model->RxnList, $stoich_hash, $plist );
    if ($err) { return $err };   	


    # get list of species names and initial value expressions for matlab
	my $mscript_species_names;
	my $mscript_species_init;
	($mscript_species_names, $mscript_species_init, $err) = $model->SpeciesList->getMatlabSpeciesNames( $model );
    if ($err) { return $err }; 


    ## Set up MATLAB Plot
    # fontsizes
    my $title_fontsize = 14;
    my $axislabel_fontsize = 12;
    my $legend_fontsize = 10;

    # generate code snippets for plotting observables or species
    my $mscript_plot_labels;
    my $mscript_make_plot;

    # get ylabel (either Number of Concentration)
    my $ylabel;
    if ( $model->SubstanceUnits eq 'Number' )
    {   $ylabel = 'number';   }
    elsif ( $model->SubstanceUnits eq 'Concentration' )
    {   $ylabel = 'concentration';   }
    else
    {   return "writeMfile(): I could not identify model substance units!";   }

    
    if ( @{$model->Observables} )
    {   # plot observables
        $mscript_plot_labels = "    observable_labels = { $mscript_observable_names };\n";
        
        $mscript_make_plot = "    plot(timepoints,observables_out);\n"
                            ."    title('${mscript_filebase} observables','fontSize',${title_fontsize},'Interpreter','none');\n"
                            ."    axis([${t_start} timepoints(end) 0 inf]);\n"
                            ."    legend(observable_labels,'fontSize',${legend_fontsize},'Interpreter','none');\n"
                            ."    xlabel('time','fontSize',${axislabel_fontsize},'Interpreter','none');\n"
                            ."    ylabel('${ylabel}','fontSize',${axislabel_fontsize},'Interpreter','none');\n";
    
    }
    else
    {   # plot species
        $mscript_plot_labels = "    species_labels = { $mscript_species_names };\n";
    
        $mscript_make_plot = "    plot(timepoints,species_out);\n"
                            ."    title('${mscript_filebase} species','fontSize',${title_fontsize},'Interpreter','none');\n"
                            ."    axis([${t_start} timepoints(end) 0 inf]);\n"
                            ."    legend(species_labels,'fontSize',${legend_fontsize},'Interpreter','none');\n"
                            ."    xlabel('time','fontSize',${axislabel_fontsize},'Interpreter','none');\n"
                            ."    ylabel('${ylabel}','fontSize',${axislabel_fontsize},'Interpreter','none');\n";
    }
    


    # open Mfile and begin printing...
	open( Mscript, ">$mscript_path" ) || die "Couldn't open $mscript_path: $!\n";
    print Mscript <<"EOF";
function [err, timepoints, species_out, observables_out ] = ${mscript_filebase}( timepoints, species_init, parameters, suppress_plot )
%${mscript_filebase_caps} Integrate reaction network and plot observables.
%   Integrates the reaction network corresponding to the BioNetGen model
%   '${model_name}' and then (optionally) plots the observable trajectories,
%   or species trajectories if no observables are defined. Trajectories are
%   generated using either default or user-defined parameters and initial
%   species values. Integration is performed by the MATLAB stiff solver
%   'ode15s'. ${mscript_filebase_caps} returns an error value, a vector of timepoints,
%   species trajectories, and observable trajectories.
%   
%   [err, timepoints, species_out, observables_out]
%        = $mscript_filebase( timepoints, species_init, parameters, suppress_plot )
%
%   INPUTS:
%   -------
%   species_init    : row vector of $n_species initial species populations.
%   timepoints      : column vector of time points returned by integrator.
%   parameters      : row vector of $n_parameters model parameters.
%   suppress_plot   : 0 if a plot is desired (default), 1 if plot is suppressed.
%
%   Note: to specify default value for an input argument, pass the empty array.
%
%   OUTPUTS:
%   --------
%   err             : 0 if the integrator exits without error, non-zero otherwise.
%   timepoints      : a row vector of timepoints returned by the integrator.
%   species_out     : array of species population trajectories
%                        (columns correspond to species, rows correspond to time).
%   observables_out : array of observable trajectories
%                        (columns correspond to observables, rows correspond to time).
%
%   QUESTIONS about the BNG Mfile generator?  Email justinshogg\@gmail.com



%% Process input arguments

% define any missing arguments
if ( nargin < 1 )
    timepoints = [];
end

if ( nargin < 2 )
    species_init = [];
end

if ( nargin < 3 )
    parameters = [];
end

if ( nargin < 4 )
    suppress_plot = 0;
end


% initialize outputs (to avoid error msgs if script terminates early
err = 0;
species_out     = [];
observables_out = [];


% setup default parameters, if necessary
if ( isempty(parameters) )
   parameters = [ $mscript_param_values ];
end
% check that parameters has proper dimensions
if (  size(parameters,1) ~= 1  |  size(parameters,2) ~= $n_parameters  )
    fprintf( 1, 'Error: size of parameter argument is invalid! Correct size = [1 $n_parameters].\\n' );
    err = 1;
    return;
end

% setup default initial values, if necessary
if ( isempty(species_init) )
   species_init = initialize_species( parameters );
end
% check that species_init has proper dimensions
if (  size(species_init,1) ~= 1  |  size(species_init,2) ~= $n_species  )
    fprintf( 1, 'Error: size of species_init argument is invalid! Correct size = [1 $n_species].\\n' );
    err = 1;
    return;
end

% setup default timepoints, if necessary
if ( isempty(timepoints) )
   timepoints = linspace($t_start,$t_end,$n_steps+1)';
end
% check that timepoints has proper dimensions
if (  size(timepoints,1) < 2  |  size(timepoints,2) ~= 1  )
    fprintf( 1, 'Error: size of timepoints argument is invalid! Correct size = [t 1], t>1.\\n' );
    err = 1;
    return;
end

% setup default suppress_plot, if necessary
if ( isempty(suppress_plot) )
   suppress_plot = 0;
end
% check that suppress_plot has proper dimensions
if ( size(suppress_plot,1) ~= 1  |  size(suppress_plot,2) ~= 1 )
    fprintf( 1, 'Error: suppress_plots argument should be a scalar!\\n' );
    err = 1;
    return;
end

% define parameter labels (this is for the user's reference!)
param_labels = { $mscript_param_names };



%% Integrate Network Model
 
% calculate expressions
[expressions] = calc_expressions( parameters );

% set ODE integrator options
$mscript_call_odeset

% define derivative function
rhs_fcn = @(t,y)( calc_species_deriv( t, y, expressions ) );

% simulate model system (stiff integrator)
try 
    [timepoints, species_out] = ode15s( rhs_fcn, timepoints, species_init', opts );
catch
    err = 1;
    fprintf( 1, 'Error: some problem encounteredwhile integrating ODE network!\\n' );
    return;
end

% calculate observables
observables_out = zeros( length(timepoints), $n_observables );
for t = 1 : length(timepoints)
    observables_out(t,:) = calc_observables( species_out(t,:), expressions );
end


%% Plot Output, if desired

if ( ~suppress_plot )
    
    % define plot labels
$mscript_plot_labels
    % construct figure
$mscript_make_plot
end


%~~~~~~~~~~~~~~~~~~~~~%
% END of main script! %
%~~~~~~~~~~~~~~~~~~~~~%


% initialize species function
function [species_init] = initialize_species( params )

    species_init = zeros(1,$n_species);
$mscript_species_init
end


% user-defined functions
$user_fcn_definitions


% Calculate expressions
function [ expressions ] = calc_expressions ( parameters )

    expressions = zeros(1,$n_expressions);
$calc_expressions_string   
end



% Calculate observables
function [ observables ] = calc_observables ( species, expressions )

    observables = zeros(1,$n_observables);
$calc_observables_string
end


% Calculate ratelaws
function [ ratelaws ] = calc_ratelaws ( species, expressions, observables )

    ratelaws = zeros(1,$n_observables);
$calc_ratelaws_string
end

% Calculate species derivates
function [ Dspecies ] = calc_species_deriv ( time, species, expressions )
    
    % initialize derivative vector
    Dspecies = zeros($n_species,1);
    
    % update observables
    [ observables ] = calc_observables( species, expressions );
    
    % update ratelaws
    [ ratelaws ] = calc_ratelaws( species, expressions, observables );
                        
    % calculate derivatives
$calc_derivs_string
end


end
EOF

	close(Mscript);
	print "Wrote M-file script $mscript_path.\n";
	
	
###
# open species_init Mfile and begin printing...
	my $mscript_filename = "speciesInit_${mscript_filebase}.m";
	my $mscript_path     = File::Spec->catpath($vol,$path,$mscript_filename);
	open( Mscript, ">$mscript_path" ) || die "Couldn't open $mscript_path: $!\n";
    print Mscript <<"EOF";

function [species_init] = ${mscript_filename}( params )

    species_init = zeros(1,$n_species);
$mscript_species_init
end
EOF
close(Mscript);
print "Wrote M-file script $mscript_path.\n";

}



###
###
###
1;



