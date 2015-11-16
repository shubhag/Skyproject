import os
if __name__ == '__main__':
	inputfile = ["sample_cor.txt", "sample_ant.txt", "sample_ind.txt"]
	binaries = ["./onepass.out", "./twopass.out", "./indexedTwoPass.out"]
	for kparam in range(1,6):
		for filename in inputfile:
			for binary in binaries:
				print kparam, filename, binary
				if binary == "./indexedTwoPass.out":
					argfile = open('param.txt', 'w')
					argfile.write("query "+filename+" "+ "output.out")
					argfile.close()
					argfile = open('query', 'w')
					argfile.write("1 2 3 4 5\n")
					argfile.write(str(kparam))
					argfile.close()
					os.system(binary + " < param.txt >> stats.txt")	
				else:
					argfile = open('param.txt', 'w')
					argfile.write(filename+" "+ "output.out" + " " + str(kparam) + " 5")
					argfile.close()
					os.system(binary + " < param.txt >> stats.txt")	
					

	inputfile = ["correlated100000.out", "anticorrelated100000.out", "independent100000.out"]
	for kparam in range(1,11):
		for filename in inputfile:
			for binary in binaries:
				print kparam, filename, binary
				if binary == "./indexedTwoPass.out":
					argfile = open('param.txt', 'w')
					argfile.write("query "+filename+" "+ "output.out")
					argfile.close()
					argfile = open('query', 'w')
					argfile.write("1 2 3 4 5 6 7 8 9 10\n")
					argfile.write(str(kparam))
					argfile.close()
					os.system(binary + " < param.txt >> stats1.txt")	
				else:
					argfile = open('param.txt', 'w')
					argfile.write(filename+" "+ "output.out" + " " + str(kparam) + " 10")
					argfile.close()
					os.system(binary + " < param.txt >> stats1.txt")	