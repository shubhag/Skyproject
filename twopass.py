import os
import time

kdominating = []
notkdominating = []
comparisons = 0

def kdominate(obj1, obj2, k):
	global comparisons
	dim = len(obj1)
	comparisons += 1
	greater = False
	kcount = 0


	for index in range(1,dim):
		if obj1[index] < obj2[index] :
			greater = True
		if obj1[index] <= obj2[index]:
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
	
def twopass(infilename, outfilename, k):
	inputfile = open(infilename, 'r')
	for line in inputfile:
		obj = line.rstrip().lstrip().split('\t')
		obj = map(float, obj)
		checkdominating(obj, k)

	removeFalsePositive(k)

if __name__ == '__main__':
	startTime = time.time()
	
	infilename = 'sample_ind.txt'
	outfilename = 'output_2pass.txt'
	k = 4
	twopass(infilename, outfilename,  k)

	global kdominating

	kdominatingIdx = []
	for item in kdominating:
		kdominatingIdx.append( int(item[0]))

	endTime = time.time()
	outfile = open(outfilename, 'w')
	outfile.write("Total running time: "+ str(endTime - startTime) + " sec\n")
	outfile.write("Comparisons: "+ str(comparisons)+"\n")
	outfile.write("Size of k-dominating skyline set: "+str(len(kdominatingIdx)) + "\n")
	outfile.write("Ids of the skyline objects: \n")
	outfile.write(str(kdominatingIdx))
	outfile.write("\n")
	outfile.close()