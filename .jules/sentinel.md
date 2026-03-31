## 2024-05-24 - Arbitrary Code Execution and Shell Injection
**Vulnerability:** Arbitrary code execution via `eval` in command line argument parsing (`bng2/Perl2/Console.pm`) and shell injection risk via `hostname` backticks in simulator execution (`bng2/Perl2/BNGAction.pm`).
**Learning:** Perl's `eval` was used to quickly parse array blocks of strings on the command line, which would allow arbitrary Perl code execution. Backticks for system commands like `hostname` allow potential shell injection or environment manipulation.
**Prevention:** Use regex or dedicated parsers for string cleanup instead of `eval`, and use standard core Perl modules like `Sys::Hostname` instead of shelling out for system properties.
