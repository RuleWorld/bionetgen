#!/usr/bin/perl

my @lines = ();
while ( my $line = <STDIN> )
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
    push @lines, $line;
}


print STDOUT join("\n", @lines);
