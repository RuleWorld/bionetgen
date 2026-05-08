use strict;
use warnings;

my $linein = "action simulate({t_end=>10})";

my ($action, $options);
if ( $linein =~ s/^action\s+// ) {
    if ( $linein =~ /^\s*(\w+)\s*\((.*)\);?\s*$/ )
    {   # syntax:  "action(options)"
        $action = $1;
        $options = $2;
    }
    elsif ( $linein =~ /^\s*(\w+)\s*$/ )
    {
        $action = $1;
        $options = "";
    }
    print "action=$action, options=$options\n";
}
