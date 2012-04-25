#!/usr/bin/perl

@files= <*.bngl>;

$flist = join(' ',@files);
$command = "../BNG2.pl -log";
print "$command $flist\n";
system "$command $flist";
