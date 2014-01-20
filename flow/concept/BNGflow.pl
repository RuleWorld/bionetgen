#!/usr/bin/perl

use strict;
use warnings;
my $file = shift @ARGV;
my $term = "output";
open(my $filehandle,"<", $file) or die "Problem opening file";
while ( my $line = <$filehandle> )
{
    if ( $line =~ /^begin organ/ )
    {
        my ($organ_name) = $line =~ /^begin organ \"(.+)\"/;
        print "#organ name = $organ_name\n";
        while (my $line = <$filehandle> )
        
        {
        	
            last if $line =~ /^end organ/;
            my ($key,$val) = $line =~ /\s*(\w+)\s+\"(.+)\"/;
            if ($key eq "file")
            {
                open(my $modelfile,"<", $val); 
                while(my $model_line = <$modelfile>)
               
				{print $model_line;}	 
				close $modelfile;}
    

				if ( $line =~ /^    output/ )
				{
					my ($flow) = $line =~ /^    output \"(.+)\"/;
					my ($rate) = $line =~ /  \"(.+)\"/;
					print "A\@Vascular_$organ_name -> A\@Vascular_$flow \n"
	

				}
            
            if ( $line =~ /^    exchange/ )
				{
					my ($exchange_rate) = $line =~ /^    exchange \"(.+)\"/;
				
					print "A\@Vascular_$organ_name <-> A\@Extravascular_$organ_name $exchange_rate \n"
	

				
            }
            }
            print "end reaction rules\nend model\n";
}
 
}

