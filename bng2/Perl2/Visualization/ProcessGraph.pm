package Viz;

use strict;
use warnings;
no warnings 'redefine';

use Class::Struct;
use NetworkGraph;
use GML;


struct ProcessGraph =>
{
	'Processes'	=> '@', 
	'Edges' => '@',
	'ReacProds' => '%',
	'Names' => '%',
	'Embed' => '%',
};

sub printProcessGraph
{
	my $pg = shift @_;
	my $str = ();
	$str .= "Processes:\n";
	$str .= join "\n", map { $_.":".$pg->{'Names'}->{$_} } @{$pg->{'Processes'}};
	$str .= "\n";
	$str .= "Influences:\n";
	$str .= join "\n", @{$pg->{'Edges'}};
	return $str;

}

sub makeProcessGraph
{
	print "Building process graph for whole model.\n";
	my $bpg = shift @_;
	my $mergepairs = @_ ? shift @_ : 0;
	my $embed = @_ ? shift @_ : 0;

	my @edges = @{$bpg->{'EdgeList'}};
	my @edges2;
	
	my @processes = grep {$bpg->{'NodeType'}->{$_} =~ /Rule/} @{$bpg->{'NodeList'}};
	my @wcs = uniq(map {$_ =~ /^(.*):.*:.*/; $1; } grep { $_ =~ /Wildcard/ } @edges);
	
	my %reacprod;
	my %context;
	my %reac;
	my %prod;
	map { my @x = (); $reacprod{$_} = \@x; } @processes;
	map { my @x = (); $context{$_} = \@x; } @processes;
	
	foreach my $proc(@processes)
	{
		my $r = quotemeta $proc;
		my @rps = 	uniq( map { $_ =~ /.*:(.*):.*/;  $1; }
					grep { $_ =~ /Reactant|Product/ }
					grep {$_ =~ /^$r:/ } @edges );
		if(@wcs) { push @rps, getWCs(\@rps,\@wcs,\@edges); }
		$reacprod{$proc} = \@rps;
				
		my @cont = 	uniq( map { $_ =~ /.*:(.*):.*/;  $1; }
					grep { $_ =~ /Context/ }
					grep {$_ =~ /^$r:/ } @edges );
		$context{$proc} = \@cont;
	}
	foreach my $r1(@processes)
	{
		foreach my $r2(@processes)
		{
			if(has_overlap($reacprod{$r1},$context{$r2}))
			{
				push @edges2, join(" ",($r1,$r2));
			}
		}
	}
	my %namesarr;
	my %bpgs;
	foreach my $proc(@processes)
	{
	my $r = quotemeta $proc;
	my @reacs = 	sort {$a cmp $b}
					uniq( map { $_ =~ /.*:(.*):.*/;  $1; }
					grep { $_ =~ /Reactant/ }
					grep {$_ =~ /^$r:/ } @edges );
	$reac{$proc} = \@reacs;
		
	my @prods = 	sort {$a cmp $b}
					uniq( map { $_ =~ /.*:(.*):.*/;  $1; }
					grep { $_ =~ /Product/ }
					grep {$_ =~ /^$r:/ } @edges );
	$prod{$proc} = \@prods;
	
	my $namearr = [[$proc],\@reacs,\@prods];
	#my $name = $proc."\n:".join("+",@reacs)."->".join("+",@prods)." }";
	#my $name = make_name($namearr);
	$namesarr{$proc} = $namearr;
	
	# building embed graph here
	if($embed)
		{
			my @embed_edges = grep { $_ =~ /Reactant|Product/ }
							  grep {$_ =~ /^$r:/ } @edges ;
			my @embed_nodes = uniq( map { $_ =~ /.*:(.*):.*/;  $1; } @embed_edges);
			push @embed_nodes,$proc;
			my %embed_nodetype;
			my %nodetype = %{$bpg->{'NodeType'}};
			@embed_nodetype { @embed_nodes } = @nodetype {@embed_nodes};
			my $bpg2 = makeRuleNetworkGraph_simple(\@embed_nodes,\@embed_edges,\%embed_nodetype,$proc);
			$bpgs{$proc} = $bpg2;
		}
	}
	
	my %names;
	map {$names{$_} = make_name($namesarr{$_});} @processes;
	
	my $pg = ProcessGraph->new();
	if($mergepairs==0)
	{
		$pg->{'Processes'} = \@processes;
		#$pg->{'ReacProds'} = \%reacprod;
		$pg->{'Names'} = \%names;
		$pg->{'Edges'} = \@edges2;
		if($embed) {$pg->{'Embed'} = \%bpgs;}
	}

	# mergepairs needs to be done correctly!
	my @procs = @processes;
	my @pairs;
	my @unpaired;
	if($mergepairs==1)
	{
		# build pairs;
		# get a process from the stack
		my @stack = @processes;
		
		while(@stack)
		{
			my $proc1 = shift @stack;
			my @stack2 = @stack;
			my @stack3;
			while(@stack2)
				{
				my $proc2 = shift @stack2;
				if(is_reverse_of($reac{$proc1},$prod{$proc1},$reac{$proc2},$prod{$proc2}))
					{
					push @pairs, $proc1." ".$proc2;
					#$pairs{$proc1} = $proc2;
					last;
					}
				else 
					{
					push @stack3,$proc2;
					}
				if(not @stack2) { push @unpaired,$proc1;}
				}
			@stack = (@stack2,@stack3);
		}
	}
	
	my @procs_p;
	my @edges_p;
	my %names_p;
	my %embed_p;
	my %remaphash;
	my %bpgs_p;
	foreach my $pair(@pairs)
	{
		my ($dom,$sub) = split(" ",$pair);
		my $proc = join(",",($dom,$sub));
		push @procs_p, $proc;
		$remaphash{$dom} = $proc;
		$remaphash{$sub} = $proc;
		
		my @name_arr = @{$namesarr{$dom}};
		push2ref($name_arr[0],$sub);
		my $name = make_name(\@name_arr);
		$names_p{$proc} = $name; 
		if($embed)
		{
			my @bpgs2 = map {$bpgs{$_} } ($dom,$sub);
			my $bpg = mergeNetworkGraphs(@bpgs2);
			$bpgs_p{$proc} = $bpg;
		}
	}
	foreach my $proc(@unpaired)
	{
		push @procs_p, $proc;
		$remaphash{$proc} = $proc;
		my $name = make_name($namesarr{$proc});
		$names_p{$proc} = $name;
		if($embed)
		{
			$bpgs_p{$proc} = $bpgs{$proc};
		}
	}
	@edges_p =uniq( map 
	{
		my @x = split(" ",$_);
		join(" ",map {$remaphash{$_}} @x);
	} @edges2);
	
	
	$pg->{'Processes'} = \@procs_p;
	#$pg->{'ReacProds'} = \%reacprod;
	$pg->{'Names'} = \%names_p;
	$pg->{'Edges'} = \@edges_p;
	if($embed) {$pg->{'Embed'} = \%bpgs_p;}
	return $pg;
	
}

sub is_reverse_of
{
	my @proc1_reac = sort {$a cmp $b} @{shift @_};
	my @proc1_prod = sort {$a cmp $b} @{shift @_};
	my @proc2_reac = sort {$a cmp $b} @{shift @_};
	my @proc2_prod = sort {$a cmp $b} @{shift @_};
	my $ret = 0;
	$ret = 1 if(scalar @proc1_reac and @proc1_reac~~@proc2_prod);
	$ret = 1 if(scalar @proc1_prod and @proc1_prod~~@proc2_reac);
	return $ret;
}

sub has_overlap
{
	my @x = @{shift @_};
	my @y = @{shift @_};
	my @z = grep {has(\@y,$_) } @x;
	return (scalar(@z) > 0) ? 1: 0;
}
sub getWCs
{
	my @aps = @{shift @_};
	my @wcs = @{shift @_};
	my @edges = @{shift @_};
	my @rets = ();
	foreach my $ap(@aps)
	{
		foreach my $wc(@wcs)
		{
			my $str = join(":",($wc,$ap,'Wildcard'));
			my @matches = grep {$_ eq $str} @edges;
			if(@matches) { push @rets,map {$_ =~ /^(.*):.*:.*/; $1; } @matches; }
		}
	}
	return uniq(@rets);
}

sub make_name
{
	my ($x,$y,$z) = @{shift @_};
	my @procs = @$x;
	my @reac = sort {$a cmp $b} @$y;
	my @prod = sort {$a cmp $b} @$z;
	
	my $str1 = join(",",@procs);
	my $str2 = join("+",@reac);
	my $str3 = join("+",@prod);
	
	my $arrow = (scalar @procs > 1) ? "<->" : "->";
	#return $str1."\n".$str2.$arrow.$str3;
	return $str2.$arrow.$str3;
}
1;