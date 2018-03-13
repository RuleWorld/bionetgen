import example
from example import *
a = new_doubleArray(2)
doubleArray_setitem(a,0,1)
doubleArray_setitem(a,1,1)
c = bng_protocol(a)
d = doubleArrayClass_frompointer(c)
counter = 0
print d[3]




