use strict;
use warnings;

my $linein = "action simulate";

my ($action, $options);
if ( $linein =~ s/^action\s+// ) {
    if ( $linein =~ /^\s*(\w+)\s*\((.*)\);?\s*$/ )
    {   # syntax:  "action(options)"
        $action = $1;
        $options = $2;
    }
    elsif ( $linein =~ /^\s*([A-Za-z_]\w*)\s*(?:\(\s*(.*)\s*\))?\s*$/ )
    {
        $action = $1;
        $options = defined $2 ? $2 : "";
    }
    print "action=$action, options=$options\n";
}
