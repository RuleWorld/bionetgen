#!/usr/bin/perl

use strict;
use warnings;
use File::Copy;

my $dir = ".";
opendir( my $dh, $dir )  or  die "cannot open directory $dir: $!";

while ( my $file = readdir $dh )
{
    next unless ( $file =~ /\.cdat$/ );
    print "$file\n";
    my $old_file = $file . ".old";
    copy($file, $old_file) or die "Copy failed: $!";

    open(my $in, '<', $old_file) or die "Cannot open $old_file: $!";
    open(my $out, '>', $file) or die "Cannot open $file: $!";

    while ( my $line = <$in> )
    {
        chomp $line;
        if ($line =~ /^#/)
        {
            $line =~ s/^#\s+//;
            my @vals = split /\s+/, $line;
            foreach my $val (@vals)
            {
                $val = sprintf "%19s", $val;
            }
            $line = join ' ', @vals;
            $line =~ s/^\s/#/;
        }
        else
        {
            $line =~ s/^\s+//;
            my @vals = split /\s+/, $line;
            foreach my $val (@vals)
            {
                $val = sprintf "%19.12e", $val;
            }
            $line = join ' ', @vals;
        }
        print $out "$line\n";
    }
    
    close($in);
    close($out);
}

    
    

