# NFsim Table Function (TFUN) Implementation Specification

## Overview

This document specifies the XML format and implementation requirements for table function (tfun) support in NFsim. BioNetGen now exports tfun functions to XML in a format that NFsim needs to implement.

## XML Format Specification

### File-Based TFUN

For file-based tfun definitions like `f() = tfun('data.tfun', time)`, BNG exports:

```xml
<Function id="f" type="TFUN" file="data.tfun" ctrName="time" method="linear">
  <ListOfReferences>
    <Reference name="__TFUN__VAL__" type="Constant"/>
    <Reference name="time" type="Observable"/>
  </ListOfReferences>
  <Expression> __TFUN__VAL__ </Expression>
</Function>
```

**Attributes:**
- `type="TFUN"` - Identifies this as a table function
- `file="..."` - Path to the .tfun data file (relative to model directory)
- `ctrName="..."` - Name of the index variable (time, parameter, or observable name)
- `method="..."` - Interpolation method (currently only "linear" is supported)

**Special Reference:**
- `__TFUN__VAL__` - Placeholder constant that represents the interpolated value from the table

### Inline TFUN

For inline tfun definitions like `f() = tfun([0, 1, 2], [10, 20, 30], time)`, BNG exports:

```xml
<Function id="f" type="TFUN" mode="inline" ctrName="time" xData="0,1,2" yData="10,20,30" method="linear">
  <ListOfReferences>
  </ListOfReferences>
  <Expression> 0.0 </Expression>
</Function>
```

**Attributes:**
- `type="TFUN"` - Identifies this as a table function
- `mode="inline"` - Indicates inline data (no external file)
- `ctrName="..."` - Name of the index variable
- `xData="..."` - Comma-separated x values (index values)
- `yData="..."` - Comma-separated y values (function values)
- `method="..."` - Interpolation method (default: "linear")

## File Format (.tfun)

TFUN data files use a simple two-column format:

```
# index_name  function_name
0  0
1  10
2  20
3  30
```

**Format requirements:**
- First line: `#` followed by two column headers
- Column 1 header: Must match the index variable name (or `time`/`t` for time-indexed)
- Column 2 header: Must match the function name
- Data rows: Two space/tab-separated numeric values
- Index column (first column): Must be monotonically increasing

## NFsim Implementation Requirements

### 1. XML Parsing

NFsim must detect and parse TFUN functions by checking for `type="TFUN"` attribute on `<Function>` elements.

For **file-based** tfuns:
1. Check for `file` attribute
2. Read the .tfun file from the specified path
3. Parse the file format (skip # header, read numeric pairs)
4. Validate monotonicity of index column
5. Build internal lookup table

For **inline** tfuns:
1. Check for `mode="inline"` attribute
2. Parse `xData` and `yData` comma-separated strings into arrays
3. Validate arrays have equal length
4. Validate x values are monotonically increasing
5. Build internal lookup table

### 2. Interpolation Implementation

**Linear interpolation (method="linear"):**
- For lookup value `x`:
  - If `x < x[0]`: return `y[0]` (constant extrapolation)
  - If `x > x[n-1]`: return `y[n-1]` (constant extrapolation)
  - Otherwise: Find interval `[x[i], x[i+1]]` containing `x`
  - Return linear interpolation: `y[i] + (y[i+1] - y[i]) * (x - x[i]) / (x[i+1] - x[i])`

**Future methods (not yet implemented):**
- `step` - Step interpolation (hold previous value)
- `cubic` - Cubic spline interpolation

### 3. Index Variable Resolution

The `ctrName` attribute specifies what quantity to use for the table lookup:

- **`time` or `t`**: Use current simulation time
- **Parameter name**: Use current value of that parameter
- **Observable name**: Use current value of that observable
- **Function name**: Use current evaluated value of that function

NFsim must be able to resolve these names and retrieve their current values during simulation.

### 4. Function Evaluation

When evaluating a rate law or expression containing a TFUN function:

1. Get the current value of the index variable (specified by `ctrName`)
2. Perform table lookup with interpolation
3. The interpolated value replaces `__TFUN__VAL__` in the expression
4. Evaluate any additional operations in the `<Expression>` element

**Example:**
```xml
<Expression> __TFUN__VAL__ * 2.0 </Expression>
```
This means: look up the table value, then multiply by 2.0.

### 5. Expression Composition

TFUN functions can appear in complex expressions:

```xml
<Function id="k_total" type="TFUN" file="base_rate.tfun" ctrName="time">
  <ListOfReferences>
    <Reference name="__TFUN__VAL__" type="Constant"/>
    <Reference name="scaling_factor" type="Constant"/>
  </ListOfReferences>
  <Expression> __TFUN__VAL__ * scaling_factor </Expression>
</Function>
```

NFsim must:
1. Perform the table lookup to get `__TFUN__VAL__`
2. Resolve other variables in the expression (`scaling_factor`)
3. Evaluate the complete expression

### 6. Validation and Error Handling

NFsim should validate:
- .tfun files exist and are readable (file-based mode)
- Index values are monotonically increasing
- xData and yData have equal length (inline mode)
- Referenced index variable exists (parameter/observable/function)
- At least 2 data points are provided

Error messages should clearly indicate:
- Which function has the error
- What validation failed
- Helpful suggestions for fixing the issue

### 7. Testing Requirements

Implement tests for:
- **Basic functionality**: Simple time-indexed tfun
- **Observable indexing**: tfun indexed by an observable value
- **Parameter indexing**: tfun indexed by a parameter (for parameter scans)
- **Inline data**: tfun with inline arrays
- **Expression composition**: tfun combined with other operations
- **Extrapolation**: Behavior when index is outside data range
- **Edge cases**: Single interval (2 points), exact matches on data points

## Example Use Cases

### 1. Time-dependent forcing

```bngl
begin functions
    drug_dose() = tfun('dosing_schedule.tfun', time)
end functions

begin reaction rules
    Receptor(d~off) -> Receptor(d~on) drug_dose() * k_on
end reaction rules
```

### 2. Observable-dependent rate

```bngl
begin observables
    Molecules TotalProtein Protein()
end observables

begin functions
    feedback() = tfun('feedback_curve.tfun', TotalProtein)
end functions

begin reaction rules
    Gene() -> Gene() + Protein() feedback()
end reaction rules
```

### 3. Parameter scan with dose-response

```bngl
begin parameters
    drug_conc 0  # Will be scanned
end parameters

begin functions
    response() = tfun('dose_response.tfun', drug_conc)
end functions
```

### 4. Inline dosing schedule

```bngl
begin functions
    # Three doses over 24 hours
    dosing() = tfun([0, 8, 8.1, 16, 16.1, 24], [0, 0, 100, 100, 50, 50], time)
end functions
```

## Backward Compatibility

The old TFUN syntax (uppercase) is still supported:

```xml
<Function id="k1" type="TFUN" file="data.dat" ctrName="obs">
  <ListOfReferences>
    <Reference name="__TFUN__VAL__" type="Constant"/>
    <Reference name="obs" type="Observable"/>
  </ListOfReferences>
  <Expression> __TFUN__VAL__ </Expression>
</Function>
```

This has the same semantics as the new file-based tfun format (without `mode` attribute).

## Questions for NFsim Developers

1. **Expression evaluation**: Does NFsim's expression evaluator already support substituting the `__TFUN__VAL__` constant with a computed value?

2. **Observable lookup**: Can NFsim evaluate observables at any point during rate calculation, or only at specific update points?

3. **Performance**: For large tables (100+ points), should we recommend any optimization strategies (binary search, caching, etc.)?

4. **Stochastic simulation**: For SSA with table functions, should we evaluate the tfun at the time of event selection or event execution?

5. **Parameter sweeps**: For parameter-indexed tfuns, how should NFsim handle parameter changes during parameter_scan actions?

## Implementation Priority

**Phase 1 (Essential):**
- File-based tfun with time indexing
- Linear interpolation
- Constant extrapolation
- Basic expression support (`__TFUN__VAL__` replacement)

**Phase 2 (Important):**
- Observable indexing
- Parameter indexing
- Inline data support
- Expression composition (tfun in complex expressions)

**Phase 3 (Future):**
- Additional interpolation methods (step, cubic)
- Performance optimizations
- Advanced validation and error messages

## Contact

For questions about this specification or the BioNetGen implementation:
- BioNetGen GitHub: https://github.com/RuleWorld/bionetgen
- Issue #278: Table function support discussion
