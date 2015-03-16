use strict; 
use warnings;

use YAML::Tiny;

# Open the config
my $yaml = YAML::Tiny->read( 'sample_annotations.yml' );


for my $key ( keys %{ $yaml->[0] } ){
  my $value = $yaml->[0]{$key};

  print "$key => $value\n";
  for my $key2 ( keys %{ $yaml->[0]{$key} } ){
  my $value2 = $yaml->[0]{$key}{$key2};

  print "\t$key2 => $value2\n";

  }
}