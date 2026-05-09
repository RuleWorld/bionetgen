## 2024-05-24 - bpgMaps.py deepcopy performance issue
**Learning:** Python's `copy.deepcopy()` can be a significant performance bottleneck when called in a hot loop on custom objects, as seen in `TraceStack` expansion in `parsers/BipartiteGraph/bpgMaps.py`.
**Action:** Replace `copy.deepcopy()` with an explicit `.copy()` method on the class that manually copies only the necessary internal state (e.g., `list(self.trace)` and `set(self._set)`). This is far faster.
