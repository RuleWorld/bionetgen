# Table Function (TFUN) Support for BioNetGen

## Summary

This PR implements comprehensive table function (tfun) support for BioNetGen, addressing feature request #278. Table functions enable users to define time-varying or data-driven functions using tabulated data with linear or step interpolation.

## Key Features

### 1. **Parser Support** (`bng2/Perl2/Expression.pm`, `bng2/Perl2/Function.pm`)
- New lowercase `tfun()` syntax with two modes:
  - **File-based**: `tfun('data.tfun', time)` - reads data from external file
  - **Inline**: `tfun([0,1,2], [10,20,30], time)` - embedded data arrays
- Complex expression support: `tfun(...) / 10`, `tfun(...) * k_scale`, etc.
- Backward compatibility with legacy uppercase `TFUN()` syntax

### 2. **Data Validation** (Issue #278)
Implements validation requirements from @wshlavacek:
- ✅ Whitespace trimming in data arrays
- ✅ Scientific notation support (`1.5e-3`, `2E+5`, etc.)
- ✅ Equal length enforcement for xData and yData
- ✅ Strictly increasing xData validation
- Clear error messages for validation failures

### 3. **XML Export** (`bng2/Perl2/Function.pm`)
Standardized XML format for NFsim integration:
```xml
<Function id="f" type="TFUN" mode="inline" ctrName="time"
         xData="0,1,2" yData="0,10,20" method="linear">
  <ListOfReferences></ListOfReferences>
  <Expression> __TFUN_VAL__ </Expression>
</Function>
```
- Standardized `__TFUN_VAL__` placeholder token (single underscore)
- Complete inline data exported as XML attributes
- Supports both file-based and inline modes

### 4. **SBML Export** (`bng2/Perl2/Param.pm`)
Automatic conversion to piecewise MathML for SBML compatibility:
- Linear interpolation → piecewise linear functions
- Step interpolation → piecewise constant functions
- Handles extrapolation (constant beyond data range)
- Enables SBML simulators to run tfun-based models without modification

### 5. **Network Simulator Support** (`Network3/`)
Full integration with run_network/C++ simulator:
- `Tfun` class with linear and step interpolation (`Network3/src/util/tfun.cpp`)
- Dynamic evaluation during simulation
- Support for time, observable, and parameter indexing

### 6. **File I/O** (`bng2/Perl2/TfunReader.pm`)
Robust .tfun file reader:
- Handles Unix and Windows line endings
- Skips comments and empty lines
- Validates minimum 2 data points
- Clear error messages

## Test Coverage

### Validation Tests (`bng2/Validate/`)
- **test_tfun_expr.bngl** - Complex expressions with tfun
- **test_tfun_validation.bngl** - Scientific notation, whitespace, negatives, decimals
- **test_tfun_validation_errors.bngl** - Error case validation
- **test_tfun_file.bngl** - File-based tfun
- **test_tfun_observable.bngl** - Observable indexing
- **test_tfun_ode_simple.bngl** - ODE simulation with linear interpolation
- **test_tfun_ode_step.bngl** - ODE simulation with step interpolation
- **test_tfun_step_sbml.bngl** - SBML export with step interpolation

All 73 validation tests pass with no regressions.

## Merge Compatibility Fixes

This PR includes fixes required for merging master into tfun-dev:

### Integration with time() Function (PR #222)
- Added `time_pointer` member to NETWORK struct for time() function support
- Implemented proper initialization in `init_network()` to prevent undefined behavior
- Restored `*network.time_pointer = t` update in `derivs_network()` that was lost during merge conflict resolution

### Fixed test_time Numerical Instability
The original `test_time.bngl` created an exponentially growing ODE system that caused CVODE integration failure:
- **Problem**: All reactions produced the same species X, creating `dX/dt = 1 + X + time()` (exponential growth)
- **Solution**: Refactored to use separate species (T, A, B) for each measurement, creating stable quadratic growth
- **Tolerance Adjustment**: Relaxed integration tolerances from 1e-12 to 1e-8 for cross-platform compatibility
- **Result**: Test now properly validates time() function accuracy during ODE integration across Mac, Linux, and Windows

The tfun implementation coexists cleanly with the time() function - both features use the same time_pointer mechanism and work independently.

## Documentation

- **TFUN_NFSIM_SPEC.md** - Comprehensive NFsim implementation specification
- **TFUN_FINALIZATION_SUMMARY.md** - Feature summary and implementation details
- **ISSUE_278_REPLY.md** - Response addressing @wshlavacek's requirements

## Migration Notes

- Fully backward compatible with existing models
- New lowercase `tfun()` syntax recommended for new models
- Legacy uppercase `TFUN()` syntax still supported
- No breaking changes to existing APIs

## Closes

Closes #278

## Collaboration

Special thanks to @wshlavacek for detailed requirements and validation specifications that guided this implementation.
