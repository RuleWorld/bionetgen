package Viz;

use strict;
use warnings;
no warnings 'redefine';

use Class::Struct;
use SpeciesGraph;
use StructureGraph;
use NetworkGraph;
use GML;


struct Graphs =>
{
	'RuleNames' => '@',
	'RuleStructureGraphs' => '@',
	'RulePatternGraphs' => '@',
	'RuleNetwork' => '$',
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



sub execute_params
{
	my $model = shift @_;
	my %args = %{shift @_};
	my @argkeys = keys %args;
	my $err = "visualize() error.";
	
	$args{'output'} = 1 if (not has(\@argkeys,'output'));
	$args{'suffix'} = '' if (not has(\@argkeys,'suffix'));
	$args{'each'} = 0 if (not has(\@argkeys,'each'));
	$args{'groups'} = 0 if (not has(\@argkeys,'groups'));
	$args{'collapse'} = 0 if (not has(\@argkeys,'collapse'));
	#$args{'filter'} = {'items'=>[],} if (not has(\@argkeys,'collapse'));
	$args{'textonly'} = 0 if (not has(\@argkeys,'textonly'));
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
	
	my @validtypes = qw (rule_pattern rule_operation rule_network reaction_network transformation_network contact process processpair );
	
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
	
	
	# getting the relevant structures
	getRuleNames($model);
	if ($type eq 'rule_operation')
		{
		getRuleStructureGraphs($model);
		}
	if ($type eq 'rule_pattern')
		{
		getRulePatternGraphs($model);
		}
	if ($type eq 'rule_network')
	{	
		getRuleNetwork($model);
	}	
	
	if ($type eq 'rule_operation')
	{
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
	
	
	if ($type eq 'rule_pattern')
	{
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
	
	
	
	
	
	if ($type eq 'rule_network')
	{
		my $bpg = $gr->{'RuleNetwork'};
		if ($output==1 and $each==0)
		{
			if (defined $filter)
			{ 
				my @items= @{$filter->{'items'}}; 
				my $level = $filter->{'level'};
				$bpg = filterNetworkGraphByList($bpg,\@items,$level);
			}
			
			if ($bkg_toggle==0)
			{
				getBackground($model,\@includes,\@excludes,$bpg);
				$bpg = filterNetworkGraph($bpg,$gr->{'Background'});
			}

			if ($groups==1)
			{
				syncClasses($model,$bpg,\%classes);
				if($collapse==1)
					{ $bpg = collapseNetworkGraph($bpg); }
			}	
			if ($textonly==1)
			{
				$str = printNetworkGraph($bpg);
				$output = 0;
			}
			else
			{
				$str = toGML_rule_network($bpg);
			}
		}
	}
	if($textonly==1)
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
						'rule_network' => 'network of rules and atomic patterns',	);
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
	
	my %outputstr = (	'rule_operation' => 'rule(s) with graph operations',
						'rule_pattern' => 'rule(s) with patterns',
						'rule_network' => 'network of rules and atomic patterns',	);
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
	my @rrules = @{$model->RxnRules};
	my @names_arr = ();
	foreach my $rrule(@rrules)
	{
		my @names = map {$_->Name;} @$rrule;
		push @names_arr,\@names;
	}
	$gr->{'RuleNames'} = \@names_arr;
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
		$bpg = mergeNetworkGraphs(flat(@{$gr->{'RuleNetworkGraphs'}}));
		$bpg->{'Merged'} = 1;
		$bpg->{'Collapsed'} = 0;
		$gr->{'RuleNetwork'} = $bpg;
	}
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
	
	# update bpg and model using %$classes_in
	if(defined $classes_in)
	{
		my %x = %$classes_in;
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
	my @rules =	map {quotemeta($_)}
				grep {$bpg->{'NodeType'}->{$_} eq 'Rule'} 
				@{$bpg->{'NodeList'}};
	my @edges =	grep { $_ =~ /Reactant|Product$/ }
				@{$bpg->{'EdgeList'}};
	my @reac_edges =	grep { $_ =~ /Reactant$/ }
				@{$bpg->{'EdgeList'}};
	my @prod_edges =	grep { $_ =~ /Product$/ }
				@{$bpg->{'EdgeList'}};
	my %reacprodhash;
	foreach my $rule(@rules)
	{
		my $reacprodstr = 	join " ",
						sort {$a cmp $b}
						uniq map {$temp{$_};}
						map {$_ =~ /.*:(.*):.*/; $1;}
						grep { $_ =~ /^$rule:/;}
						@edges;
		my $reacstr = 	join " + ",
						sort {$a cmp $b}
						uniq map {$temp{$_};}
						map {$_ =~ /.*:(.*):.*/; $1;}
						grep { $_ =~ /^$rule:/;}
						@reac_edges;
		my $prodstr = 	join " + ",
						sort {$a cmp $b}
						uniq map {$temp{$_};}
						map {$_ =~ /.*:(.*):.*/; $1;}
						grep { $_ =~ /^$rule:/;}
						@reac_edges;
		$reacprodhash{unquotemeta $rule} = $reacprodstr;
		#$reacprodhash{unquotemeta $rule} = $reacstr." -> ".$prodstr;
	}
	
	# get reacprodstrings that occur multiple times
	# if it occurs only once, it doesnt need a group
	# prune this to delete reacprods that occur only once
	my @reacprods = grep
					{
					my $x = $_;
					scalar (grep { $_ eq $x } values %reacprodhash) >1;
					} uniq values %reacprodhash;
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
		my @bkg2 = grep { has($include,$_)==1; } keys %pr;;
		my @background = (@bkg1,@bkg2);
		$gr->{'Background'} = \@background;	
	}
	
	if(defined $bpg)
	{
		my @aps = grep {$bpg->{'NodeType'}->{$_} eq 'AtomicPattern'} @{$bpg->{'NodeList'}};
		my @edges = grep {$_ =~ /Reactant|Product$/} @{$bpg->{'EdgeList'}};
		my @aps2 = grep {$_ =~ /\(.*\)/} uniq map { $_ =~ /.*:(.*):.*/; $1; } @edges;
		my @aps3 = grep { has(\@aps2,$_)==0 } @aps;
		my @background = @{$gr->{'Background'}};
		push @background, @aps3;
		@background = uniq @background;
		$gr->{'Background'} = \@background;
	
	}
	return;
}

1;
