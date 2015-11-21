import os
if __name__ == '__main__':
	os.system("rm ../stats/stats_k_vary.txt")
	inputfile = ["cor100000.txt", "ind100000.txt", "ant100000.txt"]
	binaries = ["twopass.out","indexedTwoPass.out","onepass.out"]
	for kparam in range(1,11):
		for filename in inputfile:
			for binary in binaries:
				print kparam, filename, binary
				if binary == "indexedTwoPass.out":
					argfile = open('param.txt', 'w')
					argfile.write("query "+ "../dataset_k_dominating_query/" +filename+" "+ "output.txt")
					argfile.close()
					argfile = open('query', 'w')
					argfile.write("10\n")
					argfile.write(str(kparam))
					argfile.close()
					os.system("./../k_dominating_source/"+binary + " < param.txt >> ../stats/stats_k_vary.txt")	
				else:
					argfile = open('param.txt', 'w')
					argfile.write("../dataset_k_dominating_query/"+filename+" "+ "output.txt" + " " + str(kparam) + " 10")
					argfile.close()
					os.system("./../k_dominating_source/"+ binary + " < param.txt >> ../stats/stats_k_vary.txt")
	os.system("rm param.txt output.txt query")