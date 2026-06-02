use strict;
use warnings;
use Test::More;
use FindBin;
use lib "$FindBin::Bin/../bng2/Perl2";
use BNGModel;

# Mock the writeFile method in BNGModel
{
    no warnings 'redefine';
    our $writeFile_args;
    *BNGModel::writeFile = sub {
        my $self = shift;
        $writeFile_args = shift;
        return "mocked_return";
    };
}

my $model = bless {}, 'BNGModel';

subtest 'Default parameters' => sub {
    our $writeFile_args;
    $writeFile_args = undef;

    my $ret = $model->writeNET();

    is($ret, "mocked_return", "Returns result of writeFile");
    is_deeply($writeFile_args, {
        'evaluate_expressions' => 1,
        'format'               => 'net',
        'include_model'        => 1,
        'include_network'      => 1,
        'overwrite'            => 1,
        'pretty_formatting'    => 0,
        'TextReaction'         => 0,
        'TextSpecies'          => 1,
    }, "Passes default parameters to writeFile");
};

subtest 'Custom parameters' => sub {
    our $writeFile_args;
    $writeFile_args = undef;

    my $ret = $model->writeNET({
        'format' => 'other',
        'custom_param' => 'custom_val',
        'pretty_formatting' => 1,
    });

    is($ret, "mocked_return", "Returns result of writeFile");
    is_deeply($writeFile_args, {
        'evaluate_expressions' => 1,
        'format'               => 'other',
        'include_model'        => 1,
        'include_network'      => 1,
        'overwrite'            => 1,
        'pretty_formatting'    => 1,
        'TextReaction'         => 0,
        'TextSpecies'          => 1,
        'custom_param'         => 'custom_val',
    }, "Merges custom parameters correctly");
};

done_testing();
