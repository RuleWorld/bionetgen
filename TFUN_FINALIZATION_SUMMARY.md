# TFUN Feature Finalization Summary

## Overview

This document summarizes the work completed to finalize the table function (tfun) feature in BioNetGen, addressing issue #278 and preparing for NFsim integration.

## Completed Tasks

### 1. XML Inline TFUN Format (wshlavacek's comment)

**Issue**: wshlavacek requested clarification on the finalized XML format for inline tfun data.

**Solution**: Enhanced the XML export to include all inline tfun data as attributes:

**File-based tfun XML:**
```xml
<Function id="f" type="TFUN" file="data.tfun" ctrName="time" method="linear">
  <ListOfReferences>
    <Reference name="__TFUN__VAL__" type="Constant"/>
    <Reference name="time" type="Observable"/>
  </ListOfReferences>
  <Expression> __TFUN__VAL__ </Expression>
</Function>
```

**Inline tfun XML:**
```xml
<Function id="f" type="TFUN" mode="inline" ctrName="time"
          xData="0,1,2" yData="0,10,20" method="linear">
  <ListOfReferences>
  </ListOfReferences>
  <Expression> 0.0 </Expression>
</Function>
```

**Files Modified:**
- `bng2/Perl2/Function.pm` (lines 560-574): Added inline data export with xData, yData, and ctrName attributes

### 2. SBML Export Support

**Challenge**: SBML Level 2/3 does not have native table function support.

**Solution**: Implemented automatic conversion of tfun functions to piecewise linear MathML expressions.

**Features:**
- Reads tfun data from files or inline arrays
- Generates piecewise MathML with linear interpolation
- Handles extrapolation (constant beyond data range)
- Warns if tables are very large (>50 points)
- Handles both Unix and Windows line endings

**Example Output:**
```xml
<assignmentRule variable="f_time">
  <math xmlns="http://www.w3.org/1998/Math/MathML">
    <piecewise>
      <piece>
        <cn> 1.0 </cn>
        <apply><lt/><ci> time </ci><cn> 0.0 </cn></apply>
      </piece>
      <piece>
        <apply>
          <plus/>
          <cn> 1.0 </cn>
          <apply>
            <times/>
            <cn> 1 </cn>
            <apply><minus/><ci> time </ci><cn> 0.0 </cn></apply>
          </apply>
        </apply>
        <apply>
          <and/>
          <apply><geq/><ci> time </ci><cn> 0.0 </cn></apply>
          <apply><lt/><ci> time </ci><cn> 1.0 </cn></apply>
        </apply>
      </piece>
      <!-- ... more pieces ... -->
      <otherwise>
        <cn> 16.0 </cn>
      </otherwise>
    </piecewise>
  </math>
</assignmentRule>
```

**Files Modified:**
- `bng2/Perl2/Param.pm`:
  - Added `use TfunReader` (line 16)
  - Modified `toMathMLString()` to detect tfun and convert to piecewise (lines 383-408)
  - Added `tfun_to_mathml_piecewise()` function (lines 410-530)
  - Added `read_tfun_file_simple()` helper function (lines 532-588)

**Benefits:**
- SBML models with tfun now have identical behavior to BNGL models
- SBML simulators can run tfun-based models without modification
- No loss of functionality when exporting to SBML

**Limitations:**
- Large tables (>50 points) produce verbose SBML files
- Only linear interpolation is supported (consistent with current tfun implementation)
- Performance may be slower than native table lookup in NFsim

### 3. NFsim Implementation Specification

**Created**: `TFUN_NFSIM_SPEC.md` - Comprehensive specification for NFsim developers

**Contents:**
1. **XML Format Specification**: Complete documentation of both file-based and inline formats
2. **File Format (.tfun)**: Description of the two-column data file format
3. **Implementation Requirements**:
   - XML Parsing (file-based and inline modes)
   - Interpolation Implementation (linear with constant extrapolation)
   - Index Variable Resolution (time, parameters, observables, functions)
   - Function Evaluation (expression composition)
   - Validation and Error Handling
   - Testing Requirements
4. **Example Use Cases**:
   - Time-dependent forcing
   - Observable-dependent rates
   - Parameter scan with dose-response
   - Inline dosing schedules
5. **Implementation Priority**: Phased approach (Essential → Important → Future)

**Key NFsim Requirements:**

**Phase 1 (Essential):**
- File-based tfun with time indexing
- Linear interpolation with constant extrapolation
- Basic `__TFUN__VAL__` replacement in expressions

**Phase 2 (Important):**
- Observable and parameter indexing
- Inline data support
- Complex expression composition

**Phase 3 (Future):**
- Additional interpolation methods (step, cubic)
- Performance optimizations

### 4. Testing

**Test Files Created:**
- `test_tfun_sbml.bngl`: Comprehensive test for SBML export with both file-based and inline tfun
- `test_tfun_xml.bngl`: Test for XML export with inline data

**Test Files Used:**
- `test_data.tfun`: Sample tfun data file (5 points, exponential growth)
- All existing tfun validation tests continue to pass

**Validation:**
- XML export produces correct attributes for both modes
- SBML export generates valid piecewise MathML
- File reading handles both Unix and Windows line endings
- Inline data correctly encoded as comma-separated attributes

## Technical Details

### XML Format Attributes

**Common attributes:**
- `type="TFUN"` - Identifies table function
- `ctrName="..."` - Index variable name (time/parameter/observable)
- `method="linear"` - Interpolation method

**File-based specific:**
- `file="..."` - Path to .tfun data file

**Inline specific:**
- `mode="inline"` - Indicates embedded data
- `xData="..."` - Comma-separated x values
- `yData="..."` - Comma-separated y values

### SBML Piecewise Generation

The piecewise function structure:

1. **First piece**: Extrapolation for x < x[0] → return y[0]
2. **Middle pieces**: Linear interpolation for each interval [x[i], x[i+1]]
   - Formula: y[i] + slope * (x - x[i])
   - Slope: (y[i+1] - y[i]) / (x[i+1] - x[i])
   - Condition: x >= x[i] AND x < x[i+1] (last interval uses <=)
3. **Otherwise**: Extrapolation for x > x[n-1] → return y[n-1]

### File Reading

The simple tfun file reader (`read_tfun_file_simple`):
- Handles both Unix (`\n`) and Windows (`\r\n`) line endings
- Skips header lines starting with `#`
- Skips empty lines and additional comments
- Validates at least 2 data points exist
- Returns arrays by reference for efficiency
- Provides clear error messages

## Backward Compatibility

All changes maintain backward compatibility:
- Old uppercase TFUN syntax still supported
- Existing XML format unchanged for file-based tfun
- New inline format is additive (new `mode` attribute)

## Next Steps for NFsim

1. **Review Specification**: NFsim developers should review `TFUN_NFSIM_SPEC.md`
2. **Implement Phase 1**: Basic file-based tfun with time indexing
3. **Test with BNG exports**: Use test files in `bng2/Validate/test_tfun_*.bngl`
4. **Implement Phase 2**: Observable/parameter indexing and inline support
5. **Performance testing**: Evaluate performance with large tables

## Files Modified

| File | Lines Modified | Changes |
|------|---------------|---------|
| `bng2/Perl2/Function.pm` | 560-574 | Enhanced XML inline tfun export |
| `bng2/Perl2/Param.pm` | 16, 383-588 | Added SBML piecewise conversion |

## Files Created

| File | Purpose |
|------|---------|
| `TFUN_NFSIM_SPEC.md` | NFsim implementation specification |
| `TFUN_FINALIZATION_SUMMARY.md` | This document |
| `bng2/Validate/test_tfun_sbml.bngl` | SBML export test |

## Issue Resolution

**Issue #278 - wshlavacek's comment**: ✅ **RESOLVED**
- XML format finalized with full inline data support
- Both file-based and inline modes fully documented
- PyBioNetFit can now parse XML correctly

**SBML Support**: ✅ **IMPLEMENTED**
- Models exported to SBML have identical behavior to BNGL
- Automatic piecewise conversion maintains mathematical equivalence
- Clear warnings for edge cases (large tables)

**NFsim Support**: ✅ **DOCUMENTED**
- Complete specification ready for implementation
- XML format stable and well-defined
- Test cases available for validation

## Conclusion

The tfun feature is now fully finalized for BioNetGen, with complete SBML support and a comprehensive specification for NFsim implementation. All original requirements have been met:

✅ Addressed wshlavacek's XML format question
✅ SBML export provides identical model behavior
✅ NFsim XML format finalized and documented
✅ NFsim implementation requirements specified

The next phase is NFsim implementation, which should follow the phased approach outlined in `TFUN_NFSIM_SPEC.md`.
