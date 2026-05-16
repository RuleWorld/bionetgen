## 2024-05-16 - O(N) Deep Clone Bottleneck in Perl Iterations
**Learning:** `dclone` inside a `foreach` loop to copy a large nested reference hash (`$speciesIdHash_ref`) when it only needs to be read from incurs massive unnecessary memory allocation and performance overhead (O(N) vs O(1)).
**Action:** When inspecting Perl performance code, explicitly look for `dclone` usage inside array iterations. If the cloned structure is strictly read-only, refactor it to pass references directly.
