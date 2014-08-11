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
	my @edges = @{$bpg->{'EdgeList'}};
	my @edges2;
	
	my @processes = grep {$bpg->{'NodeType'}->{$_} =~ /Rule/} @{$bpg->{'NodeList'}};
	my @wcs = uniq(map {$_ =~ /^(.*):.*:.*/; $1; } grep { $_ =~ /Wildcard/ } @edges);
	
	my %reacprod;
	my %context;
	map { my @x = (); $reacprod{$_} = \@x; } @processes;
	map { my @x = (); $context{$_} = \@x; } @processes;
	
	foreach my $pr(@processes)
	{
		my $r = quotemeta $pr;
		my @rps = 	uniq( map { $_ =~ /.*:(.*):.*/;  $1; }
					grep { $_ =~ /Reactant|Product/ }
					grep {$_ =~ /^$r:/ } @edges );
		if(@wcs) { push @rps, getWCs(\@rps,\@wcs,\@edges); }
		$reacprod{$pr} = \@rps;
		
		my @cont = 	uniq( map { $_ =~ /.*:(.*):.*/;  $1; }
					grep { $_ =~ /Context/ }
					grep {$_ =~ /^$r:/ } @edges );
		$context{$pr} = \@cont;
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
	my %names;
	map { $names{$_} = makeName($reacprod{$_}) } @processes;
	
	my $pg = ProcessGraph->new();
	$pg->{'Processes'} = \@processes;
	$pg->{'ReacProds'} = \%reacprod;
	$pg->{'Names'} = \%names;
	$pg->{'Edges'} = \@edges2;
	
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

sub makeName
{
	my @reacprods = sort {$a cmp $b} @{shift @_};
	return join(' / ', @reacprods);
}
1;