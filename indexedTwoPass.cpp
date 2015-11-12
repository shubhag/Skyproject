#include <bits/stdc++.h>
#include "helper.h"
using namespace std;

//Global variables required
Stats S; // The stats class storing running time, number of comparisons, number of skylines
Query Q; // Query class stores the dimensions on which query has been made
vector<Point> Data;  // Struct Point {id,attr} stores the id of the point and the query attribute values(not all attributes)
typedef std::numeric_limits< double > dbl;
MinList T,R,P;

vector<double> extractSubset(vector<double>& in, vector<int>& indices) {
	vector<double> res;
	for(int i=0;i<(int)indices.size();i++)
		res.push_back(in[indices[i]]);
	return res;
}

//This takes data points as input and stores in the memory, returns number of points read
int inputData(string file) {
	ifstream inp(file);
	assert(inp.is_open());
	inp.precision(dbl::digits10);
	
	Data.clear();
	S.initialize();
	
	string line;
	Point pt;
	int dimCount = 0;
	bool isFirstPoint = true;
	double value;
	int ind = (Q.K<=Q.D/2?Q.D-Q.K+1:Q.K);
	while(getline(inp,line)) {
		istringstream iss(line);
		pt.isSkyline = true;
		iss >> pt.id;
		pt.id--;
		pt.attr.clear();
		while(!iss.eof()) {
			if(isFirstPoint) {
				dimCount++;
			}
			iss >> scientific >> value;
			pt.attr.push_back(value);
		}
		S.setDataSize(S.getDataSize()+1);
		pt.attr = extractSubset(pt.attr,Q.dim);
		vector<double> temp = pt.attr;
		partial_sort(temp.begin(),temp.begin()+ind,temp.end());
		pt.key1 = temp[0];
		pt.key2 = temp[ind-1];
		Data.push_back(pt);
		isFirstPoint = false;
	}
	S.setDimensions(dimCount);
}

int comparePoints(int i,int j) {
	int a=0,b=0,c=0;
	for(int k=0;k<Data[i].attr.size();k++) {
		if(Data[i].attr[k]<Data[j].attr[k])
			a++;
		else if(Data[j].attr[k]<Data[i].attr[k])
			b++;
		else
			c++;
	}
	S.setComparisons(S.getComparisons()+1);
	int res=0;
	if(a && a+c>=Q.K)
		res++;
	if(b && b+c>=Q.K)
		res+=2;
	return res;
}

void validateSkylines(set<pair<double,int> >::iterator e) {
	set<pair<double,int> >::iterator it,it2,x;
	vector<int> newlyPruned;
	for(it=R.myList.begin();it!=e;it++) {
		if(Data[it->second].isSkyline==false)
			continue;
		x = P.myList.lower_bound(make_pair(Data[it->second].key1,it->second));
		for(it2=P.myList.begin();it2!=x;it2++) {
			int c = comparePoints(it->second,it2->second);
			if(c==2 || c==3)
				break;
		}
		if(it2==x)
			S.insertSkyline((it->second));
		else
			newlyPruned.push_back(it->second);
		Data[it->second].isSkyline=false;
	}
	for(int i=0;i<(int)newlyPruned.size();i++)
		P.insertIntoMinList(Data[newlyPruned[i]].key1,newlyPruned[i]);
}

void indexedTwoPass() {
	set<pair<double,int> >::iterator i,j,k;
	for(i=T.myList.begin();i!=T.myList.end();i++) {
		// cout << i->first << endl;
		j = R.myList.lower_bound(make_pair(i->first,-1));
		if(j!=R.myList.end()) {
			// cout << i->second << " " << j->first << " " << j->second << endl;
			if(j!=R.myList.begin())
				Data[i->second].isSkyline=false;
		}
		validateSkylines(j);
		if(j==R.myList.end() && R.myList.size()>0)
			break;

		for(;j!=R.myList.end();j++) {
			if(Data[j->second].isSkyline==false)
				continue;
			int c = comparePoints(i->second,j->second);
			if(c&1) {
				P.insertIntoMinList(Data[j->second].key1,j->second);
				Data[j->second].isSkyline=false;
			}
			if(c>1)
				Data[i->second].isSkyline=false;
		}
		if(Data[i->second].isSkyline) {
			// cout << Data[i->second].key2  << " " << i->second << endl;
			R.insertIntoMinList(Data[i->second].key2,i->second);
		}
		else {
			P.insertIntoMinList(Data[i->second].key1,i->second);
		}
	}
	validateSkylines(R.myList.end());
}

int main() {
	S = Stats();
	string queryFile,data,output;
	cin >> queryFile >> data >> output;  
	Q.setQuery(queryFile);
	inputData(data);
	clock_t s = clock();
	//This constructs our index on the basis of min dimension
	T.initialize(Data,1);
	//This constructs our index on the basis of n-k+1_th or k_th min dimension based on whether K<=D/2 or K>D/2
	R.initialize((Q.K<=Q.D/2?Q.D-Q.K+1:Q.K));
	//This is the list of pruned out points sorted on the basis of time-stamp(in our case the min attribute value)
	P.initialize(1);
	
	indexedTwoPass();
	clock_t e = clock();
	S.setRunningTime(e-s);
	
	S.printStats();
	
	sort(S.skyIds.begin(),S.skyIds.end());
	ofstream out(output);
	assert(out.is_open());
	for(int i=0;i < S.skyIds.size();i++) {
		out << S.skyIds[i]+1 << " ";
		for(int j=0;j<(int)Data[S.skyIds[i]].attr.size();j++)
			out << Data[S.skyIds[i]].attr[j] << " ";
		out << endl;
	}
	
	return 0;
}