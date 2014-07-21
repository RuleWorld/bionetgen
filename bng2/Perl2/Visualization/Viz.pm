package Viz;

use strict;
use warnings;
no warnings 'redefine';

use Class::Struct;

use StructureGraph;
use NetworkGraph;
use GML;


struct Graphs =>
{
	'RuleNames' => '@',
	'RuleStructureGraphs' => '@',
	'RulePatternGraphs' => '@',
	'RuleGroups' => '@',
};

sub uniq  { my %seen = (); grep { not $seen{$_}++ } @_; }
sub has { scalar grep ( $_ eq $_[1], @{$_[0]}); }
sub flat  { map @$_, @_; }
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
	
	if (not has(\@validtypes,$type) ) 
	{
		print $type." is an invalid type.\n";
		return $err;
	}
	
	if (not defined $model->VizGraphs) 
	{ 
		my $gr = Graphs->new();
		$model->VizGraphs($gr);
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
		getRuleNetworkGraphs($model);
	}
	
	# print stuff here;
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
		if ($output==1 and $each==0)
			{
			my @x = flat(@{$gr->{'RuleNetworkGraphs'}});
			my $bpg = combine3(\@x);
			uniqNetworkGraph($bpg);
			addWildcards($bpg);
			uniqNetworkGraph($bpg);
			if ($groups==0) { $str = toGML_rule_network($bpg); }
			}
	}
	

	if ($output==1 and $each==0)
	{
		my $suffix = $args{'suffix'};
		my %params = ('model'=>$model,'str'=>$str,'suffix'=>$suffix,'type'=>$type);
		writeGML(\%params);
	}

	if ($output==1 and $each==1)
	{
		my @names = map {@$_;} flat($gr->{'RuleNames'});
		map { 
			my %params = ('model'=>$model,'str'=>$strs[$_],'suffix'=>$names[$_],'type'=>$type);
			writeGML(\%params);
			}	(0..@names-1);
	}
	
	return '';
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

1;
