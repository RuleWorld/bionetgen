## 2024-05-14 - Optimize Bipartite Graph mapping intersection
**Learning:** Legacy Python scripts in this codebase (`parsers/BipartiteGraph/bpgMaps.py`) frequently use O(N*M) list comprehensions (e.g., `[x for x,y in map for z in list if y==z]`) to perform graph intersections. For mapping dictionaries and edges, this is terribly inefficient.
**Action:** Replace nested loops with O(1) membership testing by pre-computing a `set` (e.g., `z_set = set(list)` and `[x for x,y in map if y in z_set]`).
