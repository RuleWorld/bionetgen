import os

filepath = 'src/io/PythonExportWriter.cpp'
with open(filepath, 'r') as f:
    content = f.read()

old_code = r"join('{:<18s}'.format('time') if i == 0 else ['{:<18s}'.format(species_names[i-1]) for i in range(N_SPECIES + 1)])"
new_code = r"join(['{:<18s}'.format('time') if i == 0 else '{:<18s}'.format(species_names[i-1]) for i in range(N_SPECIES + 1)])"

if old_code in content:
    content = content.replace(old_code, new_code)
    with open(filepath, 'w') as f:
        f.write(content)
    print("Fixed PythonExportWriter.cpp")
else:
    print("Could not find code to fix in PythonExportWriter.cpp")
