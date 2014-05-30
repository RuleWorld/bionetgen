#

# pragmas
use strict;
use warnings;
no warnings 'redefine';


my $dirpath = shift @ARGV;
opendir ( my $dir, $dirpath ) or die "Cannot open\n";

my @files = grep(/bngl/,readdir $dir);
my $cmd = "perl visualize.pl";



foreach my $opt(@ARGV)
{
foreach my $file(@files)
	{
	system(join(" ",($cmd,$dirpath.$file,$opt,$opt)));
	}
}
