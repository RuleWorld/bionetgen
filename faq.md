---
title: FAQ
layout: default
permalink: /faq
parent: Resources
nav_order: 1
---

# FAQ

## General questions
**Q:** *Do you still support RuleBender?*

**A:** No. RuleBender has been deprecated. It can still be downloaded [here](https://github.com/RuleWorld/rulebender/releases/latest), but it is no longer being supported. Please use the [VSCode extension](/vsc_ext) instead.

## PyBioNetGen & VS Code extension questions

## Modeling questions

## Parameter syntax

**Q:** *How do I define a parameter in terms of other parameters?*

**A:** Parameters may be defined using C-like expressions (use `^` rather than `**` for exponentiation) that involve other parameters. For example, you could define 
```
begin parameters
  alpha 5
  k_A 1e-5
  k_b alpha*k_A
end parameters
```
You can also use mathematical functions, e.g., `sin`, `exp`, `log`, etc. in these expressions. 
<!-- TODO: Reference to full list of supported functions -->


