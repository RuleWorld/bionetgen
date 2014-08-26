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
	my $mergepairs = shift @_;

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
	}
	
	my %names;
	map {$names{$_} = make_name($namesarr{$_});} @processes;
	
	if($mergepairs==0)
	{
		my $pg = ProcessGraph->new();
		$pg->{'Processes'} = \@processes;
		$pg->{'ReacProds'} = \%reacprod;
		$pg->{'Names'} = \%names;
		$pg->{'Edges'} = \@edges2;
		return $pg;
	}

	# computing pairs
	my @pairs;
	my @stack1 = @processes;
	while(@stack1)
	{
		my $proc1 = shift @stack1;
		my @reac1 = @{$reac{$proc1}};
		my @prod1 = @{$prod{$proc1}};
		my @stack2 = ();
		while(@stack1)
		{
			my $proc2 = shift @stack1;
			my @reac2 = @{$reac{$proc2}};
			my @prod2 = @{$prod{$proc2}};
			my $str = '';
			if(@reac1)
				{ if(@reac1~~@prod2) {$str = $proc1." ".$proc2;}  }
			if(@prod1)
				{ if(@reac2~~@prod1) {$str = $proc1." ".$proc2;}  } 				
			if($str ne '')
				{ push @pairs,$str;}
			else
				{ push @stack2, $proc2; }
		}
		@stack1 = @stack2;
	}
	
	#print map $_."\n",@pairs;
	my %pairhash; # maps each to its dominant partner;
	my %revpairhash;
	map {my @x = split(" ",$_); $pairhash{$x[1]} = $x[0]; $revpairhash{$x[0]} = $x[1];} @pairs;
	my @dompair = uniq(values %pairhash);
	my @subpair = uniq(keys %pairhash);
	my @processes2 = grep {has(\@subpair,$_)==0} @processes;
	my %reacprod2;
	map {$reacprod2{$_} = $reacprod{$_};} @processes2;
	my %names2;
	map { 	my $proc = $_; 
			my @namearr = @{$namesarr{$_}};
			push2ref($namearr[0],$revpairhash{$proc}) if has(\@dompair,$proc);
			$names2{$_} = make_name(\@namearr); } @processes2;
	my @edges3;
	foreach my $edge(@edges2)
	{
		my @x = split(" ",$edge);
		my @z;
		foreach my $y(@x)
			{
			if(has(\@subpair,$y)) { push @z,$pairhash{$y};}
			else { push @z,$y;}
			}
		push @edges3,join(" ",@z);
	}
	@edges3 = uniq(@edges3);
	
	my $pg = ProcessGraph->new();
	$pg->{'Processes'} = \@processes2;
	$pg->{'ReacProds'} = \%reacprod2;
	$pg->{'Names'} = \%names2;
	$pg->{'Edges'} = \@edges3;
	return $pg;
	
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
	my @reac = @$y;
	my @prod = @$z;
	
	my $str1 = join(",",@procs);
	my $str2 = join("+",@reac);
	my $str3 = join("+",@prod);
	
	my $arrow = (scalar @procs > 1) ? "<->" : "->";
	return $str1."\n".$str2.$arrow.$str3;

}
1;