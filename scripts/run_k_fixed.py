import os
if __name__ == '__main__':
	inputfile = ["cor", "ant", "ind"]
	binaries = ["twopass.out","indexedTwoPass.out","onepass.out"]
	sizes = ["1000", "5000", "10000", "50000","100000"]
	kparam = ["8"]
	for k in kparam:
		os.system("rm ../stats/stats_k_fixed_"+k+".txt")
		for size in sizes:
			for filename in inputfile:
				for binary in binaries:
					fname = filename + size + ".txt"
					print k, size, fname, binary
					if binary == "indexedTwoPass2.out":
						argfile = open('param.txt', 'w')
						argfile.write("query "+ "../dataset_k_dominating_query/" +fname+" "+ "output.txt")
						argfile.close()
						argfile = open('query', 'w')
						argfile.write("10\n")
						argfile.write(k)
						argfile.close()
						os.system("./../k_dominating_source/"+binary + " < param.txt >> ../stats/stats_k_fixed_" + k +".txt")	
					else:
						argfile = open('param.txt', 'w')
						argfile.write("../dataset_k_dominating_query/"+fname+" "+ "output.txt" + " " + k + " 10")
						argfile.close()
						os.system("./../k_dominating_source/"+ binary + " < param.txt >> ../stats/stats_k_fixed_"+ k + ".txt")
	os.system("rm param.txt output.txt query")