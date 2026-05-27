use strict;
use warnings;
use Test::More;
use FindBin;
use lib "$FindBin::Bin/..";

require_ok('BNGModel');

# Test successful evaluation of a safe math string
my ($val, $err) = BNGModel::_safe_reval_literal("2 + 2");
is($val, 4, "safe math string should evaluate correctly");
is($err, '', "safe math string should have no error");

# Test safe math string with scientific notation
($val, $err) = BNGModel::_safe_reval_literal("2.5e2 * 2");
is($val, 500, "safe math string with scientific notation should evaluate correctly");
is($err, '', "safe math string with scientific notation should have no error");

# Test array reference construction which is typical for action args
($val, $err) = BNGModel::_safe_reval_literal("['a', 'b', 'c']");
is(ref($val), 'ARRAY', "array reference literal should evaluate to ARRAY");
is_deeply($val, ['a', 'b', 'c'], "array reference literal should evaluate to correct structure");
is($err, '', "array reference literal should have no error");

# Test hash-like structure construction which is typical for action args
($val, $err) = BNGModel::_safe_reval_literal("['method'=>'ssa', 't_end'=>10]");
is(ref($val), 'ARRAY', "key-value array literal should evaluate to ARRAY");
is_deeply($val, ['method', 'ssa', 't_end', 10], "key-value array literal should evaluate to correct structure");
is($err, '', "key-value array literal should have no error");

# Test that unsafe operations are blocked (system)
($val, $err) = BNGModel::_safe_reval_literal("system('echo unsafe')");
is($val, undef, "unsafe string (system) should evaluate to undef");
like($err, qr/trapped by operation mask/, "unsafe string (system) should return operation mask error message");

# Test that unsafe operations are blocked (backticks)
($val, $err) = BNGModel::_safe_reval_literal("`echo unsafe`");
is($val, undef, "unsafe string (backticks) should evaluate to undef");
like($err, qr/trapped by operation mask/, "unsafe string (backticks) should return operation mask error message");

# Test that unsafe operations are blocked (eval)
($val, $err) = BNGModel::_safe_reval_literal("eval '2+2'");
is($val, undef, "unsafe string (eval) should evaluate to undef");
like($err, qr/trapped by operation mask/, "unsafe string (eval) should return operation mask error message");

# Test invalid syntax
($val, $err) = BNGModel::_safe_reval_literal("2 + ");
is($val, undef, "invalid syntax should evaluate to undef");
like($err, qr/syntax error/i, "invalid syntax should return syntax error message");

done_testing();
