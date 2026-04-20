## 2026-04-19 - Fix insecure temporary file deletion in parsers/ContactMap/server.py
**Vulnerability:** Command-line string usage relying on wildcard ('*') which does not expand natively through 'subprocess.call' (causing residual temporary files that could leak local details or fill disk space if heavily used, or potentially conceal a shell injection hazard if it was "fixed" simply by adding 'shell=True').
**Learning:** When removing temporary files using wildcard patterns in Python scripts, avoid insecure and non-functional 'subprocess.call' with 'rm' (which requires risky 'shell=True' for wildcard expansion). Instead, use 'glob.glob' to match files and iterate over them with 'os.remove()', wrapping the deletion in a 'try...except OSError:' block.
**Prevention:** In Python applications, consistently manage temporary file cleanup natively using the 'os', 'shutil', 'glob' or 'tempfile' modules instead of invoking OS shell commands.

## 2024-05-24 - Fix XXE in BNGXML parsers
**Vulnerability:** XML External Entity (XXE) vulnerability in `lxml.etree.parse` where the parser resolves external entities by default. Attackers can provide malicious XMLs leading to arbitrary file disclosure or SSRF.
**Learning:** Always harden `lxml.etree` parsers, especially when dealing with external model definitions like SBML/BNG XML.
**Prevention:** Use `parser = etree.XMLParser(resolve_entities=False)` explicitly when invoking `etree.parse()`. Update mocked tests to reflect the new keyword argument using `parser=unittest.mock.ANY`.
