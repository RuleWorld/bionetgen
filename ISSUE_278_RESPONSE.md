Hi @wshlavacek,

Thanks for the feedback! I've addressed all your points from comment #4:

## 1. Token Standardization ✅

Fixed the inconsistency - now using `__TFUN_VAL__` (single underscore) throughout:
- Updated parser code in `bng2/Perl2/Expression.pm` and `bng2/Perl2/Function.pm`
- Updated all documentation files
- All XML exports now use the standardized token

## 2. Validation Implementation ✅

Implemented all four validation requirements in the parser (`bng2/Perl2/Expression.pm`, lines 210-238):

**a) Whitespace trimming** - Done (arrays are trimmed before validation)

**b) Scientific notation support** - Accepted formats: `1.5e-3`, `2E+5`, etc.

**c) Equal length enforcement** - Error if xData and yData have different lengths:
```
ERROR: tfun() xData and yData must have equal lengths (got 3 and 2 values)
```

**d) Strictly increasing xData** - Error if x values are not strictly increasing:
```
ERROR: tfun() xData must be strictly increasing, but x[3]=2.5 <= x[2]=3
```

## 3. Test Files

To see the finalized NET and XML formats, run these validation tests:

```bash
perl bng2/BNG2.pl bng2/Validate/test_tfun_expr.bngl
perl bng2/BNG2.pl bng2/Validate/test_tfun_validation.bngl
```

**test_tfun_expr.bngl** demonstrates:
- Simple tfun expressions
- Complex expressions (`tfun(...)/10`, `tfun(...)*k_scale`, etc.)
- The `__TFUN_VAL__` placeholder in XML

**test_tfun_validation.bngl** demonstrates:
- Scientific notation: `[1e-3, 2e-2, 3e-1]`
- Whitespace handling: `[ 0 , 1 , 2 ]`
- Negative values: `[-2, -1, 0, 1, 2]`
- Decimal values: `[0.5, 1.5, 2.5]`

## XML Output Example

```xml
<Function id="f_sci" type="TFUN" mode="inline" ctrName="time"
         xData="1e-3,2e-2,3e-1" yData="1.5e+2,2.5e+2,3.5e+2" method="linear">
  <ListOfReferences>
  </ListOfReferences>
  <Expression> __TFUN_VAL__ </Expression>
</Function>
```

## Summary

All items from your comment #4 are now complete:
1. ✅ Implementing against this format
2. ✅ ctrName, xData, yData, method names confirmed
3. ✅ Validation: trim whitespace, scientific notation, equal lengths, strictly increasing xData
4. ✅ `__TFUN_VAL__` token standardized

Ready for PyBioNetFit implementation. Let me know if you need anything else!

Thanks,
Jim
