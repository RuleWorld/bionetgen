import sys
import re
def toString(molpattern):
	#Check length of dictionary. If 1, this is a molecule, if >1 , this is a complex
	n = len(molpattern);
	if n == 1:
		s = str(molpattern.keys()[0]);
		s = s+'('
		for i in molpattern[molpattern.keys()[0]]:
			if type(i) is dict:
				for k in i.keys():
					s = s+str(k);
					for j in i[k]:
						s = s+'~'+str(j)
				s = s+','
			else:
				s = s+i+','
		s = s[:-1]+')'
		return s
	elif n>1:
		s = ''
		flag = 0
		for i in molpattern.keys():
			if i != 'bonds':
				tmp = {i:molpattern[i]}
				s = s + toString(tmp)
				s = s+'.'
			if i == 'bonds':
				flag = 1
				bno = len(molpattern[i])
				counter = 0
				for b in molpattern[i]:
					counter = counter + 1
					for key in b.keys():
						bond = '!'+str(counter)
						nshift = len(bond)
						val = b[key]
						m = re.search(str(key)+'\(',s)
						i1 = m.span()
						m1 = re.search('\)',s[i1[0]:])
						i2 = m1.span()
						tmp1 = s[i1[0]:i2[0]+i1[0]]
						bl = re.search(val,tmp1).start()+i1[0]
						s = s[0:bl+1]+bond+s[bl+1:]
		s = s[:-1]
		return s
		if flag == 1:
			'Problem. No bonds listed in complex'
if __name__ == '__main__':
	#TESTING
	pattern1 = {'A':[{'a':[0,1]},'b']}
	pattern2 = {'A':[{'a':[0]},'b']}
	pattern3 = {'A':[{'a':[0]},'b'],'C':['c'],'bonds' : [{'A':'b','C':'c'}]}
	pattern4 = {'A':[{'a':[0]},'b','c'],'C':['c'],'D':['d','e'],'bonds' : [{'A':'b','C':'c'},{'A':'c','D':'d'}]}
	print toString(pattern1)
	print toString(pattern2)
	print toString(pattern3)
	print toString(pattern4)