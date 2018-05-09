import MM
from MM import *
a = new_doubleArray(5)
params =  [-2.77, -1, -2, 6.02, 602]
for i in range(0,5):
	doubleArray_setitem(a,i,params[i])
c = bng_protocol(a)
d = doubleArrayClass_frompointer(c)
for i in range(0,55):
	print(d[i])
	if i+1%5==0:
		print("\n")





