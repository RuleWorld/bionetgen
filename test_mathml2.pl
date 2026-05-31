use strict;
use warnings;
use FindBin;
use lib "$FindBin::Bin/bng2/Perl2";
use Expression;

my $expr = Expression->new(Type => '~=');
$expr->Arglist([Expression->new(Type => 'VAR', Arglist => ['x']), Expression->new(Type => 'NUM', Arglist => ['5'])]);

print $expr->toMathMLString();
