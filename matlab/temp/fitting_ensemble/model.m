function [err, timepoints, species_out, observables_out ] = model( timepoints, species_init, parameters, suppress_plot )
%MODEL Integrate reaction network and plot observables.
%   Integrates the reaction network corresponding to the BioNetGen model
%   'model' and then (optionally) plots the observable trajectories,
%   or species trajectories if no observables are defined. Trajectories are
%   generated using either default or user-defined parameters and initial
%   species values. Integration is performed by the MATLAB stiff solver
%   'ode15s'. MODEL returns an error value, a vector of timepoints,
%   species trajectories, and observable trajectories.
%   
%   [err, timepoints, species_out, observables_out]
%        = model( timepoints, species_init, parameters, suppress_plot )
%
%   INPUTS:
%   -------
%   species_init    : row vector of 10 initial species populations.
%   timepoints      : column vector of time points returned by integrator.
%   parameters      : row vector of 10 model parameters.
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
%   QUESTIONS about the BNG Mfile generator?  Email justinshogg@gmail.com



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
   parameters = [ 100, 100, 100, 100, 1, 1, 1, 1, 1, 1 ];
end
% check that parameters has proper dimensions
if (  size(parameters,1) ~= 1  |  size(parameters,2) ~= 10  )
    fprintf( 1, 'Error: size of parameter argument is invalid! Correct size = [1 10].\n' );
    err = 1;
    return;
end

% setup default initial values, if necessary
if ( isempty(species_init) )
   species_init = initialize_species( parameters );
end
% check that species_init has proper dimensions
if (  size(species_init,1) ~= 1  |  size(species_init,2) ~= 10  )
    fprintf( 1, 'Error: size of species_init argument is invalid! Correct size = [1 10].\n' );
    err = 1;
    return;
end

% setup default timepoints, if necessary
if ( isempty(timepoints) )
   timepoints = linspace(0,10,20+1)';
end
% check that timepoints has proper dimensions
if (  size(timepoints,1) < 2  |  size(timepoints,2) ~= 1  )
    fprintf( 1, 'Error: size of timepoints argument is invalid! Correct size = [t 1], t>1.\n' );
    err = 1;
    return;
end

% setup default suppress_plot, if necessary
if ( isempty(suppress_plot) )
   suppress_plot = 0;
end
% check that suppress_plot has proper dimensions
if ( size(suppress_plot,1) ~= 1  |  size(suppress_plot,2) ~= 1 )
    fprintf( 1, 'Error: suppress_plots argument should be a scalar!\n' );
    err = 1;
    return;
end

% define parameter labels (this is for the user's reference!)
param_labels = { 'A0', 'B0', 'C0', 'D0', 'kf1', 'kf2', 'kf3', 'kr1', 'kr2', 'kr3' };



%% Integrate Network Model
 
% calculate expressions
[expressions] = calc_expressions( parameters );

% set ODE integrator options
opts = odeset( 'RelTol',   1e-008,   ...
               'AbsTol',   0.0001,   ...
               'Stats',    'off',  ...
               'BDF',      'off',    ...
               'MaxOrder', 5   );


% define derivative function
rhs_fcn = @(t,y)( calc_species_deriv( t, y, expressions ) );

% simulate model system (stiff integrator)
try 
    [timepoints, species_out] = ode15s( rhs_fcn, timepoints, species_init', opts );
catch
    err = 1;
    fprintf( 1, 'Error: some problem encounteredwhile integrating ODE network!\n' );
    return;
end

% calculate observables
observables_out = zeros( length(timepoints), 3 );
for t = 1 : length(timepoints)
    observables_out(t,:) = calc_observables( species_out(t,:), expressions );
end


%% Plot Output, if desired

if ( ~suppress_plot )
    
    % define plot labels
    observable_labels = { 'A_B', 'A_C', 'A_D' };

    % construct figure
    plot(timepoints,observables_out);
    title('model observables','fontSize',14,'Interpreter','none');
    axis([0 timepoints(end) 0 inf]);
    legend(observable_labels,'fontSize',10,'Interpreter','none');
    xlabel('time','fontSize',12,'Interpreter','none');
    ylabel('number','fontSize',12,'Interpreter','none');

end


%~~~~~~~~~~~~~~~~~~~~~%
% END of main script! %
%~~~~~~~~~~~~~~~~~~~~~%


% initialize species function
function [species_init] = initialize_species( params )

    species_init = zeros(1,10);
    species_init(1) = params(1);
    species_init(2) = params(2);
    species_init(3) = params(3);
    species_init(4) = params(4);
    species_init(5) = 0;
    species_init(6) = 0;
    species_init(7) = 0;
    species_init(8) = 0;
    species_init(9) = 0;
    species_init(10) = 0;

end


% user-defined functions



% Calculate expressions
function [ expressions ] = calc_expressions ( parameters )

    expressions = zeros(1,10);
    expressions(1) = parameters(1);
    expressions(2) = parameters(2);
    expressions(3) = parameters(3);
    expressions(4) = parameters(4);
    expressions(5) = parameters(5);
    expressions(6) = parameters(6);
    expressions(7) = parameters(7);
    expressions(8) = parameters(8);
    expressions(9) = parameters(9);
    expressions(10) = parameters(10);
   
end



% Calculate observables
function [ observables ] = calc_observables ( species, expressions )

    observables = zeros(1,3);
    observables(1) = species(5) +species(8) +species(10);
    observables(2) = species(8) +species(10);
    observables(3) = species(10);

end


% Calculate ratelaws
function [ ratelaws ] = calc_ratelaws ( species, expressions, observables )

    ratelaws = zeros(1,3);
    ratelaws(1) = expressions(5)*species(1)*species(2);
    ratelaws(2) = expressions(6)*species(2)*species(3);
    ratelaws(3) = expressions(7)*species(3)*species(4);
    ratelaws(4) = expressions(5)*species(1)*species(6);
    ratelaws(5) = expressions(8)*species(5);
    ratelaws(6) = expressions(6)*species(2)*species(7);
    ratelaws(7) = expressions(6)*species(5)*species(3);
    ratelaws(8) = expressions(6)*species(5)*species(7);
    ratelaws(9) = expressions(9)*species(6);
    ratelaws(10) = expressions(7)*species(6)*species(4);
    ratelaws(11) = expressions(10)*species(7);
    ratelaws(12) = expressions(5)*species(1)*species(9);
    ratelaws(13) = expressions(8)*species(8);
    ratelaws(14) = expressions(8)*species(10);
    ratelaws(15) = expressions(9)*species(8);
    ratelaws(16) = expressions(9)*species(9);
    ratelaws(17) = expressions(9)*species(10);
    ratelaws(18) = expressions(7)*species(8)*species(4);
    ratelaws(19) = expressions(10)*species(9);
    ratelaws(20) = expressions(10)*species(10);

end

% Calculate species derivates
function [ Dspecies ] = calc_species_deriv ( time, species, expressions )
    
    % initialize derivative vector
    Dspecies = zeros(10,1);
    
    % update observables
    [ observables ] = calc_observables( species, expressions );
    
    % update ratelaws
    [ ratelaws ] = calc_ratelaws( species, expressions, observables );
                        
    % calculate derivatives
    Dspecies(1) = -ratelaws(4) -ratelaws(1) +ratelaws(13) -ratelaws(12) +ratelaws(14) +ratelaws(5);
    Dspecies(2) = -ratelaws(6) -ratelaws(1) +ratelaws(16) +ratelaws(9) -ratelaws(2) +ratelaws(5);
    Dspecies(3) = ratelaws(11) -ratelaws(3) -ratelaws(7) +ratelaws(9) -ratelaws(2) +ratelaws(15);
    Dspecies(4) = ratelaws(11) -ratelaws(18) -ratelaws(3) +ratelaws(19) -ratelaws(10) +ratelaws(20);
    Dspecies(5) = -ratelaws(8) +ratelaws(1) -ratelaws(7) +ratelaws(17) +ratelaws(15) -ratelaws(5);
    Dspecies(6) = -ratelaws(4) +ratelaws(19) +ratelaws(13) -ratelaws(10) -ratelaws(9) +ratelaws(2);
    Dspecies(7) = -ratelaws(8) -ratelaws(6) -ratelaws(11) +ratelaws(3) +ratelaws(16) +ratelaws(17);
    Dspecies(8) = ratelaws(4) -ratelaws(18) +ratelaws(7) -ratelaws(13) +ratelaws(20) -ratelaws(15);
    Dspecies(9) = ratelaws(6) -ratelaws(19) -ratelaws(16) +ratelaws(10) -ratelaws(12) +ratelaws(14);
    Dspecies(10) = ratelaws(8) +ratelaws(18) -ratelaws(17) +ratelaws(12) -ratelaws(20) -ratelaws(14);

end


end
