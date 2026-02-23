#!/usr/bin/perl
# Verification script for issue #281 fix
# This script demonstrates the bug and verifies the fix

use strict;
use warnings;

print "Testing regex substitution behavior for issue #281\n";
print "=" x 60 . "\n\n";

# Test strings with multiple occurrences
my @test_cases = (
    {
        name => "Multiple < operators",
        input => "if((A<B)&&(B<C)&&(C<D), 1, 0)",
        expected_lt => 3,  # Should have 3 &lt;
        expected_and => 2, # Should have 2 'and'
    },
    {
        name => "Multiple > operators",
        input => "if((x>0)&&(y>0)&&(z>0), 1, 0)",
        expected_gt => 3,  # Should have 3 &gt;
        expected_and => 2, # Should have 2 'and'
    },
    {
        name => "Multiple || operators",
        input => "if((x>0)||(y>0)||(z>0), 1, 0)",
        expected_gt => 3,  # Should have 3 &gt;
        expected_or => 2,  # Should have 2 'or'
    },
);

foreach my $test (@test_cases) {
    print "Test: $test->{name}\n";
    print "Input: $test->{input}\n";

    # Test WITHOUT /g (buggy version)
    my $buggy = $test->{input};
    $buggy =~ s/</&lt\;/;
    $buggy =~ s/>/&gt\;/;
    $buggy =~ s/&&/and/;
    $buggy =~ s/\|\|/or/;

    print "BUGGY (no /g): $buggy\n";

    # Count replacements in buggy version
    my $buggy_lt_count = () = $buggy =~ /&lt;/g;
    my $buggy_gt_count = () = $buggy =~ /&gt;/g;
    my $buggy_and_count = () = $buggy =~ /\band\b/g;
    my $buggy_or_count = () = $buggy =~ /\bor\b/g;

    # Test WITH /g (fixed version)
    my $fixed = $test->{input};
    $fixed =~ s/</&lt\;/g;
    $fixed =~ s/>/&gt\;/g;
    $fixed =~ s/&&/and/g;
    $fixed =~ s/\|\|/or/g;

    print "FIXED (with /g): $fixed\n";

    # Count replacements in fixed version
    my $fixed_lt_count = () = $fixed =~ /&lt;/g;
    my $fixed_gt_count = () = $fixed =~ /&gt;/g;
    my $fixed_and_count = () = $fixed =~ /\band\b/g;
    my $fixed_or_count = () = $fixed =~ /\bor\b/g;

    # Verify expectations
    my $pass = 1;
    if (exists $test->{expected_lt}) {
        if ($buggy_lt_count != 1 && $buggy_lt_count != 0) {
            print "  ERROR: Buggy version has unexpected &lt; count\n";
            $pass = 0;
        }
        if ($fixed_lt_count != $test->{expected_lt}) {
            print "  ERROR: Fixed version &lt; count ($fixed_lt_count) != expected ($test->{expected_lt})\n";
            $pass = 0;
        }
    }
    if (exists $test->{expected_gt}) {
        if ($buggy_gt_count != 1 && $buggy_gt_count != 0) {
            print "  ERROR: Buggy version has unexpected &gt; count\n";
            $pass = 0;
        }
        if ($fixed_gt_count != $test->{expected_gt}) {
            print "  ERROR: Fixed version &gt; count ($fixed_gt_count) != expected ($test->{expected_gt})\n";
            $pass = 0;
        }
    }
    if (exists $test->{expected_and}) {
        if ($buggy_and_count != 1 && $buggy_and_count != 0) {
            print "  ERROR: Buggy version has unexpected 'and' count\n";
            $pass = 0;
        }
        if ($fixed_and_count != $test->{expected_and}) {
            print "  ERROR: Fixed version 'and' count ($fixed_and_count) != expected ($test->{expected_and})\n";
            $pass = 0;
        }
    }
    if (exists $test->{expected_or}) {
        if ($buggy_or_count != 1 && $buggy_or_count != 0) {
            print "  ERROR: Buggy version has unexpected 'or' count\n";
            $pass = 0;
        }
        if ($fixed_or_count != $test->{expected_or}) {
            print "  ERROR: Fixed version 'or' count ($fixed_or_count) != expected ($test->{expected_or})\n";
            $pass = 0;
        }
    }

    print "  Result: " . ($pass ? "PASS" : "FAIL") . "\n";
    print "\n";
}

print "Verification complete!\n";
