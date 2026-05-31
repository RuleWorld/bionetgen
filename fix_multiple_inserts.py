import sys

def modify():
    with open('src/io/NetWriter.cpp', 'r') as f:
        content = f.read()

    # Remove all "Cache for parsed observable patterns..." lines and the following unordered_map line
    lines = content.split('\n')
    new_lines = []
    skip_next = False
    for i in range(len(lines)):
        if skip_next:
            skip_next = False
            continue
        if lines[i].strip() == "// Cache for parsed observable patterns to avoid re-parsing and re-building graphs":
            skip_next = True
        elif "parsedObservableCache;" in lines[i] and "std::unordered_map" in lines[i]:
            pass
        else:
            new_lines.append(lines[i])

    content = '\n'.join(new_lines)

    with open('src/io/NetWriter.cpp', 'w') as f:
        f.write(content)

modify()
