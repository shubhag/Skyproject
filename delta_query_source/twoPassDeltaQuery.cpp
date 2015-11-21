#include <bits/stdc++.h>
using namespace std;

typedef std::numeric_limits< double > dbl;

struct comparator {
  bool operator() (const pair<double,int>& a, const pair<double,int>& b) const
  {return (a.first<b.first||(a.first==b.first&&a.second<b.second));}
};

struct Point {
	int id;
	vector <double> attr;
	bool isSkyline;
	double t;
};

class Stats {
private:
	vector<long long> runningTime;
	vector<long long> comparisons;
	int totalPoints;
	int totalDims;
	int skylines;
	int itrs;
	int K;
public:
	set<int> skyIds;
	Stats() {
		runningTime.clear();
		comparisons.clear();
		skylines = 0;
		itrs = 0;
		totalPoints = 0;
		totalDims = 0;
		skyIds.clear();
	}

	void reinitialize() {
		skylines = 0;
		skyIds.clear();
		runningTime.push_back(0);
		comparisons.push_back(0);
	}

	void setSkylines(int a) {
		skylines = a;
	}

	void setRunningTime(long long a) {
		runningTime[itrs-1] = a;
	}

	void setComparisons(long long a) {
		comparisons[itrs-1] = a;
	}

	void setDimensions(int a) {
		totalDims = a;
	}

	void incrementIterations() {
		itrs++;
	}
	void setDataSize(int a) {
		totalPoints = a;
	}

	void setK(int k) {
		K = k;
	}

	int getSkylines() {
		return skylines;
	}

	long long getRunningTime(int it) {
		if(it==0)
			return runningTime[itrs-1];
		else
			return runningTime[it-1];
	}

	long long getComparisons(int it) {
		if(it==0)
			return comparisons[itrs-1];
		else
			return comparisons[it-1];
	}

	int getDimensions() {
		return totalDims;
	}

	int getDataSize() {
		return totalPoints;
	}

	void insertSkyline(int id) {
		skyIds.insert(id);
	}

	void printStats(){
		long long totalComps=0,totalRT = 0;
		for(int i=0;i<itrs;i++) {
			totalComps+=comparisons[i];
			totalRT+=runningTime[i];
		}
		cout <<  (1000*(double)totalRT)/CLOCKS_PER_SEC << " " << totalComps << " " << skylines << " " << K << endl;
	}
};

class Query {
public:
	int D,K;
	vector <int> dim;
	int Delta;

	Query() {
		D = K = Delta = 0;
		dim.clear();
	}

	void setQuery(string queryFile) {
		ifstream is(queryFile);
		assert(is.is_open());
		
		string dimensions;
		int temp,i=0;	
		while(getline(is,dimensions)) {
			istringstream iss(dimensions);
			if(i==0) {
				// while(!iss.eof()) {
				// 	iss >> temp;
				// 	dim.push_back(temp-1);
				// }
				// sort(dim.begin(),dim.end());
				// D = (int)dim.size();
				iss >> D;
				for(int i=0;i<D;i++)
					dim.push_back(i);
				i++;
			}
			else {
				iss >> Delta; 
			}
		}
		
		is.close();
	}
};

class MinList {
public:
	set<pair<double,int>,comparator > myList;
	int key;
	
	void initialize(vector<Point>& data,set<int>& entries,int ind) {
		key = ind;
		myList.clear();
		pair<double,int> x;
		for(set<int>::iterator i=entries.begin();i!=entries.end();i++) {
			insertIntoMinList(data[*i]);
		}
	}

	void initialize(int ind) {
		myList.clear();
		key = ind;
	}

	void insertIntoMinList(Point& data) {
		pair<double,int> x;
		x.first = data.t;
		x.second = data.id;
		myList.insert(x);
	}

	void insertIntoMinList(double k,int id) {
		myList.insert(make_pair(k,id));
	}

	set<pair<double,int>,comparator>::iterator lowerBound(pair<double,int> a) {
		return myList.lower_bound(a);
	}

	pair<double,int> getNext(set<pair<double,int>,comparator>::iterator it) {
		return *(++it);
	}

	pair<double,int> extractNext(set<pair<double,int>,comparator>::iterator it) {
		pair<double,int> p = *(++it);
		it--;
		myList.erase(it);
		return p;
	}
};


//Global variables required
Stats S; // The stats class storing running time, number of comparisons, number of skylines
Query Q; // Query class stores the dimensions on which query has been made
vector<Point> Data;  // Struct Point {id,attr} stores the id of the point and the query attribute values(not all attributes)

// MinList P;
vector<int> P;
vector<int> T;
vector<int> R;
set<int> definiteSkylines,D_Prime;
static double timestamp;

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
	S.reinitialize();
	
	string line;
	Point pt;
	int dimCount = 0;
	bool isFirstPoint = true;
	double value;
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
	S.setComparisons(S.getComparisons(0)+1);
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
void validateSkylines(int n) {
	if(Data[n].isSkyline==false)
		return;
	// set<pair<double,int>,comparator>::iterator it,x;
	// x = P.lowerBound(make_pair(Data[n].t,-1));
	vector<int>::iterator it;
	for(it=P.begin();it!=P.end();it++) {
		if(Data[*it].t>Data[n].t)
			continue;
		int c = comparePoints(n,*it);
		if(c>1)
			break;
	}
	if(it==P.end())
		S.insertSkyline(n);
	else
		Data[n].isSkyline=false;
}

bool verifyAgainstDefSky(int k) {
	for(set<int>::iterator i=definiteSkylines.begin();i!=definiteSkylines.end();i++) {
		int c = comparePoints(k,*i);
		if(c&1)
			assert(false);
		else if(c==2) {
			return false;
		}
	}
	return true;
}

void reinitializeInput() {
	T.clear();
	for(set<int>::iterator i = D_Prime.begin();i!=D_Prime.end();i++)
		T.push_back(*i);
}

void twoPass() {
	timestamp = 0.0;
	reinitializeInput();
	R.clear();
	for(int i=0;i<(int)T.size();i++) {
		Data[T[i]].t = timestamp++;
		Data[T[i]].isSkyline = true;
		for(vector<int>::iterator j=R.begin();j!=R.end();) {
			int c = comparePoints(T[i],*j);
			if(c&1) {
				// P.insertIntoMinList(Data[*j]);
				Data[*j].isSkyline=false;
				P.push_back(*j);
				j = R.erase(j);
			}
			else
				j++;
			if(c>1)
				Data[T[i]].isSkyline=false;
		}
		if(Data[T[i]].isSkyline) {
			bool f = verifyAgainstDefSky(T[i]);
			if(!f)
				Data[T[i]].isSkyline = false;
		}
		if(Data[T[i]].isSkyline)	
			R.push_back(T[i]);
		else {
			P.push_back(T[i]);
			// P.insertIntoMinList(Data[T[i]]);
		}
	}

	for(vector<int>::iterator j=R.begin();j!=R.end();j++) {
		validateSkylines(*j);
	}
}

void resetP() {
	// P.initialize(0);
	P.clear();
	for(int i=0;i<Data.size();i++)
		if(S.skyIds.find(i)==S.skyIds.end() && definiteSkylines.find(i)==definiteSkylines.end()) {
			// P.insertIntoMinList(-1.0,i);
			P.push_back(i);
			Data[i].t = -1.0;
		}
}

int main() {
	S = Stats();
	string queryFile,data,output;
	cin >> queryFile >> data >> output;  
	
	Q.setQuery(queryFile);
	
	inputData(data);
	// P.initialize(0);
	P.clear();

	int Left = 1,Right = Q.D,Mid;
	for(int i=0;i<Data.size();i++)
		D_Prime.insert(i);

	while(Left<Right) {
		S.incrementIterations();
		S.reinitialize();
		Mid = (Left+Right)/2;
		Q.K = Mid;
		clock_t a = clock();
		twoPass();

		if(definiteSkylines.size()+S.skyIds.size()>=Q.Delta) {
			resetP();
			D_Prime.clear();
			for(set<int>::iterator i = S.skyIds.begin();i!=S.skyIds.end();i++) {
				D_Prime.insert(*i);
			}
			Right = Mid;
		}
		else {
			for(set<int>::iterator i = S.skyIds.begin();i!=S.skyIds.end();i++) {
				definiteSkylines.insert(*i);
				D_Prime.erase(*i);
			}
			//Remove points entered in this iteration from P
			/*
			for(set<int>::iterator i = D_Prime.begin();i!=D_Prime.end();i++) {
				P.myList.erase(make_pair(Data[*i].t,*i));
			}
			*/
			while(P.size()>0 && D_Prime.find(P.back())!=D_Prime.end())
				P.pop_back();
			Left=Mid+1;
		}
		clock_t b = clock();
		S.setRunningTime(b-a);
	}

	for(set<int>::iterator i = S.skyIds.begin();i!=S.skyIds.end();i++) {
		definiteSkylines.insert(*i);
	}
	
	if(definiteSkylines.size()<Q.Delta) {
		S.incrementIterations();
		S.reinitialize();
		Q.K = Left;
		clock_t a = clock();
		twoPass();
		for(set<int>::iterator i = S.skyIds.begin();i!=S.skyIds.end();i++) {
			definiteSkylines.insert(*i);
		}
		clock_t b = clock();
		S.setRunningTime(b-a);
	}
	S.setSkylines(definiteSkylines.size());
	S.setK(Left);
	cout << "DeltaTwoPass\t" << data << "\t" << Q.Delta << "\t" ;  
	S.printStats();
	
	ofstream out(output);
	assert(out.is_open());
	out.precision(dbl::digits10);
	for(set<int>::iterator i = S.skyIds.begin();i!=S.skyIds.end();i++) {
		out << *i+1 << " ";
		for(int j=0;j<(int)Data[*i].attr.size();j++)
			out << scientific <<Data[*i].attr[j] << " ";
		out << endl;
	}
	return 0;
}