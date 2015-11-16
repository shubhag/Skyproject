#include <bits/stdc++.h>
using namespace std;
#include "helper.h"

//Global variables required
Stats S; // The stats class storing running time, number of comparisons, number of skylines
Query Q; // Query class stores the dimensions on which query has been made
vector<Point> Data;  // Struct Point {id,attr} stores the id of the point and the query attribute values(not all attributes)
typedef std::numeric_limits< double > dbl;
MinList T,R,P;
static double timestamp = 0.0;

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

/*
- Compares points Data[i] and Data[j]
@return
0 - if none can dominate the other
1 - if only i dominates j
2 - if only j dominates i
3 - if both dominate each other
*/
int comparePoints(int i,int j) {
	int a=0,b=0,c=0;
	for(int k=0;k<Q.D;k++) {
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

/*
Checks out-going skylines for false positives by comparing them with already pruned out points. 
This is necessary to check against points which were pruned out even before the skyline point arrived.
*/
void validateSkylines(double e) {
	set<pair<double,int>,comparator>::iterator it,it2,x;
	for(it=R.myList.begin();it!=R.myList.end();) {
		if(it->first>=e)
			break;
		x = P.lowerBound(make_pair(Data[it->second].key3,-1));
		for(it2=P.myList.begin();it2!=x;it2++) {
			int c = comparePoints(it->second,it2->second);
			if(c==2 || c==3)
				break;
		}
		if(it2==x)
			S.insertSkyline(it->second);
		else
			Data[it->second].isSkyline=false;
		it = R.myList.erase(it);
	}
}

void indexedTwoPass() {
	set<pair<double,int>,comparator>::iterator i,j;
	double minKey2 = DBL_MAX;
	for(i=T.myList.begin();i!=T.myList.end();i++) {
		Data[i->second].key3 = timestamp++;
		
		if(i->first > minKey2)
			Data[i->second].isSkyline=false;
		minKey2 = min(minKey2,Data[i->second].key2);
		
		validateSkylines(i->first);
		if(R.myList.size()==0 && i->first > minKey2)
			break;
		for(j=R.myList.begin();j!=R.myList.end();) {
			int c = comparePoints(i->second,j->second);
			if(c&1) {
				// Data[j->second].key3 = timestamp++;
				P.insertIntoMinList(Data[j->second]);
				Data[j->second].isSkyline=false;
				j = R.myList.erase(j);
			}
			else
				j++;
			if(c>1)
				Data[i->second].isSkyline=false;
		}
		if(Data[i->second].isSkyline) {
			R.insertIntoMinList(Data[i->second]);
		}
		else {
			P.insertIntoMinList(Data[i->second]);
		}
	}
	validateSkylines(DBL_MAX);
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
	P.initialize(0);
	
	indexedTwoPass();
	clock_t e = clock();
	S.setRunningTime(e-s);
	
	cout << "IndexedTwoPass\t" << data << "\t" << Q.K << "\t" ;  
	S.printStats();
	
	sort(S.skyIds.begin(),S.skyIds.end());
	ofstream out(output);
	assert(out.is_open());
	out.precision(dbl::digits10);
	for(int i=0;i < S.skyIds.size();i++) {
		out << S.skyIds[i]+1 << " ";
		for(int j=0;j<(int)Data[S.skyIds[i]].attr.size();j++)
			out << scientific <<Data[S.skyIds[i]].attr[j] << " ";
		out << endl;
	}
	return 0;
}