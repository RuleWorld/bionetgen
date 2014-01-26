#!/usr/bin/perl

use strict;
use warnings;
my $input_file = shift @ARGV;
my $output_file;
my @compartments;

open(my $filehandle,"<", $input_file) or die "Problem opening file";
while ( my $line = <$filehandle> )
{
        if ( $line =~ /^\s*modelname/ )
        {
                my ($model_name) = $line =~ /^\s*modelname\s+\"(.+)\"/;
                open($output_file,">",$model_name.".bngl") or die "Problem open output file";
        }
    if ( $line =~ /^begin organ/ )
    {
        my ($organ_name) = $line =~ /^begin organ \"(.+)\"/;
        print "#organ name = $organ_name\n";        
        print $output_file "$organ_name\n";
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

            }
}
 
}