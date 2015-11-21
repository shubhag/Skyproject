import os
if __name__ == '__main__':
	os.system("rm ../stats/stats_delta_vary.txt")
	inputfile = ["cor20.txt", "ant20.txt", "ind20.txt"]
	binaries = ["twoPassDeltaQuery.out", "indexedDeltaQuery.out"]
	# delta = ["20", "50", "125", "300", "750", "1000", "2000", "3000"]
	delta = ["20", "40", "80", "160", "320", "640", "1280", "2560","5120"]
	for d in delta:
		for filename in inputfile:
			for binary in binaries:
				print d, filename, binary
				argfile = open('param.txt', 'w')
				argfile.write("query " + "../dataset_delta_query/" + filename + " output.txt")
				argfile.close()
				argfile = open('query', 'w')
				argfile.write("20\n")
				argfile.write(d)
				argfile.close()
				os.system("./../delta_query_source/" + binary + " < param.txt >> ../stats/stats_delta_vary.txt")
	os.system("rm param.txt output.txt query")