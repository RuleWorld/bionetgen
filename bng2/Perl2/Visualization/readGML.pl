#!/usr/bin/perl
use warnings;
use Getopt::Long;
# USAGE
# $parsed = readGML::parseGML($filename);
# @$parsed;
# print recursivelyPrintList($parsed);


#my @list = ("node","bla","node","[", "id", "bla2", "id2", "bla3", "]", "edge","bla3");


sub parseGML
{
	my $filename = shift @_;
	open(my $fh, "<", $filename) 
		or die "cannot open < ${filename}: $!";
	my @lines = <$fh>;
	close $fh;
	my @list = getTokens(\@lines);
	my @parsed = parseList(\@list);
	#my $str = recursivelyPrintList(\@parsed);
	#print $str;;
	return \@parsed;
	# returns a reference to a list of pairs with recursive nesting.
}

sub getTokens
{
	my @lines = @{shift @_};
	my $line = join("",@lines);
	$line =~ s/\n/ /g;
	$line =~ s/\s+/ /g;
	return split(/\s/,$line);
}

sub parseList
{
	my @tokenlist = @{shift @_};
	my @outlist;
	while(@tokenlist)
	{
		my $key = shift @tokenlist;
		#print "key = ".$key."\n";
		my $token = shift @tokenlist;
		my $value = $token;
		#print "token = ".$token."\n";
		if($token eq "[")
			{
				my $i = 1;
				my @newtokenlist;
				while($i>0)
				{
					my $tok = shift @tokenlist;
					$i++ if ($tok eq "[");
					$i-- if ($tok eq "]");
					if($i==0) 
					{
						#print "New token list = ".join(",",@newtokenlist)."\n";
						my $out = parseList(\@newtokenlist);
						$value = $out;
					}
					else
					{
						push @newtokenlist,$tok;
					}
				}
			}
		#print "Value = ".$value."\n";
		my @pair = ($key,$value);
		push @outlist, \@pair;	
	}
	return \@outlist;
}

sub recursivelyPrintList
{
	my $var = shift @_;
	if (ref($var) ne "ARRAY") {return $var;}
	if (ref($var) eq "ARRAY") {
		#my @arr = map {recursivelyPrintList($_)} @$var;
		return "(".join(",",map {recursivelyPrintList($_)} @$var).")";
	}
	return;
}