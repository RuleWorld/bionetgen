import time
import os

from parsers.utils.smallStructures import Species

start = time.time()
for _ in range(100000):
    s = Species()
    s.molecules = []
    str(s)
end = time.time()
print(f"Time taken: {end - start:.4f}s")
