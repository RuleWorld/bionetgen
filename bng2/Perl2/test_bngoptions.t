#!/usr/bin/perl

use strict;
use warnings;
use FindBin;
use lib "$FindBin::Bin";
use Test::More tests => 2;

# Define a mock BNGMessages before loading BNGOptions
BEGIN {
    $INC{'BNGMessages.pm'} = 1;
    package BNGMessages;
    use Exporter 'import';
    our @EXPORT_OK = qw(error warning message);
    sub error {
        my $msg = shift;
        die "MOCKED_ERROR: $msg";
    }
    sub warning {}
    sub message {}
    $INC{'BNGMessages.pm'} = 1;
}

use BNGOptions;

# We want to test that a missing module triggers the error message correctly.
# The `GetBNGOptions` method overrides `$self->config->{ModelID}` with either
# `BNGModels::MacroBNGModel` or `BNGModels::MicroBNGModel`, and then tries to load it.
# To ensure it always fails for the purpose of this test, we can mock `require` or
# temporarily remove `BNGModels/MicroBNGModel.pm` from @INC, or mock `eval` indirectly.
# However, the simplest way is to override `require` using CORE::GLOBAL::require, but
# that can be complex.
# Another way is to push a custom coderef into @INC that throws an error for `BNGModels/MicroBNGModel.pm`.

sub test_missing_module {
    my $opts = BNGOptions->new();

    # Inject a hook into @INC that guarantees failure when require attempts to load the module
    unshift @INC, sub {
        my ($coderef, $filename) = @_;
        if ($filename eq 'BNGModels/MicroBNGModel.pm') {
            die "Simulated load failure for $filename\n";
        }
        return undef; # Let the normal require search continue for other modules
    };

    my $error_caught = 0;
    my $error_msg = "";
    eval {
        $opts->GetBNGOptions();
    };
    if ($@) {
        $error_caught = 1;
        $error_msg = $@;
    }

    # Remove our hook from @INC
    shift @INC;

    ok($error_caught, "Caught error from GetBNGOptions");
    like($error_msg, qr/Module BNGModels::MicroBNGModel not loaded because/, "Error message correctly interpolates ModelID");
}

test_missing_module();
