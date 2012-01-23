#!/usr/bin/perl

use strict;
use warnings;


my $dir = ".";
opendir( my $dh, $dir )  or  die "cannot open directory $dir: $!";

while ( my $file = readdir $dh )
{
    next unless ( $file =~ /\.cdat$/ );
    print "$file\n";
    my $old_file = $file . ".old";
    my @args = ("cp", $file, $old_file);
    system(@args)==0  or  die "systems @args failed: $?";
    
    @args = ("./reformat.pl < $old_file > $file" );
    system(@args)==0  or  die "systems @args failed: $?";    
}

    
    

