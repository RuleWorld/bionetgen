package BNGModel;
# BNGAction is part of the BNGModel package. This file contains Action commands
# including:  simulate, simulate_nf, simulate_pla, generate_hybrid_model, ...


# pragmas
use strict;
use warnings;



###
###
###



sub simulate_ode
{
    my $model = shift @_;
    my $params = (@_) ? shift @_ : {};

    $params->{method} = 'cvode';
    my $err = $model->simulate( $params );
    return $err;
}



###
###
###



sub simulate_ssa
{
    my $model = shift @_;
    my $params = (@_) ? shift @_ : {};

    $params->{method} = 'ssa';
    my $err = $model->simulate( $params );
    return $err;
}



###
###
###



sub simulate_pla
{
    my $model = shift @_;
    my $params = (@_) ? shift @_ : {};

    $params->{method} = 'pla';
    my $err = $model->simulate( $params );
    return $err;
}



###
###
###


# all purpose simulation method (does not yet support NFsim)
sub simulate
{
	use IPC::Open3;

	my $model  = shift @_;
	my $params = (@_) ? shift @_ : {};
	my $err;

    my $METHODS =
    {
        cvode => { binary=>'run_network', type=>'Network', input=>'net',
                   options=>{ atol=>1, rtol=>1, sparse=>1, steady_state=>1 }   },
        ssa   => { binary=>'run_network', type=>'Network', input=>'net', 
                   options=>{ seed=>1 }                                        },
        pla   => { binary=>'run_network', type=>'Network', input=>'net',
                   options=>{ seed=>1 }                                        },
        nf    => { binary=>'NFsim', type=>'NetworkFree', input=>'xml',
                   options=>{ seed=>1 }                                        }
    };

	return '' if $BNGModel::NO_EXEC;

    # general options
	my $prefix       = defined $params->{prefix}     ? $params->{prefix}     : $model->getOutputPrefix();
	my $netfile      = defined $params->{netfile}    ? $params->{netfile}    : undef;
	my $verbose      = defined $params->{verbose}    ? $params->{verbose}    : 0;
	my $print_end    = defined $params->{print_end}  ? $params->{print_end}  : 0;
	my $print_net    = defined $params->{print_net}  ? $params->{print_net}  : 0;
    my $continue     = defined $params->{'continue'} ? $params->{'continue'} : 0;	
    my $method       = defined $params->{method}     ? $params->{method}     : undef;
    my $print_active = defined $params->{print_n_species_active} ? $params->{print_n_species_active} : 0;
    my $print_cdat   = defined $params->{print_CDAT} ? $params->{print_CDAT} : 1; # Default is to print .cdat
    my $print_fdat   = defined $params->{print_functions} ? $params->{print_functions} : 0; # Default is to NOT print .fdat

    # continuous options
	my $atol         = defined $params->{atol}         ? $params->{atol}         : 1e-8;
	my $rtol         = defined $params->{rtol}         ? $params->{rtol}         : 1e-8;
	my $sparse       = defined $params->{sparse}       ? $params->{sparse}       : 0;
	my $steady_state = defined $params->{steady_state} ? $params->{steady_state} : 0;

    # stochastic options
	my $seed         = defined $params->{seed} ? $params->{seed} : int( rand((2**31)-1) );


    # check method
    unless ( $method )
    {  return "Simulate requires method parameter (cvode, ssa, pla, nf, etc).";  }
    unless ( exists $METHODS->{$method} )
    {  return "Simulation method $method is not a valid.";  }


    # add optional suffix to output prefix
	if ( $params->{suffix} )
    {  $prefix .= "_" . $params->{suffix};  }

	# Find or Create netfile
	my $netpre;
	if ($netfile)
    {
        # user defined netfile
		# Make sure NET file has proper suffix
		($netpre = $netfile) =~ s/[.]([^.]+)$//;
		unless ( $1 =~ /net/i )
        {   return "File $netfile does not have net suffix";   }
	}
	else
    {
        # default to [prefix].net
		$netfile = "${prefix}.net";
		$netpre  = $prefix;

		# Generatese NET file if not already created or if updateNet flag is set
		if ( !(-e $netfile) or $model->UpdateNet or (defined $params->{prefix}) or (defined $params->{suffix}) )
		{
			$err = $model->writeNET( {prefix=>"$netpre"} );
            if ($err) {  return $err;  }
		}
	}


    # Find binary
    my $binary = $METHODS->{$method}->{binary}; 
	printf "%s simulation using %s\n", $METHODS->{$method}->{type}, $method;
	my $program;
	unless ( $program = findExec($binary) )
    {   return "Could not find executable $binary";   }

	
    # Begin writing command: start with program
    my $command = "\"$program\"";

    # add output prefix
    $command .= " -o \"$prefix\"";
    
    # add method to command
    $command .= " -p $method";
    
    # pla configuration
    if ($method =~ /^pla$/){
    	if (exists $params->{pla_config}){
    		$command .= " \"$params->{pla_config}\"";
    	}
    }
    
    # add method options
    {
        my $opts = $METHODS->{$method}->{options};
        if ( exists $opts->{seed} )
        {   # random seed
	        $command .= " -h $seed";
        }
        if ( exists $opts->{atol} )
        {   # absolute tolerance
		    $command .= " -a $atol";
        }
        if ( exists $opts->{rtol} )
        {   # absolute tolerance
		    $command .= " -r $atol";
        }
        if ( exists $opts->{sparse} )
        {   # sparse methods
		    if ($sparse) { $command .= " -b"; }
        }
        if ( exists $opts->{steady_state} )
        {   # check for steady state
            if ($steady_state) { $command .= " -c"; }
        }
    }

    # define update_interval
	my $update_interval = defined $params->{update_interval} ? $params->{update_interval} : 1.0;

    # define expand
	my $expand =  defined $params->{expand} ? $params->{expand} : 'lazy';
	if ( $expand eq 'lazy' ) { }
	elsif ( $expand eq 'layered' ) { }
	else { return "Unrecognized expand method $expand";	}

	# define maximum # of sim steps
    if (defined $params->{max_sim_steps})
    {  $command .= sprintf " -M %s", $params->{max_sim_steps};  } 

    # define output step interval
    if (defined $params->{output_step_interval})
    {  $command .= sprintf " -I %f", $params->{output_step_interval};  }
    
    # output concentrations data
    $command .= sprintf " --cdat %s", $print_cdat;
    
    # output function values
    $command .= sprintf " --fdat %s", $print_fdat;

    # define print_net
	if ($print_net) { $command .= " -n"; }
    # define print_end
	if ($print_end) { $command .= " -e"; }
	# More detailed output
	if ($verbose)   { $command .= " -v"; }
	# Continuation
	if ($continue)  { $command .= " -x"; }	
	# Print number of active species
	if ($print_active) { $command .= " -j"; }


	# Set start time for trajectory
	my $t_start;
	if ( defined $params->{t_start} )
    {
		$t_start = $params->{t_start};		
		# if this is a continuation, check that model time equals t_start
		if ($continue)
        {
		    unless ( defined($model->Time)  and  ($model->Time == $t_start) )
            {  return "t_start must equal current model time for continuation.";  }
		}
	}
	else
    {   # t_start defaults to 0
	    if ( $continue   and   defined($model->Time) )
        {  $t_start = $model->Time;  }
 		else
        {  $t_start = 0.0;  }
	}

    # Set the model time to t_start
    $model->Time($t_start);

  	# To preserve backward compatibility: only output start time if != 0
	unless ( $t_start == 0.0 )
    {  $command .= " -i $t_start";  }

	# Use program to compute observables
	$command .= " -g \"$netfile\"";

	# Read network from $netfile
	$command .= " \"$netfile\"";

    # define t_end and n_steps
    my ($n_steps, $t_end);
	if ( (defined $params->{n_steps} || defined $params->{n_output_steps}) && defined $params->{sample_times}){
		# Throw warning if both n_steps and sample_times are defined
		my $x = ( defined $params->{n_steps} ) ? "n_steps" : "n_output_steps";
		printf "WARNING: $x and sample_times both defined. $x takes precedence.\n";
	}
	if ( defined $params->{n_steps} || defined $params->{n_output_steps} || !defined $params->{sample_times} )
   	{
    	if ( defined $params->{n_steps} && defined $params->{n_output_steps} ){ # Don't let them both be defined
    		return "Cannot define both n_steps and n_output_steps. Please only define one (n_output_steps is preferred).";
    	}

		if ( defined $params->{t_end} ){
            $t_end = $params->{t_end};
        }
		else{
            return "Parameter t_end must be defined.";
        }
		
        if ( ($t_end - $t_start) <= 0.0 )
        {   return "t_end must be greater than t_start.";   }

#		$n_steps = ( defined $params->{n_steps} ) ? $params->{n_steps} : 1;
		
		if (defined $params->{n_steps}){
			$n_steps = $params->{n_steps};
		}
		elsif (defined $params->{n_output_steps}){
			$n_steps = $params->{n_output_steps};
		}
		else{
			$n_steps = 1;
		}
		
		my $step_size = ($t_end - $t_start) / $n_steps;
		$command .= " ${step_size} ${n_steps}";
	}
	elsif ( defined $params->{sample_times} )
    {
    	my $sample_times = $params->{sample_times};
    	@$sample_times = sort {$a <=> $b} @$sample_times; # numeric sort
    	if ( @$sample_times > 2 ){
    		if ( defined $params->{t_end} ){ 
    			$t_end = $params->{t_end};
    			while ($sample_times->[ $#$sample_times ] >= $t_end){ # remove all sample times >= t_end
    				pop @$sample_times;
    			}
    			push @$sample_times, $t_end; # push t_end as final sample time
    		}
    		else{
    			$t_end = $sample_times->[ $#$sample_times ];
    		}
    		$command .= " " . join( " ", @$sample_times ); # add sample times to argument list
    	}
    	else{
    		return "'sample_times' array must contain 3 or more points.";
		}
#
#		my $sample_times = $params->{sample_times};
#		if ( @$sample_times > 2 )
#        {
#			$command .= " " . join( " ", @$sample_times );
#			$t_end = $sample_times->[ $#$sample_times ];
#		}
#		else
#        {
#            return "sample_times array must contain 3 or more points";
#		}
	}
	
	# Determine index of last rule iteration
    my $n_iter = 0;
	if ( $model->SpeciesList )
    {
		foreach my $spec ( @{$model->SpeciesList->Array} )
        {
			my $iter = $spec->RulesApplied;
			$n_iter = ( $iter > $n_iter ) ? $iter : $n_iter;
		}
	}



    ### RUN SIMULATION ###
	print "Running run_network on ", `hostname`;
	print "full command: $command\n";

	# start simulator as child process with communication pipes
    my $pid;
	local ( *Reader, *Writer, *Err );
	unless ( $pid = open3( \*Writer, \*Reader, \*Err, "$command" ) )
    {   return "$command failed: $?";   }

    # Wait for messages from the Simulator
	my $last_msg = '';
    my $steady_state_reached;
	my $edge_warning = 0;
    my $otf = 0;
	while ( my $message = <Reader> )
    {
		# If network generation is on-the-fly, look for signal that
		# species at the edge of the network is newly populated
        if ( $message =~ /Steady state reached/ )
        {   # steady state message
            $steady_state_reached = 1;
		}
		elsif ( $message =~ s/^edgepop:\s*// )
        {
            # remember that we've attempted On-the-fly!
            $otf = 1;

			unless ( $model->SpeciesList )
            {   # Can't generate new species if running from netfile
                # TODO: I don'think it's sufficient to check is SpeciesList is defined.
                #  It's possible that it exists but the Network generation infrastructure is missing --Justin
    			++$edge_warning;
				print Writer "continue\n";
				next;
			}

            # parse edgepop message
			my (@newspec) = split /\s+/, $message;

			my $species;
			++$n_iter;
			if ( $expand eq 'lazy' )
            {
				my (@sarray, $spec);
				foreach my $sname (@newspec)
                {
					unless ( $spec = $model->SpeciesList->lookup_bystring($sname) )
                    {  return "Couldn't find species $sname.";  }
					push @sarray, $spec;
				}
				if ($verbose)
                {  printf "Applying rules to %d species\n", scalar @sarray;  }
				$species = \@sarray;
			}
			else
            {   # Do full next iteration of rule application
				$species = $model->SpeciesList->Array;
			}

			# Apply reaction rules
			my $nspec = scalar @{$model->SpeciesList->Array};
			my $nrxn  = scalar @{$model->RxnList->Array};
			my $irule = 1;
			my ($n_new, $t_off);
			foreach my $rset ( @{$model->RxnRules} )
            {
				if ($verbose) {  $t_off = cpu_time(0);  }
				$n_new = 0;
				foreach my $rr (@$rset)
                {
					$n_new += $rr->applyRule( $model->SpeciesList, $model->RxnList, $model->ParamList, $species, $params );
				}
				if ($verbose)
                {
					printf "Rule %3d: %3d new reactions %.2e s CPU time\n",
					  $irule,
					  $n_new, cpu_time(0) - $t_off;
				}
				++$irule;
			}

			# Set RulesApplied attribute to everything in @$species
			foreach my $spec (@$species)
            {
				$spec->RulesApplied($n_iter) unless ($spec->RulesApplied);
			}
			# Update observables
	        foreach my $obs (@{$model->Observables})
            {
		        $obs->update( $model->SpeciesList->Array, $nspec );
			}
            # Set ObservablesApplied attribute to everything in SpeciesList
            my $new_species = [];
            foreach my $spec ( @{$model->SpeciesList->Array} )
            {
                unless ( $spec->ObservablesApplied )
                {
                    push @$new_species, $spec  unless ( $spec->RulesApplied );
                    $spec->ObservablesApplied(1);
                }
            }

			# Print new species, reactions, and observable entries
			if ( scalar @{$model->RxnList->Array} > $nrxn )
            {
				print Writer "read\n";
				$model->SpeciesList->print( *Writer, $nspec );
				$model->RxnList->print( *Writer, $nrxn );
				print Writer "begin groups\n";
				my $i = 1;
				foreach my $obs ( @{$model->Observables} )
                {
					print Writer "$i ";
					$obs->printGroup( *Writer, $model->SpeciesList->Array, $nspec );
					++$i;
				}
				print Writer "end groups\n";
			}
			else
            {
				print Writer "continue\n";
			}
		}
		else
        {
			print $message;
			$last_msg = $message;
		}
	}

    # Simulator is finished
	my @err = <Err>;
	close Writer;
	close Reader;
	close Err;
	waitpid( $pid, 0 );


	# Report number of times edge species became populated without network expansion
	if ($edge_warning)
    {   send_warning("Edge species of truncated network became populated $edge_warning times");   }

	if (@err)
    {   # print any errors received from 
		print @err;
		return "$command\n  did not run successfully.";
	}

	unless ( $last_msg =~ /^Program times:/ )
    {   return "$command\n  did not run successfully.";  }

    
    # At this point, the simulation seems to be ok.
    #  Go ahead and print out final netfile (if there are new reactions or species)
    if ( $otf  and  $model->SpeciesList )
    {   # TODO: I don'think it's sufficient to check if SpeciesList is defined.
        #  It's possible that it exists but the Network generation infrastructure is missing --Justin
	    $err = $model->writeNET( {prefix => "$netpre"} );
        if ($err) { return $err; }
    }


    # Report steady state
	if ($steady_state)
    {
		send_warning("Steady_state status = $steady_state_reached");
		unless ( $steady_state_reached )
        {   return "Simulation did not reach steady state by t_end=$t_end";   }
	}

	# If there are no errors or flags so far, let's load output concentrations
	if ( !($model->RxnList) )
    {   # TODO: what does this accomplish? --Justin
		send_warning("Not updating species concentrations because no model has been read");
	}
	elsif ( -e "$prefix.cdat" )
    {
		print "Updating species concentrations from $prefix.cdat\n";
		open CDAT, '<', "$prefix.cdat" ;
		my $last_line = '';
		while (my $line = <CDAT>) {  $last_line = $line;  }
		close CDAT;

		# Update Concentrations with concentrations from last line of CDAT file
        my ($time, $conc);
		($time, @$conc) = split ' ', $last_line;
		my $species = $model->SpeciesList->Array;
		unless ( $#$conc == $#$species )
        {
			$err = sprintf "Number of species in model (%d) and CDAT file (%d) differ", scalar @$species, scalar @$conc;
			return $err;
		}
		$model->Concentrations( $conc );
		$model->UpdateNet(1);
	}
	else
    {	return "CDAT file is missing";   }

    # Set model time to t_end
	$model->Time($t_end);

    # all finished!
	return '';
}



###
###
###



#sub simulate_pla
#{
#	use IPC::Open3;
#
#	my $model  = shift;
#	my $params = shift;
#	my $err;
#
#	my $prefix = ( defined $params->{prefix} ) ? $params->{prefix} : $model->getOutputPrefix();
#	my $verbose = ( defined $params->{verbose} ) ? $params->{verbose} : 0;
#	my $print_end = ( defined $params->{print_end} ) ? $params->{print_end} : 0;
#	my $print_net = ( defined $params->{print_net} ) ? $params->{print_net} : 0;
#	my $seed = ( defined( $params->{seed} ) ) ? $params->{seed} : int( rand( (2**31)-1 ) );
#    # Added explicit argument for simulation continuation.  --Justin
#    my $continue = (defined $params->{continue} ) ? $params->{continue} : 0;
#    my $print_cdat   = defined $params->{print_CDAT} ? $params->{print_CDAT} : 1; # Default is to print .cdat
#    my $print_fdat   = defined $params->{print_functions} ? $params->{print_functions} : 0; # Default is to NOT print .fdat
#
#	return '' if $BNGModel::NO_EXEC;
#
#	if (exists $params->{suffix}){
#		$prefix .= "_$params->{suffix}";
#	}
#
#	print "Network simulation using PLA\n";
#	
#	my $program;
#	if ( !( $program = findExec("run_network") ) ) {
#		return ("Could not find executable run_network");
#	}
#	my $command = "\"" . $program . "\"";
#	if (exists $params->{prefix} || exists $params->{suffix}){
#		$command .= " -o \"$prefix\"";
#	}
#
#	# Default netfile based on prefix
#	my $netfile;
#	my $netpre;
#	if (!(exists $params->{netfile})){
#		$netfile = $prefix . ".net";
#		$netpre  = $prefix;
#		# Generate net file if not already created or if prefix is set in params
#		if ( ( !-e $netfile ) || $model->UpdateNet || ( defined( $params->{prefix} ) ) || ( defined( $params->{suffix} ) ) ){
#			if ( $err = $model->writeNET( { prefix => "$netpre" } ) ) {
#				return ($err);
#			}		
#		}
#	}
#	else {
#		$netfile = $params->{netfile};
#		# Make sure NET file has proper suffix
#		$netpre = $netfile;
#		$netpre =~ s/[.]([^.]+)$//;
#		if ( !( $1 =~ /net/i ) ) {
#			return ("File $netfile does not have .net suffix");
#		}
#	}
#
#	my $expand = defined $params->{expand} ? $params->{expand} : "lazy";
#	if ($expand ne "lazy" && $expand ne "layered") 
#    {
#		return "Unrecognized expand method $expand";
#	}
#
#	$command .= " -p pla";
#    if (exists $params->{pla_config}){ # PLA configuration (string)
#    	$command .= " \"$params->{pla_config}\"";
#    }
#
#	$command .= " -h $seed";
#
#	if ($print_net) { $command .= " -n"; }
#	if ($print_end) { $command .= " -e"; }
#	
#	# Added arguments for maximum # of sim steps and output step interval --Leonard
#    if (exists $params->{max_sim_steps}){
#    	$command .= " -M $params->{max_sim_steps}";
#    } 
#    if (exists $params->{output_step_interval}){
#    	$command .= " -I $params->{output_step_interval}";
#    }
#    
#    # output concentrations data
#    $command .= sprintf " --cdat %s", $print_cdat;
#    
#    # output function values
#    $command .= sprintf " --fdat %s", $print_fdat;
#
#	# More detailed output
#	if ($verbose) { $command .= " -v"; }
#	
#	# Continuation
#	# NOTE: continuation must now be specified explicitly!
#	if ($continue) { $command .= " -x"; }	
#
#	# Print number of active species
#    #if ($print_n_species_active) {
#    #    $command .= " -j";
#    #}
#
#	# Set start time for trajectory
#	my $t_start;
#	# t_start argument is defined
#	if ( defined( $params->{t_start} ) ) {
#		$t_start = $params->{t_start};		
#		# if this is a continuation, check that model time equals t_start
#		if ( $continue ) {
#		    unless ( defined($model->Time)  and  ($model->Time == $t_start) ) {
#		        return ("t_start must equal current model time for continuation.");
#		    }
#		}
#	}
#	# t_start argument not defined
#	else {
#	    if ( $continue   and   defined($model->Time) ) {   
#	    	$t_start = $model->Time;   
#	    }
# 		else {   
# 			$t_start = 0.0;   
# 		}
#	}
#
#    # set the model time to t_start
#    $model->Time($t_start);
#
#  	# To preserve backward compatibility: only output start time if != 0
#	unless ( $t_start == 0.0 ) {   
#		$command .= " -i $t_start"; 	
#	}
#
#	# Use program to compute observables
#	$command .= " -g \"$netfile\"";
#
#	# Read network from $netfile
#	$command .= " \"$netfile\"";
#
#    my ($n_steps, $t_end);
#	if ( defined $params->{n_steps} )
#    {
#		
#		if ( defined $params->{t_end} ) {
#			$t_end = $params->{t_end};
#		}
#		else {
#			return "Parameter t_end must be defined";
#		}
#		# Extend interval for backward compatibility. Previous versions default assumption was $t_start=0.
#		if ( ($t_end-$t_start) <= 0.0 )
#        {
#		    return "t_end must be greater than t_start.";
#        }
#		$n_steps = ( defined( $params->{n_steps} ) ) ? $params->{n_steps} : 1;
#		my $step_size = ( $t_end - $t_start ) / $n_steps;
#		$command .= " ${step_size} ${n_steps}";
#	}
#	elsif ( defined $params->{sample_times} )
#    {   # Not implemented yet, but will be caught by Network3!
#		# Two sample points are given.
#		my $sample_times = $params->{sample_times};
#		if ( @$sample_times > 2 )
#        {
#			$command .= " " . join( " ", @$sample_times );
#			$t_end = $sample_times->[ $#$sample_times ];
#		}
#		else
#        {
#			return "sample_times array must contain 3 or more points";
#		}
#	}
#
#	# Determine index of last rule iteration
#	if ( $model->SpeciesList )
#    {
#		my $n_iter = 0;
#		foreach my $spec ( @{ $model->SpeciesList->Array } )
#        {
#			my $iter = $spec->RulesApplied;
#			$n_iter = ( $iter > $n_iter ) ? $iter : $n_iter;
#		}
#	}
#	print "Running run_network on ", `hostname`;
#	print "full command: $command\n";
#
#	# Compute timecourses using run_network
#    my $pid;
#	local ( *Reader, *Writer, *Err );
#	unless ( $pid = open3(\*Writer, \*Reader, \*Err, "$command") )
#    {   return "$command failed: $?";   }
#
#	my $last = '';
#	my $edgepop = 0;
#	while ( my $message = <Reader>)
#    {
#        # If network generation is on-the-fly, look for signal that
#        # species at the edge of the network is newly populated
#		if ( $message =~ s/^edgepop:\s*// )
#        {
#            # NOT IMPLEMENTED YET
#		}
#		else
#        {
#			print $message;
#			$last = $message;
#		}
#	}
#	my @err = <Err>;
#	close Writer;
#	close Reader;
#	close Err;
#	waitpid( $pid, 0 );
#
#	# Report number of times edge species became populated without network expansion
#    #if ($edgepop){
#    #    printf "Edge species became populated %d times.\n", $edgepop;
#    #}
#
#	# Check for errors in running the simulation command
#	if (@err)
#    {
#		print @err;
#		return "$command\n  did not run successfully.";
#	}
#	unless ( $last =~ /^Program times:/ )
#    {
#		return "$command\n  did not run successfully.";
#	}
#
#	# Process output concentrations
#	unless ( $model->RxnList )
#    {
#		send_warning("Not updating species concnetrations because no model has been read");
#	}
#	elsif ( -e "$prefix.cdat" )
#    {
#		print "Updating species concentrations from $prefix.cdat\n";
#		open( CDAT, "$prefix.cdat" );
#		my $last = "";
#		while (<CDAT>) { $last = $_; }
#		close CDAT;
#
#		# Update Concentrations with concentrations from last line of CDAT file
#        my ($time, $conc);
#		($time, @$conc) = split ' ', $last;
#		my $species = $model->SpeciesList->Array;
#		if ( $#$conc != $#$species ) {
#			$err = sprintf "Number of species in model (%d) and CDAT file (%d) differ", scalar(@$species), scalar(@$conc);
#			return ($err);
#		}
#		$model->Concentrations($conc);
#		$model->UpdateNet(1);
#	}
#	else
#    {
#		return "CDAT file is missing";
#	}
#	$model->Time($t_end);
#
#	return '';
#}



###
###
###



sub simulate_nf
{
	use IPC::Open3;

	my $model  = shift;
	my $params = shift;
	my $err;

	my $prefix  = ( defined $params->{prefix} )  ? $params->{prefix}  : $model->getOutputPrefix();
	my $suffix  = ( defined $params->{suffix} )  ? $params->{suffix}  : '';
	my $verbose = ( defined $params->{verbose} ) ? $params->{verbose} : 0;
	my $complex = ( defined $params->{complex} ) ? $params->{complex} : 0;
    my $get_final_state = ( defined $params->{get_final_state} ) ? $params->{get_final_state} : 0;
	# Handle other command line args.
	my $otherCommandLineParameters = ( defined $params->{param} ) ? $params->{param} : '';

	return '' if $BNGModel::NO_EXEC;

    if ( $model->Params->{no_nfsim} )
    {   # don't execute NFsim if nf_nfsim parameter is true
        send_warning( "simulate_nf(): skipping simulation, 'no-nfsim' flag is true.");
        return '';
    }

	if ( $suffix ne '' ) {
		$prefix .= "_${suffix}";
	}

	print "Netfree simulation using NFsim\n";
	my $program;
	unless ( $program = findExec("NFsim") ) {
		return "Could not find executable NFsim";
	}
	my $command = "\"" . $program . "\"";

	# Write BNG xml file
	$model->writeXML( { prefix => $prefix } );

	# Defined command line
	$command .= " -xml \"${prefix}.xml\" -o \"${prefix}.gdat\"";
    if ( $get_final_state ){
        $command .= " -ss \"${prefix}.species\"";
    }

	# Append the run time and output intervals
	my $t_start;
	if ( defined( $params->{t_start} ) ) {
		$t_start = $params->{t_start};
		$model->Time($t_start);
	}
	else {
		$t_start = ( defined( $model->Time ) ) ? $model->Time : 0;
	}

    my $t_end;
	if ( defined $params->{n_steps} )
    {
		my ($n_steps, $t_end);
		$n_steps = $params->{n_steps};
		if ( $n_steps < 1 )
        {   return "No simulation output requested: set n_steps>0";   }

		if ( defined $params->{t_end} )
        {   $t_end = $params->{t_end};   }
		else
        {   return "Parameter t_end must be defined";   }

		$command .= " -sim ${t_end} -oSteps ${n_steps}";
	}
	elsif ( defined $params->{sample_times} )
    {
		return "sample_times not supported in this version of NFsim";
	}
	else
    {
		return "No simulation output requested: set n_steps>0";
	}

	# Append the other command line arguments
	$command .= " " . $otherCommandLineParameters;

	# Turn on complex bookkeeping if requested
	# TODO: Automatic check for turning this on
	if ($complex) { $command .= " -cb"; }
	if ($verbose) { $command .= " -v"; }

	print "Running NFsim on ", `hostname`;
	print "full command: $command\n";

	# Compute timecourses using nfsim
    my $pid;
	local ( *Reader, *Writer, *Err );
	unless ( $pid = open3(\*Writer, \*Reader, \*Err, "$command") )
    {   return "$command failed: $?";   }

	my $last = '';
	while ( <Reader> )
    {
		print;
		$last = $_;
	}
	( my @err = <Err> );

	close Writer;
	close Reader;
	close Err;
	waitpid( $pid, 0 );

	if (@err) {
		print "Error log:\n", @err;
		return ("$command\n  did not run successfully.");
	}

    if ( $get_final_state ){
        # Update final species concentrations to allow trajectory continuation
        if (my $err= $model->readNFspecies("${prefix}.species")){ 
            return($err);
        }
        if ($verbose){
            print $model->SpeciesList->writeBNGL( $model->Concentrations, $model->ParamList );
        }
    }
    else{
        print  "Warning: final system state was not retrieved following simulate_nf.\n"
              ."  To retreive final state, call simulate_nf with option: get_final_state=>1.\n";
    }

    $model->Time($t_end);
	return '';
}



###
###
###



sub readNFspecies 
{
    # This function reads a list of species strings from NFsim output to form a 
    # canonical species list with correct concentrations. Note that it overwritees
    # any existing species.
    my $model = shift @_;
    my $fname = shift @_;

	my $conc;
	if ($model->SpeciesList)
    {
	    $conc = [ (0) x scalar @{$model->SpeciesList->Array} ];
	}
    else
    {        
        $conc = [];
        # create species list
	    $model->SpeciesList( SpeciesList->new );
	}
    my $slist = $model->SpeciesList;

	# Read NFsim species file
	print "readNFspecies::Reading from file $fname\n";
    my $FH;
	unless ( open my $FH, '<', $fname )
    {   return "Couldn't read from file $fname: $!";   }

    my $n_spec_read = 0;
    my $n_spec_new = 0;
    my $line_num = 0;
	while ( my $string = <$FH> )
	{
        ++$line_num;
		chomp $string;                 # remove line return
        $string =~ s/^\s+//;           # remove leading whitespace
		$string =~ s/\#.*$//;          # remove comments
		next unless $string =~ /\S+/;  # skip blank lines

        # Read species string
        my $sg = SpeciesGraph->new();
        my $err = $sg->readString( \$string, $model->CompartmentList, 1, '^\s+', 
                                    $model->MoleculeTypesList );
        if ($err) {  return $err." at line $line_num of file $fname";  }

        # Read species concentration - may only be integer value
        my $conc;
        if ( $string=~ /^\s*(\d+)\s*$/ )
        {
            $conc = $1;
        }
        else
        {
            return "species concentration must be single integer at line $line_num of file $fname";
        }

        # Check if isomorphic to existing species 
        my $existing_sg = $model->SpeciesList->lookup( $sg );
        if ($existing_sg)
        { 
            # Add concentration to concentration of existing species
            $conc->[ $existing_sg->Index - 1 ] += $conc;
        }
        else
        {
            # Create new Species entry in SpeciesList with zero default concentration
            my $newspec = $model->SpeciesList->add( $sg, 0 );
            $conc->[ $newspec->Index - 1 ] = $conc;
            ++$n_spec_new;
        }
        ++$n_spec_read;
    }
    
    close $FH;

	$model->Concentrations( $conc );
    printf "Read %d unique species of %d total.\n", $n_spec_new, $n_spec_read;
	return '';
}



###
###
###



# construct a hybrid particle population model
#  --Justin, 21mar2001
sub generate_hybrid_model
{
	my $model        = shift;
	my $user_options = shift;


    my $indent = '    ';
    my $step_index = 0;
    printf "generate_hybrid_model( %s )\n", $model->Name;


    # default options
	my $options =
    {   'prefix'     => '',
        'suffix'     => 'hybrid',
		'overwrite'  => 0,
		'verbose'    => 0,
		'actions'    => ['writeXML()'],
		'execute'    => 0
	};
    # get user options
    while ( my ($opt,$val) = each %$user_options )
    {
        unless ( exists $options->{$opt} )
        {   return "Unrecognized option $opt in call to generate_hybrid_model";   }
        
        # overwrite default option
        $options->{$opt} = $val;
    }

    # do nothing if $NO_EXEC is true
	return '' if $BNGModel::NO_EXEC;


    # determine prefix, modelname and filename
    my $prefix = ( $options->{prefix} ) ? $options->{prefix} : $model->getOutputPrefix() .'_'. $options->{suffix};
    my $modelname = $model->Name .'_'. $options->{suffix};
	my $modelfile = $prefix . '.bngl';

	if ( -e $modelfile )
	{
		if ($options->{overwrite})
		{
			send_warning( "Overwriting older model file: $modelfile" );
			unlink $modelfile;
		}
		else
		{
			return "Model file $modelfile already exists. Set overwrite=>1 option to force overwrite.";
		}
	}


    # check if a ParamList exists
	unless ( defined $model->ParamList )
	{   return sprintf "Cannot continue! Model %s does not have a parameter list.", $model->Name;   }

    # Check for MoleculeTypes
	unless ( defined $model->MoleculeTypesList  and  %{$model->MoleculeTypesList->MolTypes} )
	{   return sprintf "Nothing to do! Model %s has zero molecule type definitions.", $model->Name;   } 	

    # check if a SpeciesList exists
	unless ( defined $model->SpeciesList  and  @{$model->SpeciesList->Array} )
	{   return sprintf "Nothing to do! Model %s has zero seed species definitions.", $model->Name;   }

    # Check for RxnRules
    unless ( defined $model->RxnRules  and  @{$model->RxnRules} )
	{   return sprintf "Nothing to do! Model %s has zero reaction rule definitions.", $model->Name;   } 

    # check if PopulationTypesList exists
	unless ( defined $model->PopulationTypesList  and  %{$model->PopulationTypesList->MolTypes} )
	{   return sprintf "Nothing to do! Model %s has zero population type definitions.", $model->Name;   }

    # check if PopulationList exists
	unless ( defined $model->PopulationList  and  @{$model->PopulationList->List} )
	{   return sprintf "Nothing to do! Model %s has zero population map definitions.", $model->Name;   }
   
    
    # create new model!
    my $hybrid_model = BNGModel::new();
	
	$hybrid_model->Name( $modelname );
	$hybrid_model->Version( $model->Version );
	$hybrid_model->SubstanceUnits( $model->SubstanceUnits );

    # copy options	
    %{$hybrid_model->Options} = %{$model->Options};
    # set prefix
    $hybrid_model->Options->{prefix} = $prefix;

    # copy the constants in the parameter list
    #  NOTE: we'll add observable and functions later
    print $indent . "$step_index:Fetching model parameters.. ";  ++$step_index;
    my $plist_new = $model->ParamList->copyConstant();
    $hybrid_model->ParamList( $plist_new );
    print sprintf "found %d constants and expressions.\n", scalar @{$plist_new->Array};
    
    
    # Copy compartments
    my $clist_new = undef;
    if ( defined $model->CompartmentList )
    {
        print $indent . "$step_index:Fetching compartments.. "; ++$step_index;
        $clist_new = $model->CompartmentList->copy( $plist_new );
        $hybrid_model->CompartmentList( $clist_new );
        print $indent . sprintf "found %d compartments.\n", scalar @{$clist_new->Array};
        send_warning( "generate_hybrid_model() does not support compartments at this time" ) if (@{$clist_new->Array});  
    }
    
    
    
    # Copying the moleculeTypesList and add population types
    print $indent . "$step_index:Fetching molecule types..   "; ++$step_index;
    my $mtlist_new =  $model->MoleculeTypesList->copy();
    $hybrid_model->MoleculeTypesList( $mtlist_new );
    print sprintf "found %d molecule types.\n", scalar keys %{$mtlist_new->MolTypes};
    {
        # Add population types
        print $indent . "$step_index:Adding population types..   "; ++$step_index;
        while ( my ($name,$mt) = each %{$model->PopulationTypesList->MolTypes} )
        {
            my $mt_copy = $mt->copy();
            $mt_copy->PopulationType(1);
            unless ( $mtlist_new->add($mt_copy) )
            {   return "PopulationType $name clashes with MoleculeType of the same name";  }
        }
        print sprintf "found %d population types.\n", scalar keys %{$model->PopulationTypesList->MolTypes};
    }


    # Copy seed species, replacing with populations if possible, and add empty populations
    my $slist_new = SpeciesList::new();
    $hybrid_model->SpeciesList( $slist_new );   
    {    
        print $indent . "$step_index:Fetching seed species..\n"; ++$step_index; 
 
        # loop over species in species list
        foreach my $species ( @{$model->SpeciesList->Array} )
        {
            my $sg   = $species->SpeciesGraph;
            my $conc = $species->Concentration;
        
            # check if this is isomorphic to any of our populations
            my $is_pop = 0;
	        foreach my $pop ( @{$model->PopulationList->List} )
	        {
                if ( SpeciesGraph::isomorphicTo($species->SpeciesGraph, $pop->Species) )
                {   # add the population instead of the speciesGraph
                    my $sg_copy = $pop->Population->copy();
                    $sg_copy->relinkCompartments( $hybrid_model->CompartmentList );
                    $slist_new->add( $sg_copy, $species->Concentration );
                    $is_pop = 1;
                    if ( $options->{verbose} )
                    {
                        print $indent.$indent
                            . sprintf "replaced species %s with population %s.\n", $sg->toString(), $sg_copy->toString();
                    }
                    last;
                }
            }
            unless ($is_pop)
            {   # this isn't a population, so add SpeciesGraph directly.
                my $sg_copy = $species->SpeciesGraph->copy();
                $sg_copy->relinkCompartments( $hybrid_model->CompartmentList );
                $slist_new->add( $sg_copy, $species->Concentration );
            }
        }
        print $indent . sprintf "  ..found %d seed species.\n", scalar @{$slist_new->Array};    
    }

    
    # Add population species to seed species
    {
        print $indent . "$step_index:Adding populations with zero counts to seed species..\n"; ++$step_index;     
        my $zero_pops = 0;
    	foreach my $pop ( @{$model->PopulationList->List} )
    	{
            my ($sp) = $slist_new->lookup( $pop->Population );
            unless ( $sp )
            {
                my $sg_copy = $pop->Population->copy();
                $sg_copy->relinkCompartments( $hybrid_model->CompartmentList );  
                $slist_new->add( $sg_copy, 0 );
                ++$zero_pops;
            }
        }
        print $indent . sprintf "  ..added %d populations to seed species list.\n", $zero_pops;      
    }
            

    # Copy the observables and add matches to populations (also register observable names in parameter list)
    my $obslist_new = [];
    $hybrid_model->Observables( $obslist_new );
    {
        print $indent . "$step_index:Fetching observables and adding population matches..\n"; ++$step_index;         
        # loop over observables
        foreach my $obs ( @{$model->Observables} )
        {
            my $obs_copy = $obs->copy();
            $obs_copy->relinkCompartments( $hybrid_model->CompartmentList );
            push @{$obslist_new}, $obs_copy;
        
            # get a parameter that points to this observable
            if ( $plist_new->set( $obs_copy->Name, '0', 1, "Observable", $obs_copy) )
            {
      	        my $name = $obs_copy->Name;
                return "Observable name $name clashes with previously defined Observable or Parameter";
            }
      
            # find populations to add to observable
            my @add_patterns = ();
            foreach my $pop ( @{$model->PopulationList->List} )
	        {
                my $matches = $obs_copy->match( $pop->Species );
            
                if ($matches)
                {   
                    my $ii = 0;
                    while ( $ii < $matches )
                    {
                        push @add_patterns, $pop->Population->copy()->relinkCompartments( $hybrid_model->CompartmentList );
                        ++$ii
                    }
                    if ( $options->{verbose} )
                    {
                        print $indent.$indent . sprintf "observable '%s':  +%d match%s to %s.\n",
                                                        $obs_copy->Name, $matches, ($matches>1 ? 'es' : ''), $pop->Population->toString();
                    }
                }
            }      
            push @{$obs_copy->Patterns}, @add_patterns;      
        }
        print $indent . sprintf "  ..found %d observables.\n", scalar @{$obslist_new};
    }
    
    
    # Copy functions
    {
        print $indent . "$step_index:Fetching functions.. "; ++$step_index;     
        my $fcn_copies = $model->ParamList->copyFunctions();
        foreach my $fcn ( @$fcn_copies )
        {
            $hybrid_model->ParamList->set( $fcn->Name, $fcn->Expr, 0, 'Function', $fcn );
        }
        print sprintf "found %d functions.\n", scalar @{$fcn_copies};
    }
   

    # Refine rules
    my $rxnrules_new = [];
    $hybrid_model->RxnRules( $rxnrules_new );
    {
        print $indent . "$step_index:Refining rules with respect to population objects..\n"; ++$step_index;   
	
        # get the species graphs corresponding to each population
        my $pop_species = [];
        foreach my $pop ( @{$model->PopulationList->List} )
        {   push @$pop_species, $pop->Species;   }
        my $n_popspec = scalar @$pop_species;
	
	    # loop over rules
	    my $rule_count = 0;
	    foreach my $rset ( @{$model->RxnRules} )
	    {
	        # NOTE: each element of @RxnRules is an array of reactions.
	        #  If a rule is unidirectional, then the array has a single element.
	        #  If a rule is bidirectional, then the array has two elements (forward and reverse)	
		    foreach my $rr (@$rset)
		    {    
		        # first copy the rule so we don't mess with the orginal model
		        my $rr_copy = $rr->copy();
		        $rr_copy->resetLabels();
		    
                # apply rule to species
			    my $refinements = $rr_copy->refineRule(  $pop_species, $model, $hybrid_model, {verbose => $options->{verbose}} );
			    foreach my $refinement ( @$refinements )
			    {
                    push @$rxnrules_new, [$refinement];
                }
                if ( $options->{verbose} )
                {
                    print $indent.$indent . sprintf "Rule %s: created %d refinement%s.\n",
                                                    $rr_copy->Name, scalar @$refinements, ((scalar @$refinements > 1)?'s':'');
                }
			    ++$rule_count;
	        }
	    }
        print $indent . sprintf "  ..finished processing %d reaction rules.\n", $rule_count;
    }


    # Add population maps to the list of rules
    {
        print $indent . "$step_index:Fetching population maps.. "; ++$step_index;
        foreach my $pop ( @{$model->PopulationList->List} )
	    {
	        # write rule as string
            my $rr_string = $pop->MappingRule->toString();
            # remove the linebreak
            $rr_string =~ s/\\\s//;
            # parse string to create "copy" of rule
            my ($rrs, $err) = RxnRule::newRxnRule( $rr_string, $hybrid_model );
            push @$rxnrules_new, $rrs;
        }
        print sprintf "found %d maps.\n", scalar @{$model->PopulationList->List};
    }


    # create empty RxnList
    print $indent . "$step_index:Creating empty reaction list.\n"; ++$step_index;        
    my $rxnlist_new = RxnList::new();
    $hybrid_model->RxnList( $rxnlist_new );
    

	# Print hybrid model to file
    my $FH;
    print $indent . "$step_index:Attempting to write hybrid BNGL.. "; ++$step_index;    	
	unless ( open $FH, '>', $modelfile ) {  return "Couldn't write to $modelfile: $!\n";  }
    print $FH $hybrid_model->writeBNGL( {'format'=>'bngl', 'include_model'=>1,'include_network'=>0,
                                         'pretty_formatting'=>1,'evaluate_expressions'=>0} );
    # writing actions!
    if ( @{$options->{actions}} )
    {
        my $action_string = "\n\n###  model actions  ###\n\n";
        foreach my $action ( @{$options->{actions}} )
        {
            $action_string .= "$action;\n";
        }
        $action_string .= "\n";
        print $FH $action_string;
    }
	close $FH;
	
	
	print "done.\n";
	print "Wrote hybrid model to file $modelfile.\n";
	
	if ( $options->{execute} )
	{   # execute actions
	    my $errors = [];
        foreach my $action ( @{$options->{actions}} )
        {
            my $action_string = "\$hybrid_model->$action";
            my $err = eval "$action_string";
            if ($@)   {  warn $@;  }
            if ($err) {  push @$errors, $err;  }
        }	
        if (@$errors) {  return join "\n", $errors;  }
	}
	
	return '';
}



###
###
###



sub parameter_scan
{
    my $model = shift @_;
    my $params = @_ ? shift @_ : {};

    # define default params
    my $default_params = {  'prefix'   => $model->getOutputPrefix(),
                            'log'      => 0,
                            't_start'  => 0,
                            't_end'    => 20,
                            'n_steps'  => 20,
                            'steady_state' => 0
                         };

    # copy default values for undefined keys
    while ( my ($key, $val) = each %$default_params )
    {
        unless ( defined $params->{$key} )
        {   $params->{$key} = $val;   }
    }

    # check for required parameters
    unless ( defined $params->{var} )
    {   return "parameter_scan error: 'var' is not defined.";   }

    unless ( defined $params->{var_min} )
    {   return "parameter_scan error: 'var_min' is not defined.";   }

    unless ( defined $params->{var_max} )
    {   return "parameter_scan error: 'var_max' is not defined.";   }
    
    unless ( defined $params->{n_pts} )
    {   return "parameter_scan error: 'n_pts' is not defined.";   }

    unless ( defined $params->{method} )
    {   return "parameter_scan error: 'method' is not defined.";   }


    # substitute cvode for ode
    if ( $params->{method} eq 'ode' )
    {   $params->{method} = 'cvode';   }


    # update user
    printf "ACTION: parameter_scan (var: %s, min: %.3e, max: %.3e, n_pts: %d, log: %d)\n",
           $params->{var}, $params->{var_min}, $params->{var_max}, $params->{'log'};


    # define basename for scan results
    my $basename = $params->{prefix};
    if ( $params->{suffix} )
    {   $basename .= "_" . $params->{suffix};   }
    else
    {   $basename .= "_" . $params->{var};      }

    # define working directory for simulation data
    my $workdir = $basename;
    # define output file for parameter scan results
    my $outfile = $basename . ".scan";


    # create working directory
    if (-d $workdir)
    {   # delete working directory
        my $all_files = File::Spec->catfile( ($workdir), '*' );
        unlink <$all_files>;
        #system "rm -r $workdir";
    }
    else
    {   mkdir $workdir;   }


    # define parameter scan range
    my $var_min = $params->{'log'} ? log $params->{var_min} : $params->{var_min};
    my $var_max = $params->{'log'} ? log $params->{var_max} : $params->{var_max};
    my $delta   =($var_max - $var_min) / ($params->{n_pts} - 1);




    # remember concentrations!
    $model->saveConcentrations();

    # loop over timepoints
    for ( my $k = 0;  $k < $params->{n_pts};  ++$k )
    {
        # define parameter value
        my $par_value = $var_min + $k*$delta;
        if ( $params->{'log'} )
        {   $par_value = exp $par_value;   }

        # set parameter
        $model->setParameter( $params->{var}, $par_value );

        # define prefix
        my $local_prefix = File::Spec->catfile( ($workdir), sprintf("par_%s_%05d", $params->{var}, $k) );

        # reset concentrations
        $model->resetConcentrations();

        # define local params
        my $local_params;
        %$local_params = %$params;
        $local_params->{prefix} = $local_prefix;
        delete $local_params->{suffix};

        # run simulation
        my $err = $model->simulate( $local_params );
        if ( $err )
        {   # return error message
            $err = "parameter_scan error (step $k): $err";
            return $err;
        }   
    }

    # recover concentrations
    $model->resetConcentrations();


    # Extract last timepoint from each simulation and write to outfile
    my $ofh;
    unless ( open $ofh, '>', $outfile )
    {   return "parameter_scan error: problem opening parameter scan output file $outfile";   }

    for ( my $k = 0;  $k < $params->{n_pts};  ++$k )
    {
        my $par_value = $var_min + $k*$delta;
        if ( $params->{'log'} )
        {   $par_value = exp $par_value;   }

        # Get data from gdat file
        my $data_file = File::Spec->catfile( ($workdir), sprintf("par_%s_%05d.gdat", $params->{var}, $k) );
        print "Extracting observable trajectory from $data_file\n";
        my $ifh;
        unless ( open $ifh,'<', $data_file )
        {   return "parameter_scan error: problem opening observable file $data_file";   }

        # write header
        if ( $k == 0 )
        {
            my $headline = <$ifh>;
            $headline =~ s/^\s*\#//;
            my @headers = split ' ', $headline;
            shift @headers;
            printf $ofh "# %+14s", $params->{var};
            foreach my $header (@headers)
            {
                printf $ofh "%+16s", $header;
            }
            print $ofh "\n";
        }

        # get last line
        my $lastline;
        while ( my $linein = <$ifh> )
        {   $lastline = $linein;   }
        
        # close input file
        close $ifh;

        # extract data and write to output file
        my @data = split ' ', $lastline;
        my $time = shift @data;
        printf $ofh "%16.8e", $par_value;
        foreach my $data ( @data )
        {
            printf $ofh "%16.8e", $data;
        }
        print $ofh "\n";
    }  
    close $ofh;

    # return without error
    return undef;
}



###
###
###



sub LinearParameterSensitivity
{
    #This function will perform a brute force linear sensitivity analysis
    #bumping one parameter at a time according to a user specified bump
    #For each parameter, simulations are saved as:
    #'netfile_paramname_suffix.(c)(g)dat', where netfile is the .net model file
    #and paramname is the bumped parameter name, and c/gdat files have meaning as normal

	######################
	# TODO: NOT IMPLEMENTED YET!!
	#Additional files are written containing the raw sensitivity coefficients
	#for each parameter bump
	#format: 'netfile_paramname_suffix.(c)(g)sc'
	#going across rows is increasing time
	#going down columns is increasing species/observable index
	#first row is time
	#first column is species/observable index
	######################

	#Starting time assumed to be 0

    #Input Hash Elements:
    #REQUIRED PARAMETERS
    #net_file:  the base .net model to work with; string;
    #t_end:  the end simulation time; real;
    #OPTIONAL PARAMETERS
    #bump:  the percentage parameter bump; real; default 5%
    #inp_ppert:  model input parameter perturbations; hash{pnames=>array,pvalues=>array};
    #default empty
    #inp_cpert:  model input concentration perturbations; hash{cnames=>array,cvalues=>array};
    #default empty
    #stochast:  simulate_ssa (1) or simulate_ode (0) is used; boolean; default 0 (ode)
    #CANNOT HANDLE simulate_ssa CURRENTLY
    #sparse:    use sparse methods for integration?; boolean; 1
    #atol:  absolute tolerance for simulate_ode; real; 1e-8
    #rtol:  relative tolerance for simulate_ode; real; 1e-8
    #init_equil:  equilibrate the base .net model; boolean; default 1 (true)
    #re_equil:  equilibrate after each parameter bump but before simulation; boolean; default 1 (true)
    #n_steps:  the number of evenly spaced time points for sensitivity measures; integer;
    #default 50
    #suffix:  added to end of filename before extension; string; default ""

	#Variable Declaration and Initialization
	use strict;
	my $model;     #the BNG model
	my %params;    #the input parameter hash table
	my $net_file = "";
	my %inp_pert;
	my $t_end;
	my %readFileinputs;
	my %simodeinputs;
	my $simname;
	my $basemodel = BNGModel->new();
	my $plist;
	my $param_name;
	my $param_value;
	my $new_param_value;
	my $pperts;
	my $cperts;
	my $pert_names;
	my $pert_values;
#	my $pert_names;
#	my $pert_values;
	my $newbumpmodel = BNGModel->new();
	my $foo;
	my $i;

	#Initialize model and input parameters

	$model  = shift;
	my $params = shift;

	#Required params
	if ( defined( $params->{net_file} ) ) {
		$net_file = $params->{net_file};
	}
	else {
		$net_file = $model->getOutputPrefix();
	}
	if ( defined( $params->{t_end} ) ) {
		$t_end = $params->{t_end};
	}
	else {
		return ("t_end not defined");
	}

	#Optional params
	my $bump     = ( defined( $params->{bump} ) )     ? $params->{bump}     : 5;
	my $stochast = ( defined( $params->{stochast} ) ) ? $params->{stochast} : 0;
	my $sparse   = ( defined( $params->{sparse} ) )   ? $params->{sparse}   : 1;
	my $atol = ( defined( $params->{atol} ) ) ? $params->{atol} : 1e-8;
	my $rtol = ( defined( $params->{rtol} ) ) ? $params->{rtol} : 1e-8;
	my $init_equil =
	  ( defined( $params->{init_equil} ) ) ? $params->{init_equil} : 1;
	my $t_equil = ( defined( $params->{t_equil} ) ) ? $params->{t_equil} : 1e6;
	my $re_equil = ( defined( $params->{re_equil} ) ) ? $params->{re_equil} : 1;
	my $n_steps = ( defined( $params->{n_steps} ) ) ? $params->{n_steps} : 50;
	my $suffix  = ( defined( $params->{suffix} ) )  ? $params->{suffix}  : "";

	#Run base case simulation
	%readFileinputs = ( file => "$net_file.net" );
	$basemodel->readFile( \%readFileinputs );

	#if initial equilibration is required
	if ($init_equil) {
		$simname      = "_baseequil_";
		%simodeinputs = (
			prefix       => "$net_file$simname$suffix",
			t_end        => $t_equil,
			sparse       => $sparse,
			n_steps      => $n_steps,
			steady_state => 1,
			atol         => $atol,
			rtol         => $rtol
		);
		$basemodel->simulate_ode( \%simodeinputs );
	}
	$simname      = "_basecase_";
	%simodeinputs = (
		prefix       => "$net_file$simname$suffix",
		t_end        => $t_end,
		sparse       => $sparse,
		n_steps      => $n_steps,
		steady_state => 0,
		atol         => $atol,
		rtol         => $rtol
	);

	#Implement input perturbations
	if ( defined( $params->{inp_ppert} ) ) {
		$pperts      = $params->{inp_ppert};
		$pert_names  = $pperts->{pnames};
		$pert_values = $pperts->{pvalues};
		$i           = 0;
		while ( $pert_names->[$i] ) {
			$param_name  = $pert_names->[$i];
			$param_value = $pert_values->[$i];
			$basemodel->setParameter( $param_name, $param_value );
			$i = $i + 1;
		}
	}
	if ( defined( $params->{inp_cpert} ) ) {
		$cperts      = $params->{inp_cpert};
		$pert_names  = $cperts->{cnames};
		$pert_values = $cperts->{cvalues};
		$i           = 0;
		while ( $pert_names->[$i] ) {
			$param_name  = $pert_names->[$i];
			$param_value = $pert_values->[$i];
			$basemodel->setConcentration( $param_name, $param_value );
			$i = $i + 1;
		}
	}
	$basemodel->simulate_ode( \%simodeinputs );

	$plist = $basemodel->ParamList;

	#For every parameter in the model
	foreach my $model_param ( @{ $plist->Array } )
    {
		$param_name      = $model_param->Name;
		$param_value     = $model_param->evaluate();
		$new_param_value = $param_value * ( 1 + $bump / 100 );

		#Get fresh model and bump parameter
		$newbumpmodel->readFile( \%readFileinputs );
		$newbumpmodel->setParameter( $param_name, $new_param_value );

		#Reequilibrate
		if ($re_equil) {
			$simname = "equil_${param_name}";
			%simodeinputs = ( prefix       => "${net_file}_${simname}_${suffix}",
				              t_end        => $t_equil,
				              sparse       => $sparse,
				              n_steps      => $n_steps,
				              steady_state => 1,
				              atol         => $atol,
				              rtol         => $rtol
			                );
			$newbumpmodel->simulate_ode( \%simodeinputs );
		}

		#Implement input and run simulation
		$simname = $param_name;
		%simodeinputs = (
			prefix       => "${net_file}_${simname}_${suffix}",
			t_end        => $t_end,
			sparse       => $sparse,
			n_steps      => $n_steps,
			steady_state => 0,
			atol         => $atol,
			rtol         => $rtol
		);
		if ( defined( $params->{inp_ppert} ) ) {
			$pperts      = $params->{inp_ppert};
			$pert_names  = $pperts->{pnames};
			$pert_values = $pperts->{pvalues};
			$i           = 0;
			while ( $pert_names->[$i] ) {
				$param_name  = $pert_names->[$i];
				$param_value = $pert_values->[$i];
				$newbumpmodel->setParameter( $param_name, $param_value );
				$i = $i + 1;
			}
		}
		if ( defined( $params->{inp_cpert} ) ) {
			$cperts      = $params->{inp_cpert};
			$pert_names  = $cperts->{cnames};
			$pert_values = $cperts->{cvalues};
			$i           = 0;
			while ( $pert_names->[$i] ) {
				$param_name  = $pert_names->[$i];
				$param_value = $pert_values->[$i];
				$newbumpmodel->setConcentration( $param_name, $param_value );
				$i = $i + 1;
			}
		}
		$newbumpmodel->simulate_ode( \%simodeinputs );

		#Evaluate sensitivities and write to file

		#Get ready for next bump
		$newbumpmodel = BNGModel->new();
	}

  
}



###
###
###

1;
