import os
if __name__ == '__main__':
	os.system("rm ../stats/stats_delta_fixed.txt");
	inputfile = ["cor", "ant", "ind"]
	binaries = ["twoPassDeltaQuery.out", "indexedDeltaQuery.out"]
	kdim = ["5", "10", "15", "20"]
	for d in kdim:
		for filename in inputfile:
			for binary in binaries:
				print binary, filename, d
				argfile = open('param.txt', 'w')
				fname = filename + d + ".txt"
				argfile.write("query ../dataset_delta_query/"+fname+" "+ "output.txt")
				argfile.close()
				argfile = open('query', 'w')
				argfile.write(d+"\n")
				argfile.write("400")
				argfile.close()
				os.system("./../delta_query_source/" + binary + " < param.txt >> ../stats/stats_delta_fixed.txt")
	os.system("rm param.txt output.txt query")