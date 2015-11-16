import sys
if __name__ == '__main__':
	inputfilename = sys.argv[1]
	outputfilename = sys.argv[2]
	infile = open(inputfilename, 'r')
	outfile = open(outputfilename, 'w')
	for line in infile:
		obj = line.rstrip().lstrip().split(',')
		outfile.write('\t'.join(obj))
		outfile.write('\n')
	infile.close()
	outfile.close()