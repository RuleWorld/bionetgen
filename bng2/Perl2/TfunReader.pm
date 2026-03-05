package TfunReader;

# Module for reading and validating TFUN data files
# Supports the new .tfun format with headers

use strict;
use warnings;

# Read a .tfun file and return x and y value arrays
# Arguments: filename, function_name, index_name
# Returns: (\@x_vals, \@y_vals) or dies on error
sub read_tfun_file {
    my ($filename, $func_name, $index_name) = @_;

    my @x_vals;
    my @y_vals;

    # Open file
    open(my $fh, '<', $filename)
        or die "ERROR: Cannot open tfun file '$filename': $!\n";

    # Read header line
    my $header = <$fh>;
    unless ($header) {
        close($fh);
        die "ERROR: tfun file '$filename' is empty\n";
    }

    # Remove leading # and whitespace
    $header =~ s/^#\s*//;
    chomp($header);

    # Split into column names
    my @cols = split(/\s+/, $header);

    # Validate header
    validate_header(\@cols, $func_name, $index_name, $filename);

    # Read data lines
    my $line_num = 1;
    while (my $line = <$fh>) {
        $line_num++;

        # Skip comments and blank lines
        next if $line =~ /^\s*#/;
        next if $line =~ /^\s*$/;

        # Parse data
        chomp($line);
        my @values = split(/\s+/, $line);

        # Should have exactly 2 values
        unless (scalar(@values) == 2) {
            close($fh);
            die "ERROR: Line $line_num in '$filename' has " . scalar(@values) .
                " values, expected 2\n";
        }

        # Add to arrays
        push @x_vals, $values[0];
        push @y_vals, $values[1];
    }

    close($fh);

    # Validate data
    validate_data(\@x_vals, \@y_vals, $filename);

    return (\@x_vals, \@y_vals);
}

# Validate that the header matches the function and index names
sub validate_header {
    my ($cols, $func_name, $index_name, $filename) = @_;

    # Must have exactly 2 columns
    unless (scalar(@$cols) == 2) {
        die "ERROR: tfun file '$filename' must have exactly 2 columns in header, " .
            "found " . scalar(@$cols) . "\n";
    }

    # Check index name matches (column 1)
    my $idx_col = $cols->[0];
    $idx_col =~ s/\(\)$//;  # Strip () if present

    # Allow 'time' or 't' to match 'time' index
    my $idx_match = ($idx_col eq $index_name) ||
                    (($index_name =~ /^(time|t)$/i) && ($idx_col =~ /^(time|t)$/i));

    unless ($idx_match) {
        die "ERROR: tfun file '$filename' column 1 header '$idx_col' " .
            "doesn't match index name '$index_name'\n";
    }

    # Check function name matches (column 2)
    my $val_col = $cols->[1];
    $val_col =~ s/\(\)$//;  # Strip () if present

    unless ($val_col eq $func_name) {
        die "ERROR: tfun file '$filename' column 2 header '$val_col' " .
            "doesn't match function name '$func_name'\n";
    }
}

# Validate the data arrays
sub validate_data {
    my ($x_vals, $y_vals, $filename) = @_;

    # Must have at least 2 data points
    unless (scalar(@$x_vals) >= 2) {
        die "ERROR: tfun file '$filename' must have at least 2 data points, " .
            "found " . scalar(@$x_vals) . "\n";
    }

    # Arrays must have same length
    unless (scalar(@$x_vals) == scalar(@$y_vals)) {
        die "ERROR: tfun file '$filename' has mismatched array lengths: " .
            scalar(@$x_vals) . " x values, " . scalar(@$y_vals) . " y values\n";
    }

    # Check that x values are strictly increasing (monotonic)
    for (my $i = 0; $i < $#{$x_vals}; $i++) {
        unless ($x_vals->[$i] < $x_vals->[$i+1]) {
            die "ERROR: tfun file '$filename' x values must be strictly increasing\n" .
                "       At line " . ($i+2) . ": $x_vals->[$i] >= $x_vals->[$i+1]\n";
        }
    }
}

# Validate inline tfun data (arrays passed directly)
sub validate_inline_data {
    my ($x_vals, $y_vals) = @_;

    # Must have at least 2 data points
    unless (scalar(@$x_vals) >= 2) {
        die "ERROR: tfun() inline data must have at least 2 data points, " .
            "found " . scalar(@$x_vals) . "\n";
    }

    # Arrays must have same length
    unless (scalar(@$x_vals) == scalar(@$y_vals)) {
        die "ERROR: tfun() inline data has mismatched array lengths: " .
            scalar(@$x_vals) . " x values, " . scalar(@$y_vals) . " y values\n";
    }

    # Check that x values are strictly increasing (monotonic)
    for (my $i = 0; $i < $#{$x_vals}; $i++) {
        unless ($x_vals->[$i] < $x_vals->[$i+1]) {
            die "ERROR: tfun() inline data x values must be strictly increasing\n" .
                "       At index $i: $x_vals->[$i] >= $x_vals->[$i+1]\n";
        }
    }

    # Warn if dataset is very large for inline format
    if (scalar(@$x_vals) > 100) {
        print "WARNING: tfun() has " . scalar(@$x_vals) . " data points. " .
              "Consider using file-based format for large datasets:\n" .
              "         tfun('data.tfun', index)\n";
    }
}

1;  # Return true for module

__END__

=head1 NAME

TfunReader - Read and validate table function (tfun) data files

=head1 SYNOPSIS

    use TfunReader;

    # Read file-based tfun
    my ($x_vals, $y_vals) = TfunReader::read_tfun_file(
        'dosing.tfun', 'drug_dose', 'time'
    );

    # Validate inline tfun
    TfunReader::validate_inline_data(\@x_vals, \@y_vals);

=head1 DESCRIPTION

TfunReader provides functions to read and validate table function data
for use in BioNetGen models. It supports the .tfun file format with
headers and validates data consistency.

=head1 FILE FORMAT

TFUN files are plain text with a header line:

    # time  drug_dose
    0  0
    1  100
    2  50
    3  0

Requirements:
- First line must start with # and have 2 column names
- Column 1 header must match index name (or time/t)
- Column 2 header must match function name
- X values (column 1) must be strictly increasing
- Minimum 2 data rows required

=head1 FUNCTIONS

=head2 read_tfun_file($filename, $func_name, $index_name)

Read a .tfun file and return validated data arrays.

Dies with descriptive error if file is malformed.

=head2 validate_inline_data(\@x_vals, \@y_vals)

Validate inline tfun data arrays.

Dies with descriptive error if data is invalid.

=head1 AUTHOR

BioNetGen development team

=cut
