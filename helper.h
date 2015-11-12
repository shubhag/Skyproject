#include <bits/stdc++.h>
using namespace std;

struct comparator {
  bool operator() (const pair<double,int>& a, const pair<double,int>& b) const
  {return (a.first<b.first||(a.first==b.first&&a.second<b.second));}
};


struct Point {
	int id;
	vector <double> attr;
	bool isSkyline;
	double key1,key2;
};

class Stats {
private:
	long long runningTime;
	long long comparisons;
	int totalPoints;
	int totalDims;
	int skylines;
public:
	vector<int> skyIds;
	Stats() {
		runningTime = comparisons = 0LL;
		skylines = 0;
		skyIds.clear();
	}

	void initialize() {
		setDimensions(0);
		setDataSize(0);
		setSkylines(0);
		setRunningTime(0);
		setComparisons(0);
		skyIds.clear();
	}

	void setSkylines(int a) {
		skylines = a;
	}

	void setRunningTime(long long a) {
		runningTime = a;
	}

	void setComparisons(long long a) {
		comparisons = a;
	}

	void setDimensions(int a) {
		totalDims = a;
	}

	void setDataSize(int a) {
		totalPoints = a;
	}

	int getSkylines() {
		return skylines;
	}

	long long getRunningTime() {
		return runningTime;
	}

	long long getComparisons() {
		return comparisons;
	}

	int getDimensions() {
		return totalDims;
	}

	int getDataSize() {
		return totalPoints;
	}

	void insertSkyline(int id) {
		skyIds.push_back(id);
		skylines++;
	}

	void printStats() {
		cout << "Total running time in terms of number of CPU clocks: " << runningTime << endl;
		cout << "Total running time in milliseconds: " << (1000*(double)runningTime)/CLOCKS_PER_SEC << endl;
		cout << "Number of comparisons: " << comparisons << endl;
		cout << "Number of skyline points: " << skylines << endl;
		cout << "Total points & dimensions: " << totalPoints << " & " << totalDims << endl;
	}
};

class Query {
public:
	int D,K;
	vector <int> dim;

	Query() {
		D = K = 0;
		dim.clear();
	}

	void setQuery(string queryFile) {
		ifstream is(queryFile);
		assert(is.is_open());
		
		string dimensions;
		int i = 0,temp;	
		while(getline(is,dimensions)) {
			istringstream iss(dimensions);
			if(i==0) {
				while(!iss.eof()) {
					iss >> temp;
					dim.push_back(temp-1);
				}
				sort(dim.begin(),dim.end());
				D = (int)dim.size();
				i++;
			}
			else if(i==1){
				iss >> K;
			}
		}
		is.close();
	}
};

class MinList {
public:
	set<pair<double,int>,comparator > myList;
	int key;
	void initialize(vector<Point>& data,int ind) {
		key = ind;
		myList.clear();
		pair<double,int> x;
		for(int i=0;i<(int)data.size();i++) {
			x.first = key==1?data[i].key1:data[i].key2;
			x.second = data[i].id;
			myList.insert(x);
		}
	}

	void initialize(int ind) {
		myList.clear();
		key = ind;
	}

	void insertIntoMinList(Point data) {
		pair<double,int> x;
		x.first = key==1?data.key1:data.key2;
		x.second = data.id;
		myList.insert(x);
	}

	void insertIntoMinList(double k,int id) {
		myList.insert(make_pair(k,id));
	}

	pair<double,int> getNext(set<pair<double,int> >::iterator it) {
		return *(++it);
	}

	pair<double,int> extractNext(set<pair<double,int> >::iterator it) {
		pair<double,int> p = *(++it);
		it--;
		myList.erase(it);
		return p;
	}
};
