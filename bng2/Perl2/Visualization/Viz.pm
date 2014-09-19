package Viz;

use strict;
use warnings;
no warnings 'redefine';

use Class::Struct;
use SpeciesGraph;
use StructureGraph;
use NetworkGraph;
use ProcessGraph;
use ContactMap;
use GML;


struct Graphs =>
{
	'RuleNames' => '@',
	'RuleStructureGraphs' => '@',
	'RulePatternGraphs' => '@',
	'RuleNetwork' => '$',
	'RuleNetworkCurrent' => '$',
	'ContactMap' => '$',
	'Background' => '@', # array of atomic patterns
	'Classes' => '%', # classname => \@arrayofnodes
	'NewName' => '$', # just a number to keep track of generated new names
};

sub uniq  { my %seen = (); grep { not $seen{$_}++ } @_; }
sub has { scalar grep ( $_ eq $_[1], @{$_[0]}); }
sub flat  { map @$_, @_; }
sub unquotemeta(;$) {
    my ($string) = scalar(@_) ? $_[0] : $_;    # quotemeta() "If EXPR is omitted, uses $_."
    return '' if !defined $string;             # quotemeta() undef behavior

    $string =~ s/(?:\\(?!\\))//g;
    $string =~ s/(?:\\\\)/\\/g;

    return $string;
}

sub uniqadd { if (not has($_[0],$_[1]) ) {push @{$_[0]}, $_[1] ; }}
sub indexHash { my @x = @{$_[0]}; map { $x[$_]=>$_ } 0..@x-1; }

sub push2ref
{
	my $arr = shift @_;
	my $item = shift @_;
	if(ref $item) { push @$arr,@$item; }
	else { push @$arr,$item; }
	return;
}

sub execute_params
{
	my $model = shift @_;
	my %args = %{shift @_};
	
	my @argkeys = keys %args;
	my $err = "visualize() error.";
	
	$args{'output'} = 1 if (not has(\@argkeys,'output'));
	if($args{'output'} == 1) {print "Executing visualize() command.\n"; }
	$args{'suffix'} = '' if (not has(\@argkeys,'suffix'));
	$args{'each'} = 0 if (not has(\@argkeys,'each'));
	$args{'groups'} = 0 if (not has(\@argkeys,'groups'));
	$args{'collapse'} = 0 if (not has(\@argkeys,'collapse'));
	#$args{'filter'} = {'items'=>[],} if (not has(\@argkeys,'collapse'));
	$args{'textonly'} = 0 if (not has(\@argkeys,'textonly'));
	$args{'embed'} = 0 if(not has(\@argkeys,'embed'));
	$args{'classes'} = {} if (not has(\@argkeys,'classes'));
	
	$args{'background'} = {}
			if (not has(\@argkeys,'background'));;
	my @argkeys2 = keys %{$args{'background'}};
	$args{'background'}->{'toggle'} = 1 if(not has(\@argkeys2,'toggle'));
	$args{'background'}->{'include'} = [] if(not has(\@argkeys2,'include'));
	$args{'background'}->{'exclude'} = [] if(not has(\@argkeys2,'exclude'));

	
	if (not has(\@argkeys,'type'))
	{
		print "Visualization type unspecified. Use visualize({type=>string}).\n";
		print "string being one of rule_pattern, rule_operation, rule_network, reaction_network, transformation_network, contact, process, processpair.\n";
		return $err;
	}
	
	#my @validtypes = qw (rule_pattern rule_operation rule_network reaction_network transformation_network contact process processpair );
	my @validtypes = qw (ruleviz_pattern ruleviz_operation regulatory reaction_network contactmap process );
	my $type = $args{'type'};
	my $output = $args{'output'};
	my $each = $args{'each'};
	my $groups = $args{'groups'};
	my $background = $args{'background'};
	my $except = $args{'except'};
	my $collapse = $args{'collapse'};
	my $filter = $args{'filter'};
	my $textonly = $args{'textonly'};
	my $suffix = $args{'suffix'};
	my %classdefs = %{$args{'classes'}};
	my $embed = $args{'embed'};
	
	my $bkg = $args{'background'};
	my $bkg_toggle = $bkg->{'toggle'};
	my $bkg_include = $bkg->{'include'};
	my $bkg_exclude = $bkg->{'exclude'};
	
	#my $closed = $args{'closed'};
	
	my @includes = ();
	my $ref1 = getAtomicPatterns($bkg_include);
	if(not ref $ref1) { print "\nAtomic Pattern could not be created from ".$ref1."\n"."An atomic pattern is either \n\tA binding site, e.g. A(b),\n\tAn internal state, e.g. A(b~x),\n\tA bond, e.g. A(b!1).B(a!1), or\n\tA molecule, e.g. A.\n"; return $err;}
	else {@includes = @$ref1};
	
	
	my @excludes = ();
	my $ref2 = getAtomicPatterns($bkg_exclude);
	if(not ref $ref2) { print "\nAtomic Pattern could not be created from ".$ref2."\n"."An atomic pattern is either \n\tA binding site, e.g. A(b),\n\tAn internal state, e.g. A(b~x),\n\tA bond, e.g. A(b!1).B(a!1), or\n\tA molecule, e.g. A.\n"; return $err;}
	else {@excludes = @$ref2};
	
	my %classes;
	foreach my $name(keys %classdefs)
	{
		# we're converting class1:[item1,item2] to the form item1:class1, item2:class1
		my $ref = getAtomicPatterns($classdefs{$name});
		if(not ref $ref) { print "\nAtomic Pattern could not be created from ".$ref."\n"."An atomic pattern is either \n\tA binding site, e.g. A(b),\n\tAn internal state, e.g. A(b~x),\n\tA bond, e.g. A(b!1).B(a!1), or\n\tA molecule, e.g. A.\n"; return $err;}
		else {@classes {@$ref} = ($name) x @$ref; }
	}
	
	if (not has(\@validtypes,$type) ) 
	{
		print $type." is an invalid type.\n";
		return $err;
	}
	
	if (not defined $model->VizGraphs) 
	{ 
		my $gr = Graphs->new();
		$model->VizGraphs($gr);
		$gr->{'NewName'} = -1;
	}
	
	my $gr = $model->VizGraphs;
	my @rrules = @{$model->RxnRules};
	my $str = '';
	my @strs = ();
	my @groups;
	
	getRuleNames($model);
	if ($type eq 'ruleviz_operation')
	{
		getRuleStructureGraphs($model);
		if($output==1 and $each==0)
		{	
			my @rsgs = map {@$_;} flat($gr->{'RuleStructureGraphs'});
			my $rsg = combine2(\@rsgs);
			#print printStructureGraph($rsg);
			$str = toGML_rule_operation($rsg);
		}
		if($output==1 and $each==1 and $groups==0)
		{	
			my @rsgs = map {@$_;} flat($gr->{'RuleStructureGraphs'});
			foreach my $rsg(@rsgs)
			{
				$str = toGML_rule_operation($rsg);
				push @strs,$str;
			}
		}
	}
	
	
	if ($type eq 'ruleviz_pattern')
	{
		getRulePatternGraphs($model);
		if($output==1 and $each==0)
		{
			my @rsgs = map {@$_;} flat($gr->{'RulePatternGraphs'});
			my $rsg = combine2(\@rsgs);
			#print printStructureGraph($rsg);
			$str = toGML_rule_pattern($rsg);
		}
		if($output==1 and $each==1 and $groups==0)
		{	
			my @rsgs = map {@$_;} flat($gr->{'RulePatternGraphs'});
			foreach my $rsg(@rsgs)
			{
				$str = toGML_rule_pattern($rsg);
				push @strs,$str;
			}
		}
	}
	
	if ($type eq 'contactmap')
	{
		getContactMap($model);
		if($output==1)
		{
			my $str = toGML_pattern($gr->{'ContactMap'});
			my $suffix = $args{'suffix'};
			my %params = ('model'=>$model,'str'=>$str,'suffix'=>$suffix,'type'=>$type);
			writeGML(\%params);
			return;
		
		}
	}
	
	if ($type eq 'regulatory')
	{
		# printing rule network (using each or without each)
		getRuleNetwork($model);
		applyRuleNetworkCurrent($model,$model->VizGraphs->{'RuleNetwork'});
		if ($each==0)
		{
			if ($filter->{'toggle'}==1 and defined $filter->{'items'})
			{ 
				my $bpg = $gr->{'RuleNetworkCurrent'};
				my @items= @{$filter->{'items'}}; 
				my $level = $filter->{'level'};
				print "Filtering network based on user-provided list.\n";
				$bpg = filterNetworkGraphByList($bpg,\@items,$level);
				applyRuleNetworkCurrent($model,$bpg);
			}
			
			if ($bkg_toggle==0)
			{
				my $bpg = $gr->{'RuleNetworkCurrent'};
				getBackground($model,\@includes,\@excludes,$bpg);
				print "Filtering background.\n";
				$bpg = filterNetworkGraph($bpg,$gr->{'Background'});
				applyRuleNetworkCurrent($model,$bpg);
			}

			if ($groups==1)
			{
				my $bpg = $gr->{'RuleNetworkCurrent'};
				print "Creating classes of atomic patterns and rules.\n";
				syncClasses($model,$bpg,\%classes);
				if($collapse==1)
					{ 
					print "Collapsing network graph using equivalence classes.\n";
					$bpg = collapseNetworkGraph($bpg); 
					}
				applyRuleNetworkCurrent($model,$bpg);
			}
			if($output==1)
			{
				my $bpg = $gr->{'RuleNetworkCurrent'};
				if ($textonly==1)
				{
					$str = printNetworkGraph($bpg);
					$output = 0;
				}
				else
				{
					$str = toGML_rule_network($bpg,$embed);
				}
			}
		}
		if ($each ==1)
		{
			
			my %args2 = duplicate_args(\%args);
			$args2{'output'} = 0;
			$args2{'each'} = 0;
			$args2{'collapse'} = 0;
			execute_params($model,\%args2);
			my $bpg = $model->VizGraphs->{'RuleNetworkCurrent'};
			my %classes = ($groups==1) ? %{$bpg->{'NodeClass'}} : ();
			my %nodetype = %{$bpg->{'NodeType'}};
			my @edges = @{$bpg->{'EdgeList'}};
			my @wcs = uniq(map {$_ =~ /^(.*):.*:.*/; $1; } grep /Wildcard/,@edges);
			my @groups = 	uniq map {$classes{$_}}
							grep {has([keys %classes],$_)}
							grep {$nodetype{$_} eq 'Rule'}
							@{$bpg->{'NodeList'}};
			
			# classed rules
			foreach my $i(0..@groups-1)
			{
				my $grpname = $groups[$i];
				# check if group is for rules or aps;
				my @grp = grep { $classes{$_} eq $grpname } keys %classes;
				my $grptype = $nodetype{$grp[0]};
				my $bpg2 = filterNetworkGraphByList($bpg,\@grp,1);
				my $str = ($textonly==1) ? printNetworkGraph($bpg2) : toGML_rule_network($bpg2,$embed);
				my %params = ('model'=>$model,'str'=>$str,'suffix'=>$grpname,'type'=>$type);
				if($output==1)
				{
					if($textonly==1) { writeText(\%params); }
					else { writeGML(\%params); }
				}
			}
			# unclassed rules
			my @unclassed_rules = 	grep { not has([keys %classes],$_) }
									grep {$nodetype{$_} eq 'Rule'} 
									@{$bpg->{'NodeList'}};
			foreach my $i(0..@unclassed_rules-1)
			{
				my @grp = ($unclassed_rules[$i]);
				my $bpg2 = filterNetworkGraphByList($bpg,\@grp,1);
				# right now this is not efficient
				# better way is to regenerate the network graph, but then u'll have to apply background n
				# filter n other things again... boring!
				my $str = ($textonly==1) ? printNetworkGraph($bpg2) : toGML_rule_network($bpg2,$embed);
				my %params = ('model'=>$model,'str'=>$str,'suffix'=>$grp[0],'type'=>$type);
				if($output==1)
				{
					if($textonly==1) { writeText(\%params); }
					else { writeGML(\%params); }
				}
			}
		}
		
		# printing pattern space
		if($output == 1 and $collapse ==0)
		{
			if($textonly==0)
			{
				my $bpg = $model->VizGraphs->{'RuleNetworkCurrent'};
				my %classes = (defined $bpg->{'NodeClass'}) ? %{$bpg->{'NodeClass'}} : ();
				my %nodetype = %{$bpg->{'NodeType'}};
				my @aps = grep {$nodetype{$_} eq 'AtomicPattern'} @{$bpg->{'NodeList'}};
				my @classed = grep { has([keys %classes],$_) } @aps;
				my @apclasses = uniq map { $classes{$_} } @classed;
				my @unclassed = grep { not has(\@classed, $_) } @aps;
				my @pats = ();
			
				my $j = -1;
				foreach my $i(0..@apclasses-1)
				{
					$j++;
					my $apclass = $apclasses[$i];
					my @grp = grep {$classes{$_} eq $apclass} @classed;
					my @psgs = map { stringToPatternStructureGraph($grp[$_],$_) } 0..@grp-1;
					my $psg = combine(\@psgs,$j);
					my $psg2 = addPatternNode($psg,$j,'',$apclass);
					push @pats, $psg2;
				}
				foreach my $i(0..@unclassed-1)
				{
					$j++;
					my $psg = stringToPatternStructureGraph($unclassed[$i],$j);
					push @pats, $psg;
				}
				
				my $psg = combine2(\@pats);
				my $str = toGML_pattern($psg);
				my %params = ('model'=>$model,'str'=>$str,'suffix'=>'patterns','type'=>$type);
				writeGML(\%params);
			}
			else
			{
				my $bpg = $model->VizGraphs->{'RuleNetworkCurrent'};
				my $str = printNetworkGraph($bpg);
				my %params = ('model'=>$model,'str'=>$str,'type'=>'regulatory');
				# this is the whole network, not the pattern space..
				# too bored to sit n filter this..
				writeText(\%params);
			}
		}
		
		if($each==1) {$output = 0;}
	}
	
	if($type eq 'process')
	{
		my %args2 = duplicate_args(\%args);
		$args2{'type'} = 'regulatory';
		$args2{'output'} = 0;
		#$args2{'groups'} = 0;
		$args2{'collapse'} = 0;
		$args2{'embed'} = 0;
		#if($args{'mergepairs'}) {$args2{'groups'}=1;$args{'groups'}=1;}
		#if($args{'groups'}) { $args2{'collapse'}=1; }
		execute_params($model,\%args2);
		my $bpg = $model->VizGraphs->{'RuleNetworkCurrent'};
		
		my $pg = makeProcessGraph2($bpg,\%args);
		if($args{'embed'}==1) {embedProcessGraph($pg,$gr,\%args);} 
		if($output==1)
			{
			if($textonly==1) {$str = printProcessGraph($pg); }
			else { $str = toGML_process2($pg); }
			}
	}
		
		
		
	
	if($textonly==1 and $each==0)
	{
		my %params = ('model'=>$model,'str'=>$str,'suffix'=>$suffix,'type'=>$type);	
		writeText(\%params);
		$output=0;
	}

	if ($output==1 and $each==0)
	{
		my $suffix = $args{'suffix'};
		my %params = ('model'=>$model,'str'=>$str,'suffix'=>$suffix,'type'=>$type,'groups'=>$groups);
		writeGML(\%params);
	}

	if ($output==1 and $each==1 and $groups==0)
	{
		my @names = map {@$_;} flat($gr->{'RuleNames'});
		map { 
			my %params = ('model'=>$model,'str'=>$strs[$_],'suffix'=>$names[$_],'type'=>$type);
			writeGML(\%params);
			}	(0..@names-1);
	}
	
	
	return '';
}

sub writeText
{
	my %params = %{shift @_};
	my $model = $params{'model'};
	my $str = $params{'str'};
	my $prefix = $model->getOutputPrefix();
	my $type = $params{'type'};
	my $suffix = (defined $params{'suffix'}) ? $params{'suffix'} : '';
	
	my %outputstr = (	'rule_operation' => 'rule(s) with graph operations',
						'rule_pattern' => 'rule(s) with patterns',
						'regulatory' => 'network of rules and atomic patterns',
						'patterns' => 'atomic patterns',);
	my $outputmsg = $outputstr{$type};
	
	my $file = '';
	$file .= $prefix;
	$file .= "_".$type;
	$file .= "_".$suffix if (length $suffix > 0);
	$file .= ".txt";
		
	# write the string to file
    my $FH;
    open($FH, '>', $file)  or  return "Couldn't write to $file: $!\n";
    print $FH $str;
    close $FH;

    # all done
    print sprintf( "Wrote %s in TXT format to %s.\n", $outputmsg, $file);
    return undef;
}

sub writeGML
{
	my %params = %{shift @_};
	my $model = $params{'model'};
	my $str = $params{'str'};
	my $prefix = $model->getOutputPrefix();
	my $type = $params{'type'};
	my $suffix = (defined $params{'suffix'}) ? $params{'suffix'} : '';
	
	my %outputstr = (	'ruleviz_operation' => 'rule(s) with graph operations',
						'ruleviz_pattern' => 'rule(s) with patterns',
						'regulatory' => 'network of rules and atomic patterns',
						'process' => 'process graph of rules',
						'contactmap' => 'contact map of model'
						);
	my $outputmsg = $outputstr{$type};
	
	my $file = '';
	$file .= $prefix;
	$file .= "_".$type;
	$file .= "_".$suffix if (length $suffix > 0);
	$file .= ".gml";
		
	# write the string to file
    my $FH;
    open($FH, '>', $file)  or  return "Couldn't write to $file: $!\n";
    print $FH $str;
    close $FH;

    # all done
    print sprintf( "Wrote %s in GML format to %s.\n", $outputmsg, $file);
    return undef;
}

# get methods for different graphs
sub getRuleNames
{
	my $model = shift @_;
	my $gr = $model->VizGraphs;
	if(not defined $gr->{'RuleNames'})
	{
		my @rrules = @{$model->RxnRules};
		my @names_arr = ();
		foreach my $rrule(@rrules)
		{
			my @names = map {$_->Name;} @$rrule;
			push @names_arr,\@names;
		}
		$gr->{'RuleNames'} = \@names_arr;
	}
	return;
}
sub getRuleStructureGraphs
{
	my $model = shift @_;
	my $gr = $model->VizGraphs;
	if (not defined $gr->{'RuleStructureGraphs'})
	{
		my @rrules = @{$model->RxnRules};
		my @rsgs_arr = ();
		my @names_arr = ();
		my $j = 0;
		foreach my $i(0..@rrules-1)
		{
			my @rrule = @{$rrules[$i]};
			my @rsgs = map makeRuleStructureGraph($_,$j++), @rrule;
			push @rsgs_arr, \@rsgs;
		}
		$gr->{'RuleStructureGraphs'} = \@rsgs_arr; 
	}
	return;
}
sub getRulePatternGraphs
{
	my $model = shift @_;
	my $gr = $model->VizGraphs;
	
	if (not defined $gr->{'RulePatternGraphs'})
	{
		my @rrules = @{$model->RxnRules};
		my @rsgs_arr = ();
		my $j = 0;
		foreach my $i(0..@rrules-1)
		{
			my @rrule = @{$rrules[$i]};
			my @rsgs = map makeRulePatternGraph($_,$j++), @rrule;
			push @rsgs_arr, \@rsgs;
		}
		$gr->{'RulePatternGraphs'} = \@rsgs_arr; 
	}
	return;
}
sub getRuleNetworkGraphs
{
	my $model = shift @_;
	my $gr = $model->VizGraphs;
	getRuleStructureGraphs($model);
	if (not defined $gr->{'RuleNetworkGraphs'})
	{
		print "Building network graphs for individual rules.\n";
		my @rsgs_arr = @{$gr->{'RuleStructureGraphs'}};
		my @names_arr = @{$gr->{'RuleNames'}};
		my @bpgs_arr = ();
		foreach my $i(0..@rsgs_arr-1)
		{
			my @names = @{$names_arr[$i]};
			my @rsgs = @{$rsgs_arr[$i]};
			my @bpgs;
			foreach my $j(0..@rsgs-1)
				{
				my $bpg = makeRuleNetworkGraph($rsgs[$j],$names[$j]);
				push @bpgs, $bpg;
				}
			push @bpgs_arr,\@bpgs;
		}
		$gr->{'RuleNetworkGraphs'} = \@bpgs_arr;
	}
	return;
}
sub getRuleNetwork
{
	#my ($model,$groups,$background,$excepts,$classes) = @_;
	my $model = shift @_;
	my $gr = $model->VizGraphs;
	getRuleNetworkGraphs($model);
	my $bpg;
	if (not defined $gr->{'RuleNetwork'})
	{
		print "Compiling network graph for whole model.\n";
		$bpg = mergeNetworkGraphs(flat(@{$gr->{'RuleNetworkGraphs'}}));
		$bpg->{'Merged'} = 1;
		$bpg->{'Collapsed'} = 0;
		$gr->{'RuleNetwork'} = $bpg;
	}
	return;
}


sub getContactMap
{
	my $model = shift @_;
	my $gr = $model->VizGraphs;
	getRuleNetworkGraphs($model);
	if(not defined $gr->{'ContactMap'})
	{
		my $psg = makeContactMap( [ flat(@{$gr->{'RuleNetworkGraphs'}}) ] );
		$gr->{'ContactMap'} = $psg;
	}
	return;
}

sub applyRuleNetworkCurrent
{
	my $model = shift @_;
	my $bpg = shift @_;
	$model->VizGraphs->{'RuleNetworkCurrent'} = $bpg;
	return;
}
sub getRuleGroups
{
	my $model = shift @_;
	my $gr = $model->VizGraphs;
	my $bpg = $gr->{'RuleNetwork'};
	# this is to be used only by rule_pattern and rule_operation
	my %classes = %{$bpg->{'NodeClass'}};
	my @rules = grep {$bpg->{'NodeType'}->{$_} eq 'Rule'} @{$bpg->{'NodeList'}};
	

	
}
# other get methods
sub getAtomicPatterns
{
	my @stringarr = @{shift @_};
	my @x;
	foreach my $pat(@stringarr)
	{
		my $ap = stringToAtomicPattern($pat);
		if(length($ap) > 0)
			{
			push @x, $ap;
			}
		else { return $pat; }
	}
	#$arr = \@x;
	return \@x;
}
sub syncClasses
{
	my $model = shift @_;
	my $bpg = shift @_;
	my $classes_in = @_ ? shift @_ : undef;
	#print map $_." ".$$classes_in{$_}."\n", keys %$classes_in;
	
	my $gr = $model->VizGraphs;
	if(not defined $gr->{'Classes'})
		{
		$gr->{'Classes'} = {};
		}
		
	if(not defined $bpg->{'NodeClass'})
		{
		$bpg->{'NodeClass'} = {};
		}
	
	
	# get only the atomic patterns 
	my @aps = 	grep {$bpg->{'NodeType'}->{$_} eq 'AtomicPattern'} 
				@{$bpg->{'NodeList'}};
	
	# updating classes_in to include wildcards
	if(defined $classes_in)
	{
		my @wcs = grep /\!\+/, @aps;
		foreach my $wc(@wcs)
		{
		# get wildcard relations
		next if (has([keys %$classes_in],$wc));
		my @matches = 	map { $_ =~  /.*:(.*):.*/; $1;}
						grep { $_ =~ quotemeta $wc }
						grep { $_ =~ /Wildcard$/ } 
						@{$bpg->{'EdgeList'}} ;
		next if ( scalar( grep {has([keys %$classes_in],$_)==0} @matches));
		next if ( scalar( uniq( map {$classes_in->{$_}} @matches ) ) > 1 );
		$classes_in->{$wc} = $classes_in->{$matches[0]};
		#if(scalar(@matches) ==1) { $classes_in->{$wc} = $matches[0]; }
		}
	
	}
	
	# update bpg and model using %$classes_in
	if(defined $classes_in)
	{
		my @aps2 = grep { has([keys %$classes_in],$_); } @aps;
		map
			{
			$gr->{'Classes'}->{$_} = $classes_in->{$_};
			$bpg->{'NodeClass'}->{$_} = $classes_in->{$_};
			} @aps2;
	
	}
	
	# creating temp hash to hold for each AP
	# either the class (if classed) or 
	# its name (if unclassed)
	my @classed = keys %{$bpg->{'NodeClass'}};
	my @unclassed = grep has(\@classed,$_)==0, @aps;
	my %temp;
	@temp { @classed } = @{$bpg->{'NodeClass'}} { @classed };
	@temp { @unclassed } = @unclassed;
	
	# getting rules
	#my @rules =	map {quotemeta($_)}
	#			grep {$bpg->{'NodeType'}->{$_} eq 'Rule'} 
	#				@{$bpg->{'NodeList'}};
	my @edges =	grep { $_ =~ /Reactant|Product$/ }
				@{$bpg->{'EdgeList'}};
	my @reac_edges =	grep { $_ =~ /Reactant$/ }
				@{$bpg->{'EdgeList'}};
	my @prod_edges =	grep { $_ =~ /Product$/ }
				@{$bpg->{'EdgeList'}};
	my %reacprodhash;
	# dont wanna lose the order;
	my @reacprodvals; 
	my @rules = grep {$bpg->{'NodeType'}->{$_} eq 'Rule'} @{$bpg->{'NodeList'}};
	foreach my $rule(@rules)
	{
		my @reac = 	uniq 
					sort {$a cmp $b}
					map { $temp{$_} } 
					map { $_ =~ /.*:(.*):.*/; $1; }
					grep { $_ =~ /^(.*):.*:.*/; $1 eq $rule; } 
					@reac_edges;
		my @prod = 	uniq
					sort {$a cmp $b}
					map { $temp{$_} }
					map { $_ =~ /.*:(.*):.*/; $1; }
					grep { $_ =~ /^(.*):.*:.*/; $1 eq $rule; } 
					@prod_edges;
		my $str = join(" -> ", map { join(" + ", @$_); } (\@reac,\@prod) );
		my $hasreacprod = @reac ? 1 : @prod ? 1 : 0;
		my $reacprodstr = $hasreacprod ? $str : '';
		$reacprodhash{$rule} = $reacprodstr;
		push @reacprodvals,$reacprodstr;
		#print $rule.":".join(" ",@reac).":".join(" ",@prod)."\n";
	}
	
	
	# foreach my $rule(@rules)
	# {
		# my $reacprodstr = 	join " ",
						# sort {$a cmp $b}
						# uniq map {$temp{$_};}
						# map {$_ =~ /.*:(.*):.*/; $1;}
						# grep { $_ =~ /^$rule:/;}
						# @edges;
		# my @reac = 		map {$_ =~ /.*:(.*):.*/; $1;}
						# grep { $_ =~ /^$rule:/;}
						# @reac_edges;

		# my @reac1 = @reac ? uniq map {$temp{$_}} @reac : ();
		# my @prod = 		map {$_ =~ /.*:(.*):.*/; $1;}
						# grep { $_ =~ /^$rule:/;}
						# @prod_edges;
		# my @prod1 = @reac ? uniq map {$temp{$_}} @prod : ();
		# my $reacstr = @reac1 ? join(" ",sort {$a cmp $b}  @reac1) : '';
		# my $prodstr = @prod1 ? join(" ",sort {$a cmp $b}  @prod1) : '';
		# $reacprodstr = $reacstr." -> ".$prodstr;
		
		# $reacprodhash{unquotemeta $rule} = $reacprodstr;
		# push @reacprodvals,$reacprodstr;
		# #$reacprodhash{unquotemeta $rule} = $reacstr." -> ".$prodstr;
	# }
	
	#print map $_."\n",uniq(@reacprodvals);
	
	
	# get reacprodstrings that occur multiple times
	# if it occurs only once, it doesnt need a group
	# prune this to delete reacprods that occur only once
	my @reacprods = grep
					{
					my $x = $_;
					$x ne '' and (scalar (grep { $_ eq $x } values %reacprodhash) >1);
					} uniq @reacprodvals;
	return if (scalar @reacprods == 0);
	$gr->{'NewName'} = -1 if (not defined $gr->{'NewName'}); 
	
	# assigning names to rule groups
	my %names;
	@names{ @reacprods } = map 'RG'.++$gr->{'NewName'}, @reacprods;
	
	# updating model and bpg classdefs for the grouped rules
	my @rules2 = 	grep { has(\@reacprods,$reacprodhash{$_}) } 
					map unquotemeta, @rules;
	foreach my $rule(@rules2)
	{
		$gr->{'Classes'}->{$rule} = $names{$reacprodhash{$rule}};
		$bpg->{'NodeClass'}->{$rule} = $names{$reacprodhash{$rule}};
	}
	return;
}

sub getBackground
{
	print "Computing background.\n";
	my $model = shift @_;
	my $include = @_ ? shift @_ : [];
	my $exclude = @_ ? shift @_ : [];
	
	#my $except = @_ ? shift @_ : [];
	my $bpg = @_ ? shift @_ : undef;
	my $gr = $model->VizGraphs;
	
	if (not defined($gr->{'Background'}))
	{
		getRuleStructureGraphs($model);
		my @rsgs = map {@$_;} flat($gr->{'RuleStructureGraphs'});
		my @trs = uniq(map { getTransformations($_); } @rsgs);
		my %added;
		my %re;
		my %pr;
		foreach my $tr(@trs)
		{	
			my ($reac,$prod) = getReactantsProducts($tr);
			# dang, Perl is awesome! 
			map { $re{$_}++ if(not $added{$_}++); } @$reac;
			map { $pr{$_}++ if(not $added{$_}++); } @$prod;
		}
		my @bkg1 = grep { has($exclude,$_)==0; } keys %re;
		my @bkg2 = grep { has($include,$_)==1; } keys %pr;
		my @background = (@bkg1,@bkg2);
		$gr->{'Background'} = \@background;	
	}
	
	if(defined $bpg)
	{
		my @aps = grep {$bpg->{'NodeType'}->{$_} eq 'AtomicPattern'} @{$bpg->{'NodeList'}};
		my @edges = grep {$_ =~ /Reactant|Product|Wildcard$/} @{$bpg->{'EdgeList'}};
		
		my @aps_1 = 	map {$_ =~ /^.*:(.*):.*$/; $1; }
						grep {$_ =~ /Reactant|Product$/} 
						@{$bpg->{'EdgeList'}};
		my @aps_2 = 	map {$_ =~ /^(.*):.*:.*$/; $1; }
						grep {$_ =~ /Wildcard$/} 
						@{$bpg->{'EdgeList'}};
		my @aps_3 = uniq((@aps_1,@aps_2));
		my @aps_4 = grep {(has(\@aps_3,$_)==0 and has($exclude,$_)==0) or has($include,$_)==1} @aps;
		#print map $_."\n", grep {has(\@aps_3,$_)==0} @aps;
		
		#my @aps2 = grep {$_ =~ /\(.*\)/} uniq map { $_ =~ /.*:(.*):.*/; $1; } @edges;
		#my @aps3 = grep { (has(\@aps2,$_)==0 and has($exclude,$_)==0) or has($include,$_)==1 } @aps;
		
		my @background = @{$gr->{'Background'}};
		#push @background, @aps3;
		push @background, @aps_4;
		@background = uniq @background;
		$gr->{'Background'} = \@background;
	
	}
	return;
}


sub duplicate_args
{
	my %args = %{shift @_};
	my %args2;
	map {$args2{$_} = $args{$_}} keys %args;
	return %args2;
}
1;
