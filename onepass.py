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

def checkSame(obj1, obj2):
	for index in range(1, len(obj1)):
		if not obj1[index] == obj2[index]:
			return False
	return True

def onepass(infilename, outfilename, k):
	global kdominating
	global notkdominating

	inputfile = open(infilename, 'r')
	for line in inputfile:
		obj = line.rstrip().lstrip().split('\t')
		obj = map(float, obj)

		isUniqueSkyline = True

		for point in notkdominating[:]:
			if kdominate(obj, point, k):
				notkdominating.remove(point)
			elif kdominate(point, obj, k) or checkSame(point, obj) :
				isUniqueSkyline = False
				break

		if isUniqueSkyline:
			isDominant = True
			for point in kdominating[:]:
				if kdominate(point, obj, k):
					isDominant = False
				if kdominate(obj, point, k):
					kdominating.remove(point)
					notkdominating.append(point)

			if isDominant:
				kdominating.append(obj)
			else:
				notkdominating.append(obj)

if __name__ == '__main__':
	startTime = time.time()
	infilename = 'sample_ant.txt'
	outfilename = 'output_1pass.txt'
	k = 5
	onepass(infilename, outfilename,  k)
	
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