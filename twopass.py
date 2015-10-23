import os

kdominating = []
notkdominating = []

def kdominate(obj1, obj2, k):
	dim = len(obj1)

	greater = False
	kcount = 0
	for index in range(1,dim):
		if obj1[index] > obj2[index] :
			greater = True
		if obj1[index] >= obj2[index]:
			kcount += 1 

	if greater and kcount >= k :
		return True
	else :
		return False

def checkdominating(obj, k):
	global kdominating
	global notkdominating
	isDominating = True

	for possibleKdominating in kdominating[:]:
		if kdominate(possibleKdominating, obj, k):
			isDominating = False
		if kdominate(obj, possibleKdominating, k):
			kdominating.remove(possibleKdominating)
			notkdominating.append(possibleKdominating)
	if(isDominating):
		kdominating.append(obj)
	else:
		notkdominating.append(obj)

def removeFalsePositive(k):
	global kdominating
	global notkdominating

	for obj in notkdominating:
		for possibleKdominating in kdominating[:]:
			if obj[0] < possibleKdominating[0]:
				if kdominate(obj, possibleKdominating, k):
					kdominating.remove(possibleKdominating)

def outputToFile(outfilename):
	global kdominating

	kdominatingIdx = []
	for item in kdominating:
		kdominatingIdx.append( int(item[0]))

	print kdominatingIdx
	print len(kdominatingIdx)
def twopass(infilename, outfilename, k):
	inputfile = open(infilename, 'r')
	for line in inputfile:
		obj = line.rstrip().lstrip().split('\t')
		obj = map(float, obj)
		checkdominating(obj, k)

	removeFalsePositive(k)
	outputToFile(outfilename)

if __name__ == '__main__':
	infilename = 'sample.txt'
	outfilename = 'output.txt'
	k = 6
	twopass(infilename, outfilename,  k)