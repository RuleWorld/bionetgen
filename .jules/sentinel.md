## 2026-05-16 - [Dynamic Path Resolution and XML Patching]
**Vulnerability:** Python scripts using hardcoded execution paths (`/usr/bin/killall`) and unpatched `xmlrpclib` vulnerable to XML attacks (Billion laughs).
**Learning:** Hardcoded paths break cross-environment compatibility and cause tests to fail on systems without the binary in that specific location, while `xmlrpclib` is inherently insecure against entity expansion.
**Prevention:** Use `shutil.which` to dynamically locate executables and ensure test cases mock `shutil.which`. Always apply `defusedxml.xmlrpc.monkey_patch()` wrapped in an `ImportError` block for legacy XML parsers.
