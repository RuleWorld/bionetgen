import timeit

setup = """
class Component:
    def __init__(self, name):
        self.name = name

components = [Component(str(i)) for i in range(100)]
"""

test_generator = "s = set(x.name for x in components)"
test_comprehension = "s = {x.name for x in components}"

time_generator = timeit.timeit(test_generator, setup=setup, number=100000)
time_comprehension = timeit.timeit(test_comprehension, setup=setup, number=100000)

print(f"Generator time: {time_generator:.5f} s")
print(f"Comprehension time: {time_comprehension:.5f} s")
print(f"Improvement: {(time_generator - time_comprehension) / time_generator * 100:.2f}%")
