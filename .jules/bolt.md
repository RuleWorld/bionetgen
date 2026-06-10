## 2026-06-08 - O(N) String Construction Over In-Place Replacement in C++ Export Writers
**Learning:** In C++ (specifically within BioNetGen export writers), repeatedly using `std::string::replace` in-place within a `while` loop to substitute parameters causes O(N*M) character shifting overhead, which severely degrades performance when generating models with long expressions or many parameters.
**Action:** Always allocate a new `std::string` using `reserve()` and construct the modified string using sequential `append()` operations. This reduces the time complexity of the replacements to O(N).
