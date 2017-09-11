package ParameterPrior;

use strict;
use warnings;

use Class::Struct;


struct ParameterPrior => 
{
	name 		 => '$',
	type		 => '$',
	parameter1   => '$',
	parameter2   => '$',
};


# parse prior from BNGL string

sub readString
{
	shift @_;
	my @prior_string =  @_;
	my @prior_array = split ' ',$prior_string[0];
	my $prior = ParameterPrior->new();
	$prior->{name} = $prior_array[0];
	$prior->{type} = $prior_array[1];
	$prior->{parameter1} = $prior_array[2];
	$prior->{parameter2} = $prior_array[3];
	return $prior;
}

1;