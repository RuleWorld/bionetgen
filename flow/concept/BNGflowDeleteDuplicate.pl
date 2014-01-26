#!/usr/bin/perl

use strict;
use warnings;

my @array = '/Users/cmcarcil/git/bionetgen/flow/concept/BNGflowSortOutput.bngl';
{
        local @ARGV = (@array);
##delets repeats

print "\n";
my %duplicate = ();
{
   local @ARGV = (@array);
   while(<>){
      $duplicate{$_}++;
      next if $duplicate{$_} > 1;
      print;
  }
}
print "end model";
}
