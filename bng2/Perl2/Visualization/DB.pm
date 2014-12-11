package Viz;

use strict;
use warnings;
no warnings 'redefine';

use Class::Struct;
use Visualization::NetworkGraph;
use Visualization::ProcessGraph;
use DBI;

	
sub newDB
{

	my $dbh = DBI->connect(          
		"dbi:SQLite:dbname=:memory:", 
		"",                          
		"",                          
		{ RaiseError => 1 },         
	) or die $DBI::errstr;
	return $dbh;
}

sub newDBFile
{
	my $name = shift;
	my $dbh = DBI->connect(          
		"dbi:SQLite:dbname=$name", 
		"",                          
		"",                          
		{ RaiseError => 1 },         
	) or die $DBI::errstr;
	return $dbh;
}

sub getBPG
{
	my $dbh = shift @_;
	my $background = shift @_;
	my $groups = shift @_;
	my $collapse = shift @_;
	
	my @nodelist;
	my @edgelist;
	my %nodetype;
	my %nodeclass;
	my $sth;
	my $str;
	
	# nodes
	my @ap;
	my @rules;
	
	
}

sub copyDB
{
	my $source = shift;
	my $target = shift;
	

}


sub getRegDB
{
	my $dbh = shift @_;
	my $bpg = shift @_;
	
	my @nodelist = @{$bpg->{'NodeList'}};
	my @edgelist = @{$bpg->{'EdgeList'}};
	my %nodetype = %{$bpg->{'NodeType'}};

	my @ap = grep {$nodetype{$_} eq 'AtomicPattern'} @nodelist;
	$dbh = getAtomicPatternsDB($dbh,\@ap);
	$dbh = getEdgesDB($dbh,\@edgelist);
	return $dbh;
}

sub getAtomicPatternsDB
	{
	my $dbh = shift @_;
	my @aps = @{shift @_};
	#my %nodetype = %{shift @_};
	
	$dbh->do("DROP TABLE IF EXISTS AtomicPatterns;");
	$dbh->do("CREATE TABLE AtomicPatterns(Name TEXT, Type TEXT, Class TEXT, Background INTEGER);"); 
	
	foreach my $ap(@aps)
	{

		my $type = getAPType($ap);
		my $alias = ($type eq 'Bond')  ? getBondAlias($ap) : "NULL";
		my ($mol1,$mol2,$comp1,$comp2,$is) = ("NULL") x 5;
		my @x1 = map {$_ eq "NULL" ? "NULL" : quote($_);} ($ap,$type);
		push @x1, default_class($ap) , default_bkg($type);
		my $str = "INSERT INTO AtomicPatterns VALUES(".join(",",@x1).")";
		$dbh->do($str);
	}
	return $dbh;
}

sub getRuleDB
{
	my $dbh = shift @_;
	my @rrs = @{shift @_};
	$dbh->do("DROP TABLE IF EXISTS Rules");
	$dbh->do("CREATE TABLE Rules(Name TEXT, Type TEXT, String TEXT, Class TEXT, Background INTEGER)"); 
	foreach my $rr(@rrs)
	{
		my @types = ("Forward","Reverse");
		my @bkg = ("0","1");
		foreach my $i(0..@$rr-1)
		{
			my $name = $$rr[$i]->Name;
			my $type = $types[$i];
			my $string = $$rr[$i]->toString();
			my @x1 = map quote($_), ($name, $type,$string);
			push @x1, quote(), $bkg[$i];
			my $str = "INSERT INTO Rules VALUES(".join(",",@x1).")";
			$dbh->do($str);
		}
	}
	return $dbh;
}

sub getEdgesDB
{
	my $dbh = shift @_;
	my @edges = @{shift @_};
	
	$dbh->do("DROP TABLE IF EXISTS Edges");
	$dbh->do("CREATE TABLE Edges(Rule TEXT, AtomicPattern TEXT, Type TEXT)"); 
	
	$dbh->do("DROP TABLE IF EXISTS Wildcards");
	$dbh->do("CREATE TABLE Wildcards(Wildcard TEXT, AtomicPattern TEXT)"); 
	
	foreach my $edge(@edges)
	{
		my @splits = split(":",$edge);
		my $type = $splits[2];
		if(has(['Reactant','Product','Context'],$type))
		{
			my @x1 = map quote($_), @splits;
			my $str = "INSERT INTO Edges VALUES(".join(",",@x1).")";
			$dbh->do($str);
		}
		elsif($type eq 'Wildcard')
		{
			pop @splits;
			my @x1 = map quote($_), @splits;
			my $str = "INSERT INTO Wildcards VALUES(".join(",",@x1).")";
			$dbh->do($str);
		}
	}
	return $dbh;
}


sub getAPType
	{
		my $str = shift;
		if($str =~ /\!\+/) {return "BondWildcard";}
		if($str =~ /\!/) {return "Bond";}
		if($str =~ /\~/) {return "InternalState";}
		if($str =~ /\(.*\)/) {return "BindingSite";}
		return "Molecule";
	}
sub quote
	{
		return @_? "'".$_[0]."'" : "NULL";
	}
sub default_bkg
	{
		return ($_[0] eq 'BindingSite') ? "1" : "0";
	}
sub default_class
	{
		my $x = shift;
		return quote() if ($x !~ /\./);
		my $y = join "_", map getMolecule($_), split (/\./,$x);
		return quote($y);
	}
	
sub getBondAlias
	{
		return join(".", reverse split (/\./,$_[0]));
	}
sub getMolecule 
	{ 
	my $x = shift; $x =~ /(.*)\(.*\)/; return $1;
	}
sub getComponent 
	{ 
	my $x = shift; 
	if($x =~ /\!/) { $x =~ /.*\((.*)!.*\)/; return $1;};
	if($x =~ /~/) { $x =~ /.*\((.*)~.*\)/; return $1;}
	$x =~ /.*\((.*)\)/; return $1;
	}
sub getInternalState
	{
	my $x = shift; $x =~ /.*\(.*~(.*)\)/; return $1;
	}
1;