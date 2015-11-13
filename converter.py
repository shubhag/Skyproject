if __name__ == '__main__':
	infile = open("input.out", 'r')
	outfile = open("output.out", 'w')
	for line in infile:
		obj = line.rstrip().lstrip().split(',')
		outfile.write('\t'.join(obj))
		outfile.write('\n')
	infile.close()
	outfile.close()