## 2024-10-24 - Avoid std::regex in hot paths
**Learning:** Using `std::regex` inside hot loop paths incurs massive overhead. Even pre-compiling `std::regex` objects does not avoid the heap allocations triggered by `std::smatch` and `std::regex_search`.
**Action:** Replace `std::regex` with native string operations (e.g. `std::string::find`, `substr`, `std::isdigit`) in performance-critical C++ parsing code.

## 2024-10-24 - Avoid std::transform with std::tolower
**Learning:** `std::transform` paired with `std::tolower` relies on locale checking, which is unnecessarily slow for simple ASCII string conversion.
**Action:** Replace it with an inline loop modifying characters manually (`if (c >= 'A' && c <= 'Z') c += ('a' - 'A')`).
