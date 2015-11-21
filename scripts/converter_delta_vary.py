import numpy as np

if __name__ == '__main__':
	infile = open('../stats/stats_delta_vary.txt', 'r')
	result = {}
	algorithms = ["IndexedDeltaTwoPass", "DeltaTwoPass"]
	files = ["../dataset_delta_query/ind20.txt","../dataset_delta_query/cor20.txt", "../dataset_delta_query/ant20.txt"]
	num = ["20", "40", "80", "160", "320", "640", "1280", "2560","5120"]
	for line in infile:
		values = line.strip().replace('\t',' ').replace(' ',' ').split()
		if not values[0] in result:
			result[values[0]] = {}
		if not values[1] in result[values[0]]:
			result[values[0]][values[1]] = {}
		result[values[0]][values[1]][values[2]] = {'c': values[4], 't': values[3]}

	# print result

	for j in range(0,3):
		s1 = open('data'+str(j+1)+'.dat', 'w')
		cur_file = files[j]
		for index in num:
			s1.write(index + "\t")
			a = {}
			for algo in algorithms:
				a[algo] = float(result[algo][cur_file][index]['c'])
				s1.write(result[algo][cur_file][index]['c'] + "\t")
			s1.write(str(a["IndexedDeltaTwoPass"]*100/a["DeltaTwoPass"]) + "\n")
		s1.close()
	for j in range(0,3):
		s1 = open('data'+str(j+4)+'.dat', 'w')
		cur_file = files[j]
		for index in num:
			s1.write(index + "\t")
			a = {}
			for algo in algorithms:
				a[algo] = float(result[algo][cur_file][index]['t'])
				s1.write(result[algo][cur_file][index]['t'] + "\t")
			s1.write(str(a["IndexedDeltaTwoPass"]*100/a["DeltaTwoPass"]) + "\n")
		s1.close()