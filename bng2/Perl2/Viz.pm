package Viz;

use strict;
use warnings;
no warnings 'redefine';

use Class::Struct;

struct VizArgs => 
{ 
	'Model' => '$', # array of strings
	'ContactMap'=> '$', # true or false
	'Rules'=> '$', # true or false
	'BaseName'=> '$',
	'Suffix'=>'$',
	'RRules'=>'@',
	'RuleNames'=>'@',
	
};



sub uniq (@) { my %seen = (); grep { not $seen{$_}++ } @_; }
sub flat(@) { map @$_, @_; }


sub execute_params
{
	my $model = shift @_;
	my $args = shift @_;

}
1;
