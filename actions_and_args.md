---
title: BNG Actions and Arguments
layout: default
permalink: /actions_and_args
parent: Resources
nav_order: 3
---

# Actions and Arguments for BNGL

The table below represents the valid actions and arguments in BNGL.

| Action<br>`action({arg=>val,...})` | File Types | Allowed Arguments<br>`arg=>val_type` | Argument Desc. | Default Value | Available Since | Example |
| --- | --- | --- | --- | --- | --- | --- |
| `generate_network` | .net | `prefix=>"string"` | Change the base filename for output. | `"[model_name]"` | | |
| *Generate species and reactions by iterative application of rules, starting from the seed species* | | `suffix=>"string"` | Suffix added to output base filename. | `""` | | |
| | | `verbose=>0/1` | Verbose output. | `0` | | |
| | | `overwrite=>0/1` | Overwrite existing NET file. | `0` | | |
| | | `print_iter=>0/1` | Write NET file at every iteration. | `0` | | |
| | | `max_agg=>int` | Max # of molecules in a complex. | `1e9` | | |
| | | `max_iter=>int` | Max # of iterations of rule application. | `100` | | |
| | | `max_stoich=>{"string"=>int,"string"=>int,...}` | Max # of molecules of specified type(s) in a complex. | `undef` | | |
| | | `TextReaction=>0/1` | Write reactions with BNG-style species strings (human readable). | `0` | | |
| | | `TextSpecies=>0/1` | Write BNG-style species strings (human readable). | `1` | | |
| --- | --- | --- | --- | --- | --- | --- |