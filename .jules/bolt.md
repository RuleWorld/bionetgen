## 2024-05-14 - Cache PatternGraphs in HybridModelGenerator
**What:** Added a `PatternGraph` cache (`std::unordered_map` with a `std::mutex`) to memoize the parsing of species and patterns within `isIsomorphic` and `countMatches` of `HybridModelGenerator`.
**Why:** Re-parsing the strings using ANTLR on every call within nested loops caused significant performance degradation.
**Impact:** Massive speedup (~584x per operation) to `generate_hybrid_model` execution time during BNGL network processing by circumventing redundant compilation overhead.
**Measurement:** The `isIsomorphic` benchmark improved from ~20.93ms to ~35us (a 584x speedup per execution).
