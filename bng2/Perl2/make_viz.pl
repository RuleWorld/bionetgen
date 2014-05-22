#

# pragmas
use strict;
use warnings;
no warnings 'redefine';


my $dirpath = $ARGV[0];
opendir ( my $dir, $dirpath ) or die "Cannot open\n";

my @files = grep(/bngl/,readdir $dir);
my $cmd = "perl visualize.pl";
foreach my $file(@files)
{
#system(join(" ",($cmd,$dirpath.$file,'rules','rules')));
system(join(" ",($cmd,$dirpath.$file,'contact','contact')));
system(join(" ",($cmd,$dirpath.$file,'regulatory','regulatory')));
system(join(" ",($cmd,$dirpath.$file,'process','process')));
}

