#!/usr/bin/perl

use strict;
use warnings;

my @array = '/Users/cmcarcil/git/bionetgen/flow/concept/output.bngl';
##combines groups

my %a = ();
{
	local @ARGV = (@array);
print "begin model\n";
while (<>) {	
  if (/begin model/../end parameters/) {
    next if /begin model/ || /end parameters/;
    print;}
	}
}
print "end parameters\n\n";
my %b = ();
{
	local @ARGV = (@array);
	print "begin compartments\n\n";
while (<>) {
  if (/begin compartments/../end compartments/) {
    next if /begin compartments/ || /end compartments/;
    print;}

  }
}
print "end compartments\n\n";
my %c = ();
{
	local @ARGV = (@array);
	print "begin molecule types\n\n";
while (<>) {
if (/begin molecule types/../end molecule types/) {
  next if /begin molecule types/ || /end molecule types/;
  print;}

  }
}
	print "end molecule types\n\n";
my %d = ();
{
	local @ARGV = (@array);
print "begin seed species\n\n";
while (<>) {
if (/begin seed species/../end seed species/) {
  next if /begin seed species/ || /end seed species/;
  print;}

  }
}
print "end seed species\n\n";
my %e = ();
{
	local @ARGV = (@array);
print "begin observables\n\n";
while (<>) {
if (/begin observables/../end observables/) {
  next if /begin observables/ || /end observables/;
  print;}

  }
}
print "end observables\n\n";

my %f = ();
{
	local @ARGV = (@array);
	print "begin functions\n\n";
while (<>) {
if (/begin functions/../end functions/) {
  next if /begin functions/ || /end functions/;
  print;}

  }
}
	print "end functions\n\n";
my %g = ();
{
	local @ARGV = (@array);
	print "begin reaction rules\n\n";
while (<>) {
if (/begin reaction rules/../end model/) {
  next if /begin reaction rules/ || /end model/;
  print;}

  }
}


