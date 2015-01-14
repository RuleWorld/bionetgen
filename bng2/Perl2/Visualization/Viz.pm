package Viz;

use strict;
use warnings;
no warnings 'redefine';

use Class::Struct;
use SpeciesGraph;
use Visualization::StructureGraph;
use Visualization::NetworkGraph;
use Visualization::ProcessGraph;
use Visualization::ContactMap;
use Visualization::GML;


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
sub indexHash 
{ 
	my @x = @{$_[0]};  
	map { $x[$_]=>$_ } 0..@x-1;
}

sub push2ref
{
	my $arr = shift @_;
	my $item = shift @_;
	if(ref $item) { push @$arr,@$item; }
	else { push @$arr,$item; }
	return;
}

##########################
sub initializeExecParams
{
	my $toggle1 = 1;

	my @include = ();
	my @exclude = ();
	my $background = {'toggle'=>$toggle1,'include'=>\@include,'exclude'=>\@exclude};
	my $each = 0;
	my $groups = 0;
	my $embed = 0;
	my $classes = {};
	
	my $toggle2 = 0;
	my @items = ();
	my $level = 1;
	my $filter = {'toggle'=>$toggle2,'items'=>\@items,'level'=>$level};
	my %x = ('background'=>$background,'each'=>$each,'groups'=>$groups,'classes'=>$classes,'filter'=>$filter,'embed'=>0);
	return %x;
	
}

sub getExecParams
{
	my %args = %{shift @_};
	my %exec_params = initializeExecParams();
	my %toggle;
	my %background;
	my %classes;
	
	foreach my $file(@{$args{'opts'}})
	{
		my ($x,$y,$z,$f) = parseOpts($file);
		my %toggle = %$x;
		my %background = %$y;
		my %classes = %$z;
		my %filter = %$f;
		
		# do toggles;
		foreach my $key(keys %toggle)
		{
			if($key eq 'background') 
			{ $exec_params{'background'}->{'toggle'} = $toggle{$key}; }
			
			if(has(['each','groups'],$key)) 
			{ $exec_params{$key} = $toggle{$key}; }
		}
		# do background
		foreach my $key(keys %background)
		{
			my $ref = $exec_params{'background'}->{$key};
			push2ref($ref,$background{$key});
		}
		foreach my $key(keys %classes)
		{
			my $classes2 = $exec_params{'classes'};
			my @arr = ();
			if(not defined $classes2->{$key}) { $classes2->{$key} = \@arr; }
			push2ref($classes2->{$key},$classes{$key});
		}
		if(defined $filter{'items'}) 
		{
			if (not defined $exec_params{'filter'}->{'items'}) {$exec_params{'filter'}->{'items'} = [];};
			push2ref($exec_params{'filter'}->{'items'},$filter{'items'});
		}
		
	}
	
	$exec_params{'type'} = $args{'type'};
	$exec_params{'background'}->{'toggle'} = $args{'background'};
	$exec_params{'filter'}->{'toggle'} = $args{'filter'};
	$exec_params{'each'} = $args{'each'};
	$exec_params{'groups'} = $args{'groups'};
	$exec_params{'collapse'} = $args{'collapse'};
	$exec_params{'textonly'} = $args{'textonly'};
	$exec_params{'suffix'} = $args{'suffix'};
	$exec_params{'mergepairs'} = $args{'mergepairs'};
	$exec_params{'embed'} = $args{'embed'};
	
	if(defined $args{'level'}) { $exec_params{'filter'}->{'level'} = $args{'level'} };
	return \%exec_params;
}
##########################

sub execute_params
{
	my $model = shift @_;
	my %args = %{shift @_};
	
	my @argkeys = keys %args;
	my $err = ''; #"visualize() error.";
	
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
	
	$args{'background'} = {} if (not has(\@argkeys,'background'));
	my @argkeys2 = keys %{$args{'background'}};
	$args{'background'}->{'toggle'} = 1 if(not has(\@argkeys2,'toggle'));
	$args{'background'}->{'include'} = [] if(not has(\@argkeys2,'include'));
	$args{'background'}->{'exclude'} = [] if(not has(\@argkeys2,'exclude'));

	#my @validtypes = qw (rule_pattern rule_operation rule_network reaction_network transformation_network contact process processpair );
	my @validtypes = qw (ruleviz_pattern ruleviz_operation regulatory reaction_network contactmap process );
	
	if (not has(\@argkeys,'type'))
	{
		$err =  "Visualization type unspecified. Use visualize({type=>\"string\"}),\n";
		$err .=	"string being one of (";
		foreach my $v (@validtypes){
			$err .= " $v";
		}
		$err .= " ).";
		return $err;
	}
	
	if (not has(\@validtypes,$args{'type'}) ) 
	{
		$err = "Visualization error: '" . $args{'type'} . "' is an invalid type.\n";
		return $err;
	}

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
	if(not ref $ref1) { 
		$err = "Atomic Pattern could not be created from ".$ref1."\n"."An atomic pattern is either \n\tA binding site, e.g. A(b),\n\tAn internal state, e.g. A(b~x),\n\tA bond, e.g. A(b!1).B(a!1), or\n\tA molecule, e.g. A.\n"; 
		return $err;
	}
	else {@includes = @$ref1};
	
	my @excludes = ();
	my $ref2 = getAtomicPatterns($bkg_exclude);
	if(not ref $ref2) { 
		$err = "Atomic Pattern could not be created from ".$ref2."\n"."An atomic pattern is either \n\tA binding site, e.g. A(b),\n\tAn internal state, e.g. A(b~x),\n\tA bond, e.g. A(b!1).B(a!1), or\n\tA molecule, e.g. A.\n"; 
		return $err;
	}
	else {@excludes = @$ref2};
	
	my %classes;
	foreach my $name(keys %classdefs)
	{
		# we're converting class1:[item1,item2] to the form item1:class1, item2:class1
		my $ref = getAtomicPatterns($classdefs{$name});
		if(not ref $ref) { 
			$err = "Atomic Pattern could not be created from ".$ref."\n"."An atomic pattern is either \n\tA binding site, e.g. A(b),\n\tAn internal state, e.g. A(b~x),\n\tA bond, e.g. A(b!1).B(a!1), or\n\tA molecule, e.g. A.\n"; 
			return $err;
		}
		else {@classes {@$ref} = ($name) x @$ref; }
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
				my %params = ('model'=>$model,'str'=>$str,'suffix'=>($suffix ? $suffix.'_'.$grpname : $grpname),'type'=>$type);
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
				my %params = ('model'=>$model,'str'=>$str,'suffix'=>($suffix ? $suffix.'_'.$grp[0] : $grp[0]),'type'=>$type);
				if($output==1)
				{
					if($textonly==1) { writeText(\%params); }
					else { writeGML(\%params); }
				}
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
		#$args2{'embed'} = 0;
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
			my %params = ('model'=>$model,'str'=>$strs[$_],'suffix'=>($suffix ? $suffix.'_'.$names[$_] : $names[$_]),'type'=>$type);
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
	my $mtypes = $model->MoleculeTypesList->MolTypes;
	my @statepatternlist;
	foreach my $mname(keys %$mtypes)
	{
		my $ctypes = $$mtypes{$mname}->Components;
		foreach my $comp(@$ctypes)
			{
				my $cname = $comp->Name;
				my @states = @{$comp->States};
				for(my $i=0; $i<@states; $i++)
					{
					my $str = $mname."(".$cname."~".$states[$i].")";
					push @statepatternlist, $str;
					}
			}
	
	}
	if(not defined $gr->{'ContactMap'})
	{
		my $psg = makeContactMap( [ flat(@{$gr->{'RuleNetworkGraphs'}}) ], \@statepatternlist );
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
			next if($tr eq "");
			my ($reac,$prod) = getReactantsProducts($tr);
			# dang, Perl is awesome! 
			map { $re{$_}++ if(not $added{$_}++); } @$reac;
			map { $pr{$_}++ if(not $added{$_}++); } @$prod;
		}
		my @bkg1 = grep { has($exclude,$_)==0; } keys %re;
		my @bkg2 = grep { has($include,$_)==1; } keys %pr;
		my @bkg3 = grep { has(\@bkg2,$_)==0;} @$include;
		my @background = (@bkg1,@bkg2,@bkg3);
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

#########################
sub display_viz_help
{
	my $args = @_ ? shift @_ : 0;
	
	if($args==0) {
		print qq{
---------------------------------------------/ HELP MENU /----------
ACTION: visualize
SYNOPSIS:

  visualize({help=>1})                 show this help menu
  visualize({help=>1,type=>"TYPE"})    show help on visualization of type TYPE
  visualize({type=>"TYPE"})            make visualization of type TYPE
  
  Allowed types: {ruleviz_pattern, ruleviz_operation, regulatory, reaction_network, contactmap, process}

OPTIONS:

  File I/O
  --------
  opts=>"FILE"                         use input options file FILE
  opts=>["FILE1","FILE2",...]          use multiple input options files
  textonly=>0/1                        disable/enable text-only output of visualization (default: 0)
  suffix=>"STR"                        add suffix STR to output filename

  Viz config
  ----------
  type=>"TYPE"                         create visualization of type TYPE (default: regulatory)
  background=>0/1                      disable/enable background (default: 0)
  groups=>0/1                          disable/enable grouping (default: 0)
  each=>0/1                            disable/endable separate outputs of each rule/group (default: 0)
}
	}
	
	elsif($args->{'type'} eq 'regulatory')
	{
		print qq{
---------------------------------------------/ HELP MENU /----------
ACTION: visualize
TYPE: regulatory
PURPOSE: Generates a regulatory network of rules and atomic patterns.

OPTIONS SET 1:

visualize({type=>"regulatory", background=>0/1, opts=>[FILE1,FILE2,...], filter=>0/1, level=>INT, groups=>0/1})

background=>0/1 (default: 0)
----------------------------
Disable/enable background. When 0, some patterns are determined to 
be background and removed from network graph. The assignment can be 
modified using the options file.

opts=>["FILE1","FILE2",...]
---------------------------
Specifies option file(s) with the following structure:

	begin background
			<atomic patterns>
	end background
	begin filter
			<atomic patterns>
	end filter
	begin classes
		begin classname
			<atomic patterns>
		end classname
	end classes
		
<atomic patterns> is a whitespace-separated list of patterns.
'classname' refers to arbitrary names for pattern classes.
All blocks are optional.

filter=>0/1, level=>INT
(Defaults: filter=>0, level=>1)
-------------------------------
Disable/enable generation of a subgraph starting from a defined 
set of nodes and propagating along the edges 'level' levels deep. 
The starting nodes are defined in the options file.

groups=>0/1 (default: 0)
------------------------
Disables/enables grouping. Patterns are grouped using classes
that are provided in the options file. Rules are grouped 
automatically based on pattern relationships.
		
OPTIONS SET 2:

visualize({type=>"regulatory", groups=>0/1, collapse=>0/1, textonly=>0/1})

collapse=>0/1
-------------
Disable/enable computation of a smaller network graph where groups of nodes 
are replaced by a single node representing the group. Requires groups=>1.
 
textonly=>0/1
-------------
Disable/enable output of a text-only version of the model network graph.
		
OPTIONS SET 3:
		
visualize({groups=>0/1, each=>0/1})
		
each=>0/1
---------
Disable/enable separate output of each rule (or rule group if groups=>1).
		
Option sets 2 and 3 are compatible with set 1 but incompatible with each other.
		
For help on file input and output options try: visualize({help=>1})
To start working on a model try:
		
visualize({type=>"regulatory", groups=>1})
}
	}
	
	elsif($args->{'type'} eq 'process')
	{
		print qq{
---------------------------------------------/ HELP MENU /----------
ACTION: visualize
TYPE: process
PURPOSE: Generates a process diagram of rules and atomic patterns.

USAGE:

visualize({type=>"process", background=>0/1, opts=>[FILE1,FILE2,...], groups=>0/1, mergepairs=>0/1, textonly=>0/1})

background=>0/1 (default: 0)
----------------------------
Disable/enable background. When 0, some patterns are determined to 
be background and removed from network graph. The assignment can be 
modified using the options file.

opts=>["FILE1","FILE2",...]
---------------------------
Specifies option file(s) with the following structure:

	begin background
		begin include
			<atomic patterns>
		end include
		
		begin exclude
			<atomic patterns>
		end exclude
	end background
	
	begin filter
		<atomic patterns>
	end filter
	
	begin classes
		begin classname
			<atomic patterns>
		end classname
	end classes

<atomic patterns> is a whitespace-separated list of patterns.
'classname' refers to arbitrary names for pattern classes.
All blocks are optional.

groups=>0/1 (default: 0)
------------------------
Disables/enables grouping. Patterns are grouped using classes
that are provided in the options file. Rules are grouped 
automatically based on pattern relationships.

textonly=>0/1
-------------
Disable/enable output of a text-only version of the model network graph.

For help on file input and output options try: visualize({help=>1})
To start working on a model try:

visualize({type=>process, groups=>1})
}
	}
	
	elsif($args->{'type'} eq 'ruleviz_pattern')
	{
		print qq{
---------------------------------------------/ HELP MENU /----------
ACTION: visualize
TYPE: ruleviz_pattern
PURPOSE: Visualizes rules as bipartite graphs with embedded pattern graphs.
USAGE:

visualize({type=>"ruleviz_pattern", each=>0/1})

each=>0/1 (default: 0)
---------
Disable/enable separate output of each rule.
}
	}
	
	elsif($args->{'type'} eq 'ruleviz_operation')
	{
		print qq{
---------------------------------------------/ HELP MENU /----------
ACTION: visualize
TYPE: ruleviz_operation
PURPOSE: Visualizes rules as pattern graphs with graph operation nodes.
USAGE:

visualize({type=>"ruleviz_operation", each=>0/1})

each=>0/1 (default: 0)
---------
Disable/enable separate output of each rule.

	}
	}
	elsif($args->{'type'} eq 'reaction_network')
	{
		print qq{
No help yet for 'reaction_network'. Please check back later.
}
	}
	
	elsif($args->{'type'} eq 'contactmap')
	{
		print qq{
---------------------------------------------/ HELP MENU /----------
ACTION: visualize
TYPE: ruleviz_pattern
PURPOSE: Visualizes rules as bipartite graphs with embedded pattern graphs.
USAGE:

visualize({type=>"contactmap", each=>0/1})

each=>0/1 (default: 0)
---------
Disable/enable separate output of each rule.
}
	}
	
	else
	{
		print $args->{'type'} . " is not a valid visualization type.\n";
	}
	
	return "";
}

1;
