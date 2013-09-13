#!/usr/bin/perl

my $file = shift @ARGV;
open(my $filehandle,"<", $file) or die "Problem opening file";
while ( my $line = <$filehandle> )
{
    if ( $line =~ /^begin organ/ )
    {
        my ($organ_name) = $line =~ /^begin organ \"(.+)\"/;
        print "organ name = $organ_name\n";
        while (my $line = <$filehandle> )
        {
            last if $line =~ /^end organ/;
            my ($key,$val) = $line =~ /\s*(\w+)\s+\"(.+)\"/;
            if ($key eq "file")
            {
                open(my $modelfile,"<", $val);
                while ( my $model_line = <$modelfile>)
                {
                    print $model_line;
                }
                close $modelfile;
            }
        }
    }
}


