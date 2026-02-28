# $Id: Param.pm,v 1.3 2006/09/26 03:36:00 faeder Exp $

package Param;

# pragmas
#use strict;
#use warnings;
no warnings 'redefine';

# Perl Modules
use Class::Struct;

# BNG Modules
use ParamList;
use Expression;
use TfunReader;



# A "parameter" object is a name associated with an Expression, Function or Observable.
struct Param =>
{
    Name     => '$',          # Parameter name
    Expr     => 'Expression', # Refers to the expression object
    Type     => '$',          # Parameter type (see below for allowed types)
    Ref      => '$',          # For Observables and Functions: refers to the function or observable object
    CVodeRef => '$',          # describes the parameter location in a CVode vector
    Index    => '$',          # vector index for network outputs.  NOTE: Constant and ConstantExpression type
                              #   are indexed separately from Observable type.  Function and Local types not indexed.
};


# This hash describes the allowed parameter types
my %allowedTypes = ( Constant           => 1,  # A number or an expression involving only numbers
                     ConstantExpression => 1,  # An expression involving at least one other Constant or ConstantExpression
		             Observable         => 1,  # A variable defined by the name of an Observable - may take a single argument
		             Function           => 1,  # A function call w/ arguments
                     Local              => 1,  # local arguments to functions
                     RRef               => 1,  # Reactant Reference (not yet implemented)
                   );


###
###
###


# evaluate the expression that this parameter refers to
sub evaluate
{
    my $param = shift @_;
    my $args  = @_ ? shift @_ : [];  
    my $plist = @_ ? shift @_ : undef;    
    my $level = @_ ? shift @_ : 0; 

    #print STDERR "eval param name: ", $param->Name ,"  type: ", $param->Type, "\n";
    if ($param->Type eq 'Constant'  or  $param->Type eq 'ConstantExpression' )
    {
        if ( defined $param->Expr )
        {   return $param->Expr->evaluate($plist, $level+1);   }
    }
    elsif ( $param->Type eq 'Function'  or $param->Type eq 'Observable' )
    {
        if ( defined $param->Ref )
        {   return $param->Ref->evaluate($args, $plist, $level+1);   }
    }
    return undef;
}


###
###
###


sub copyConstant
{
    my $param = shift @_ ;
    my $plist = @_ ? shift @_ : undef;

    return undef unless ( $param->Type eq 'Constant'  or  $param->Type eq 'ConstantExpression' );

    my $param_copy = Param->new( 'Name' => $param->Name, 'Type' => $param->Type );
    if (defined $param->Expr)
    {
        my ($expr_copy, $err) = $param->Expr->clone($plist);
        if ($err) {  print "$err\n";  return undef;  }
        $param_copy->Expr( $expr_copy );
    }
    return $param_copy;
}


###
###
###


# set the type field of this parameter
sub setType
{
    my $param = shift @_;
    my $type  = shift @_;
    my $ref   = @_ ? shift @_ : '';

    unless ( exists $allowedTypes{$type} )
    {
        die "Type $type not recognized in setType";
    }

    $param->Type($type);
    if ($ref) {  $param->Ref($ref);  }
    return '';
}


###
###
###


# Write the expression that this parameter refers to:
# expand = 0,1 : always write expression
# expand = 2   : write expression, unless constant (just write param name in this case).
sub toString
{
    my $param  = shift @_;
    my $plist  = @_ ? shift @_ : undef;
    my $level  = @_ ? shift @_ : 0;
    my $expand = @_ ? shift @_ : 0;
  
    if ( $param->Type eq 'Constant'  and  $expand == 2 )
    {
        return $param->Name;
    }
    else
    {
        if ( defined $param->Expr )
        {
            return $param->Expr->toString($plist, $level, $expand);
        }   
        else
        {   # TODO: ??
            return '';
        }
    }
}


###
###
###


# Initialize Parameter from XML hash
# ($param, $err) = Param->readXML( \%xml_hash, $plist ) 
sub readXML
{
    my ($xml_hash, $plist) = @_;

    # Read expression
    my $string = $xml_hash->{'-value'};
    my $expr = Expression->new();
    $expr->setAllowForward(1);
    if ( my $err = $expr->readString( \$string, $plist ) ) { return undef, $err; }
    $expr->setAllowForward(0);

    # setup parameter
    my $param = Param->new( 'Name'  => $xml_hash->{'-id'},
                            'Type' => $xml_hash->{'-type'},
                            'Expr' => $expr );

    # return parameter
    return $param, undef;
}


###
###
###


# Write the parameter to XML
sub toXML
{
    my $param  = shift @_;
    my $plist  = @_ ? shift @_ : undef;
    my $level  = @_ ? shift @_ : 0;
    my $expand = @_ ? shift @_ : 0;

    if ( $param->Type eq 'Constant'  and  $expand == 2 )
    {
        return $param->Name;
    }
    else
    {  
        if ( $param->Expr )
        {
            return $param->Expr->toXML($plist, $level, $expand);
        }
        else
        {
            return '';
        }
    }
}



###
###
###


# write parameter expression as a CVode string
sub toCVodeString
{
    my $param  = shift @_;
    my $plist  = @_ ? shift @_ : undef;
    my $level  = @_ ? shift @_ : 0;
    my $expand = @_ ? shift @_ : 0;    
  
    if ( $param->Type eq 'Constant')
    {
        return $param->getCVodeName;
    }
    elsif ( defined $param->Expr )
    {
        return $param->Expr->toCVodeString($plist, $level, $expand);
    }
    else
    {
        return '';
    }
}


###
###
###


sub getCVodeName
{
    my $param = shift @_;
    
    my $type = $param->Type;
    my $expr = $param->Expr;
    
    my $name;
    if    ( $type eq 'Constant')
    {
        $name = 'NV_Ith_S(expressions,' . $param->Index .')';
    }
    elsif ( $type eq 'ConstantExpression' )
    {        
        $name = 'NV_Ith_S(expressions,' . $param->Index .')';   
    }
    elsif ( $type eq 'Observable' )
    {
        $name = 'NV_Ith_S(observables,' . $param->Index .')'; 
    }
    elsif ( $type eq 'Function' )
    {
        $name = $param->Name;
    }
    elsif ( $type eq 'Local' )
    {
        $name = $param->Name;
    }

    return $name;
}


###
###
###


sub getMatlabName
{
    my $param = shift;
    
    my $name;
    my $offset = 1;
    if    ( $param->Type eq 'Constant')
    {
        $name = 'expressions(' . ($param->Index + $offset) .')';
    }
    elsif ( $param->Type eq 'ConstantExpression' )
    {        
        $name = 'expressions(' . ($param->Index + $offset) .')';   
    }
    elsif ( $param->Type eq 'Observable' )
    {
        $name = 'observables(' . ($param->Index + $offset) .')'; 
    }
    elsif ( $type eq 'Function' )
    {
        $name = $param->Name;
    }
    elsif ( $type eq 'Local' )
    {
        $name = $param->Name;
    }
    elsif ( $type eq 'RRef' )
    {
        $name = $param->Name;
    }
    
    return $name;
}


###
###
###


# write the parameter for Matlab output
sub toMatlabString
{
    my $param  = shift;
    my $plist  = (@_) ? shift : undef;
    my $level  = (@_) ? shift : 0;
    my $expand = (@_) ? shift : 0;    
  
    my $offset = 1;
    if ( $param->Type eq 'Constant' )
    {
        return 'expressions(' . ($param->Index + $offset) . ')';
    }
    elsif ( $param->Expr )
    {
        return $param->Expr->toMatlabString($plist, $level, $expand);
    }
    else
    {
        return '';
    }
}



###
###
###


sub toXMLReference
{
    my $param = shift;
    my $head  = shift;
    my $id    = shift;
    my $plist = (@_) ? shift : '';
  
    if ($head eq ''){  $head = 'Reference';  }
    my $string="<$head";

    # Attributes
    # id
    if ($id ne '')
    {
        $string.= " id=\"".$param->Name."\"";
    }
    # name 
    $string.= " name=\"".$param->Name."\"";
    # type
    $string.= " type=\"".$param->Type."\"";
  
    $string.= "/>\n";
  
    return ($string);  
}


###
###
###


sub toMathMLString
{
    my $param  = shift;
    my $plist  = (@_) ? shift : '';
    my $indent = (@_) ? shift : '';

    if ($param->Expr)
    {
        # Check if this is a tfun function - if so, convert to piecewise
        if ($param->Expr->tfunData) {
            return tfun_to_mathml_piecewise($param->Expr->tfunData, $indent);
        }
        # Check for old TFUN format
        elsif ($param->Expr->tfunFile) {
            my $tfun_data = {
                mode => 'file',
                file => $param->Expr->tfunFile,
                index => $param->Expr->ctrName,
                method => 'linear'
            };
            return tfun_to_mathml_piecewise($tfun_data, $indent);
        }
        return($param->Expr->toMathMLString($plist,$indent));
    }
    return ('');
}

# Convert tfun data to MathML piecewise function for SBML export
sub tfun_to_mathml_piecewise
{
    my $data = shift;
    my $indent = shift || '';

    my @x_vals;
    my @y_vals;

    # Load data based on mode
    if ($data->{mode} eq 'file') {
        # Read tfun file
        # For SBML export, we use lenient reading (no func/index name validation)
        my ($x_ref, $y_ref, $err) = read_tfun_file_simple($data->{file});
        if ($err) {
            warn "WARNING: Cannot convert tfun file '$data->{file}' to SBML: $err\n";
            warn "         Using constant value 0.0 instead.\n";
            return $indent . "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n" .
                   $indent . "  <cn> 0.0 </cn>\n" .
                   $indent . "</math>\n";
        }
        @x_vals = @$x_ref;
        @y_vals = @$y_ref;
    }
    elsif ($data->{mode} eq 'inline') {
        @x_vals = @{$data->{x_vals}};
        @y_vals = @{$data->{y_vals}};
    }
    else {
        warn "WARNING: Unknown tfun mode: $data->{mode}\n";
        return $indent . "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n" .
               $indent . "  <cn> 0.0 </cn>\n" .
               $indent . "</math>\n";
    }

    # Warn if table is very large
    my $n_points = scalar(@x_vals);
    if ($n_points > 50) {
        warn "WARNING: tfun table has $n_points points - SBML export may produce large file\n";
        warn "         Consider using NFsim for better performance with large tables\n";
    }

    my $index_var = $data->{index};
    my $method = $data->{method} || 'linear';

    # Build piecewise function based on interpolation method
    if ($method eq 'step') {
        return tfun_to_mathml_step(\@x_vals, \@y_vals, $index_var, $indent);
    }

    # Default: linear interpolation
    my $string = $indent . "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n";
    my $indent2 = $indent . "  ";
    my $indent3 = $indent . "    ";
    my $indent4 = $indent . "      ";

    # Build piecewise function
    $string .= $indent2 . "<piecewise>\n";

    # Handle below range (extrapolation to first value)
    $string .= $indent3 . "<piece>\n";
    $string .= $indent4 . sprintf("<cn> %s </cn>\n", $y_vals[0]);
    $string .= $indent4 . "<apply>\n";
    $string .= $indent4 . "  <lt/>\n";
    $string .= $indent4 . "  <ci> $index_var </ci>\n";
    $string .= $indent4 . sprintf("  <cn> %s </cn>\n", $x_vals[0]);
    $string .= $indent4 . "</apply>\n";
    $string .= $indent3 . "</piece>\n";

    # Build linear interpolation for each interval
    for (my $i = 0; $i < $n_points - 1; $i++) {
        my $x0 = $x_vals[$i];
        my $x1 = $x_vals[$i+1];
        my $y0 = $y_vals[$i];
        my $y1 = $y_vals[$i+1];

        # Calculate slope: m = (y1 - y0) / (x1 - x0)
        my $dx = $x1 - $x0;
        my $dy = $y1 - $y0;
        my $slope = ($dx != 0) ? $dy / $dx : 0;

        # Interpolation formula: y0 + slope * (x - x0)
        $string .= $indent3 . "<piece>\n";
        $string .= $indent4 . "<apply>\n";
        $string .= $indent4 . "  <plus/>\n";
        $string .= $indent4 . sprintf("  <cn> %s </cn>\n", $y0);
        $string .= $indent4 . "  <apply>\n";
        $string .= $indent4 . "    <times/>\n";
        $string .= $indent4 . sprintf("    <cn> %s </cn>\n", $slope);
        $string .= $indent4 . "    <apply>\n";
        $string .= $indent4 . "      <minus/>\n";
        $string .= $indent4 . "      <ci> $index_var </ci>\n";
        $string .= $indent4 . sprintf("      <cn> %s </cn>\n", $x0);
        $string .= $indent4 . "    </apply>\n";
        $string .= $indent4 . "  </apply>\n";
        $string .= $indent4 . "</apply>\n";

        # Condition: x >= x0 AND x < x1 (except for last interval where we use <=)
        $string .= $indent4 . "<apply>\n";
        $string .= $indent4 . "  <and/>\n";
        $string .= $indent4 . "  <apply>\n";
        $string .= $indent4 . "    <geq/>\n";
        $string .= $indent4 . "    <ci> $index_var </ci>\n";
        $string .= $indent4 . sprintf("    <cn> %s </cn>\n", $x0);
        $string .= $indent4 . "  </apply>\n";
        $string .= $indent4 . "  <apply>\n";
        if ($i == $n_points - 2) {
            # Last interval: use <= to include endpoint
            $string .= $indent4 . "    <leq/>\n";
        } else {
            $string .= $indent4 . "    <lt/>\n";
        }
        $string .= $indent4 . "    <ci> $index_var </ci>\n";
        $string .= $indent4 . sprintf("    <cn> %s </cn>\n", $x1);
        $string .= $indent4 . "  </apply>\n";
        $string .= $indent4 . "</apply>\n";
        $string .= $indent3 . "</piece>\n";
    }

    # Otherwise (above range): extrapolate to last value
    $string .= $indent3 . "<otherwise>\n";
    $string .= $indent4 . sprintf("<cn> %s </cn>\n", $y_vals[-1]);
    $string .= $indent3 . "</otherwise>\n";

    $string .= $indent2 . "</piecewise>\n";
    $string .= $indent . "</math>\n";

    return $string;
}

# Simple tfun file reader for SBML export (no validation)
# Returns (\@x_vals, \@y_vals, $error)
sub read_tfun_file_simple
{
    my $filename = shift;

    my @x_vals;
    my @y_vals;

    # Try to open file
    my $fh;
    unless (open($fh, '<', $filename)) {
        return (undef, undef, "Cannot open file: $!");
    }

    # Skip header line if present
    my $first_line = <$fh>;
    unless (defined($first_line)) {
        close($fh);
        return (undef, undef, "File is empty");
    }

    # If first line is not a header (doesn't start with #), process it as data
    $first_line =~ s/\r?\n$//;  # Remove line ending (handles both Unix and Windows)
    $first_line =~ s/^\s+//;  # Remove leading whitespace
    unless ($first_line =~ /^#/) {
        my @vals = split(/\s+/, $first_line);
        if (scalar(@vals) >= 2 && $vals[0] =~ /^[+-]?\d/ && $vals[1] =~ /^[+-]?\d/) {
            push @x_vals, $vals[0];
            push @y_vals, $vals[1];
        }
    }

    # Read data lines
    while (my $line = <$fh>) {
        $line =~ s/\r?\n$//;  # Remove line ending (handles both Unix and Windows)
        $line =~ s/^\s+//;  # Remove leading whitespace
        # Skip comments and empty lines
        next if ($line =~ /^#/ || $line =~ /^$/);

        my @vals = split(/\s+/, $line);
        next unless (scalar(@vals) >= 2 && $vals[0] =~ /^[+-]?\d/ && $vals[1] =~ /^[+-]?\d/);

        push @x_vals, $vals[0];
        push @y_vals, $vals[1];
    }

    close($fh);

    # Validate we have at least 2 points
    if (scalar(@x_vals) < 2) {
        my $msg = "Need at least 2 data points, found " . scalar(@x_vals);
        $msg .= " (possible file format issue - check for proper line endings and numeric data)";
        return (undef, undef, $msg);
    }

    return (\@x_vals, \@y_vals, undef);
}

# Convert tfun with step interpolation to MathML piecewise
# Step function is left-continuous: f(x) = y[i] for x in [x[i], x[i+1])
sub tfun_to_mathml_step
{
    my ($x_vals_ref, $y_vals_ref, $index_var, $indent) = @_;

    my @x_vals = @$x_vals_ref;
    my @y_vals = @$y_vals_ref;
    my $n_points = scalar(@x_vals);

    my $string = $indent . "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n";
    my $indent2 = $indent . "  ";
    my $indent3 = $indent . "    ";
    my $indent4 = $indent . "      ";

    $string .= $indent2 . "<piecewise>\n";

    # Handle below range (extrapolation to first value)
    $string .= $indent3 . "<piece>\n";
    $string .= $indent4 . sprintf("<cn> %s </cn>\n", $y_vals[0]);
    $string .= $indent4 . "<apply>\n";
    $string .= $indent4 . "  <lt/>\n";
    $string .= $indent4 . "  <ci> $index_var </ci>\n";
    $string .= $indent4 . sprintf("  <cn> %s </cn>\n", $x_vals[0]);
    $string .= $indent4 . "</apply>\n";
    $string .= $indent3 . "</piece>\n";

    # Build step function for each interval
    # Step is left-continuous: value = y[i] for x in [x[i], x[i+1])
    for (my $i = 0; $i < $n_points - 1; $i++) {
        my $x0 = $x_vals[$i];
        my $x1 = $x_vals[$i+1];
        my $y0 = $y_vals[$i];

        $string .= $indent3 . "<piece>\n";
        $string .= $indent4 . sprintf("<cn> %s </cn>\n", $y0);

        # Condition: x >= x[i] AND x < x[i+1]
        $string .= $indent4 . "<apply>\n";
        $string .= $indent4 . "  <and/>\n";
        $string .= $indent4 . "  <apply>\n";
        $string .= $indent4 . "    <geq/>\n";
        $string .= $indent4 . "    <ci> $index_var </ci>\n";
        $string .= $indent4 . sprintf("    <cn> %s </cn>\n", $x0);
        $string .= $indent4 . "  </apply>\n";
        $string .= $indent4 . "  <apply>\n";
        $string .= $indent4 . "    <lt/>\n";
        $string .= $indent4 . "    <ci> $index_var </ci>\n";
        $string .= $indent4 . sprintf("    <cn> %s </cn>\n", $x1);
        $string .= $indent4 . "  </apply>\n";
        $string .= $indent4 . "</apply>\n";
        $string .= $indent3 . "</piece>\n";
    }

    # Handle the last point and above range
    # For x >= x[n-1], return y[n-1]
    $string .= $indent3 . "<otherwise>\n";
    $string .= $indent4 . sprintf("<cn> %s </cn>\n", $y_vals[-1]);
    $string .= $indent3 . "</otherwise>\n";

    $string .= $indent2 . "</piecewise>\n";
    $string .= $indent . "</math>\n";

    return $string;
}

1;
