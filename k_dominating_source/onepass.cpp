#include <bits/stdc++.h> 
using namespace std;
vector<vector<double> > Data;
vector<int> kdominating,notkdominating;
int kparam, dimension;
long long comparisons = 0;

int comparePoints(int i, int j,int m) {
	int a=0,b=0,c=0;
	comparisons++;
	for(int k=0;k<dimension;k++) {
		if(Data[i][k]<Data[j][k])
			a++;
		else if(Data[j][k]<Data[i][k])
			b++;
		else
			c++;
	}
	int res=0;
	if(a && a+c>=m)
		res++;
	if(b && b+c>=m)
		res+=2;
	return res;
}

bool checkSame(int i, int j){
	for(int index=0; index<dimension; index++){
		if(Data[i][index]!=Data[j][index]) return false;
	}
	return true;
}

void processPoint(int i) {
	bool isUniqueSkyline = true;
	bool pruning = false;
	vector<int>::iterator it;
	for(it=notkdominating.begin();it!=notkdominating.end(); ){
		bool flag = true;
		int c = comparePoints(i,*it,dimension);
		if(c==1){
			it = notkdominating.erase(it);
			flag = false;
		}
		else if(c==2){
			isUniqueSkyline = false;
			break;
		} 

		c = comparePoints(i,*it,kparam);
		if(c>1){
			pruning = true;
		}
		if(flag) ++it;
	}

	if(isUniqueSkyline){
		bool isDominant = true;
		vector<int>::iterator it;
		for(it=kdominating.begin(); it!=kdominating.end();){
			int c = comparePoints(i,*it,kparam);
			if(c>1){
				isDominant = false;
			}
			if(c&1){
				notkdominating.push_back(*it);
				it = kdominating.erase(it);
			} else{
				++it;
			}
		}
		if(!pruning && isDominant){
			kdominating.push_back(i);
		} else{
			notkdominating.push_back(i);
		}
	}
}

void onepass(string infile){
	ifstream file(infile.c_str());
	assert(file.is_open());
	string line;
	while (getline(file, line)){
	    vector<double> obj;
		istringstream iss(line);
	    double a;
	    iss >> a;
	    for(int i=0;i< dimension;i++){
			iss >> a;
			obj.push_back(a);
		}
		Data.push_back(obj);
		processPoint((int)Data.size()-1);
	}
}

int main(int argc, char** argv){
	string infilename;
	string outfilename;
	cin >> infilename >> outfilename >> kparam >> dimension;
	clock_t t1,t2;
	kdominating.clear();
	notkdominating.clear();
    t1=clock();
	onepass(infilename);
	t2=clock();
    double diff ((double)t2-(double)t1);

	sort(kdominating.begin(), kdominating.end());
	ofstream myfile;
	myfile.open(outfilename.c_str());
	myfile << "Time taken : " << (1000*(double)diff)/CLOCKS_PER_SEC << " seconds" <<endl;
	myfile << "Comparisons : " << comparisons << endl;
	myfile << "Size of k-dominating skyline set: " << kdominating.size() << endl;

	cout << "Onepass\t" << infilename << "\t" << kparam << "\t" << (1000*(double)diff)/CLOCKS_PER_SEC <<"\t" << comparisons << "\t" << kdominating.size() << endl;
	vector<int>::iterator iter;
	for(iter=kdominating.begin(); iter!=kdominating.end(); ++iter){
		myfile << *iter << ", ";
	}
	myfile << endl;
	myfile.close();
	return 0;
}