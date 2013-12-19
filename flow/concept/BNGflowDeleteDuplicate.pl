#!/usr/bin/perl

use strict;
use warnings;

my @array = '/Users/cmcarcil/git/bionetgen/flow/concept/BNGflowSortOutput.bngl';
{
	local @ARGV = (@array);
##delets repeats

my %seen = ();
{
   local @ARGV = (@array);
   while(<>){
      $seen{$_}++;
      next if $seen{$_} > 1;
      print;
  }
}
print "end model";
}