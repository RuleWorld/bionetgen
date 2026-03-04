# Reply to wshlavacek - Issue #278

Hi @wshlavacek,

Thanks for your question about the XML inline tfun format. I've now finalized the implementation and wanted to get your feedback before merging to master.

## Finalized XML Format

The XML export now includes complete inline tfun data as attributes:

### File-based tfun:
```xml
<Function id="f_time" type="TFUN" file="test_data.tfun" ctrName="time" method="linear">
  <ListOfReferences>
    <Reference name="__TFUN_VAL__" type="Constant"/>
    <Reference name="time" type="Observable"/>
  </ListOfReferences>
  <Expression> __TFUN_VAL__ </Expression>
</Function>
```

### Inline tfun:
```xml
<Function id="f_inline" type="TFUN" mode="inline" ctrName="time"
          xData="0,1,2" yData="0,10,20" method="linear">
  <ListOfReferences>
  </ListOfReferences>
  <Expression> __TFUN_VAL__ </Expression>
</Function>
```

### Key Attributes:
- `type="TFUN"` - Identifies table function
- `ctrName="..."` - Index variable (time/parameter/observable)
- `method="linear"` or `method="step"` - Interpolation method
- **File-based**: `file="..."` - Path to .tfun data file
- **Inline**: `mode="inline"`, `xData="..."`, `yData="..."` - Embedded data as comma-separated values

### Expression Placeholder: `__TFUN_VAL__`

The `<Expression>` element uses `__TFUN_VAL__` as a placeholder for the tfun evaluation result. This makes it clear to parsers (like PyBioNetFit) that:
1. The value should be computed from the tfun data (file or inline)
2. It's not a literal numeric value
3. It can be part of complex expressions

**Examples:**
- Simple: `<Expression> __TFUN_VAL__ </Expression>` → just the tfun value
- Complex: `<Expression> __TFUN_VAL__/10 </Expression>` → tfun value divided by 10
- With parameters: `<Expression> (__TFUN_VAL__+5)*k_scale </Expression>` → more complex formula

This approach is consistent with the legacy uppercase `TFUN()` format and provides clear semantics for implementation.

## Additional Updates

### 1. SBML Export Support
Since SBML doesn't have native table function support, I've implemented automatic conversion to piecewise MathML expressions:
- Linear interpolation → piecewise linear functions
- Step interpolation → piecewise constant functions
- Handles extrapolation (constant beyond data range)
- See `bng2/Validate/test_tfun_sbml.bngl` and `test_tfun_step_sbml.bngl` for examples

### 2. NFsim Implementation Spec
Created comprehensive specification document (`TFUN_NFSIM_SPEC.md`) with:
- Complete XML format documentation
- Implementation requirements and algorithms
- Test cases and validation criteria
- Phased implementation approach

## Testing

All validation tests pass (68 models), including:
- 7 existing tfun tests
- New SBML export tests with both linear and step interpolation
- No regressions in existing functionality

## Request for Feedback

Before I submit the PR, could you review:
1. Does the XML inline format work for PyBioNetFit's needs?
2. Are the attribute names (`xData`, `yData`, `ctrName`, `method`) clear and appropriate?
3. Any concerns with the comma-separated format for data arrays?
4. Is the `__TFUN_VAL__` placeholder clear and appropriate for parsing? (Changed from `0.0` to avoid confusion with literal values)

**Note**: I initially used `0.0` as the expression placeholder, but changed to `__TFUN_VAL__` for clarity - it's now immediately obvious to parsers that this is a placeholder that needs special handling, not a literal zero.

The implementation is on branch `tfun-dev` and ready for review. Let me know if you'd like me to adjust anything!

Thanks,
Jim
