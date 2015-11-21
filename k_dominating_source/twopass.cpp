#include<bits/stdc++.h>
using namespace std;

vector<vector<double> > Data;
vector<int> kdominating, notkdominating;
int kparam, dimension;
long long comparisons = 0;

int comparePoints(int i, int j) {
	comparisons++;
	int a=0,b=0,c=0;
	for(int k=0;k<dimension;k++) {
		if(Data[i][k]<Data[j][k])
			a++;
		else if(Data[j][k]<Data[i][k])
			b++;
		else
			c++;
	}
	int res=0;
	if(a && a+c>=kparam)
		res++;
	if(b && b+c>=kparam)
		res+=2;
	return res;
}

void checkdominating(int i){
	bool isDominating = true;
	for(vector<int>::iterator it=kdominating.begin(); it!=kdominating.end();){
		int c = comparePoints(*it, i);
		if(c&1){
			isDominating = false;
		}
		if(c>1){
			notkdominating.push_back(*it);
			it = kdominating.erase(it);
		} else{
			++it;
		}
	}
	if(isDominating){
		kdominating.push_back(i);
	} else{
		notkdominating.push_back(i);
	}
}

void removeFalsePositive(){
	for(vector<int>::iterator it=notkdominating.begin(); it!=notkdominating.end(); ++it){
		for(vector<int>::iterator kit=kdominating.begin(); kit!=kdominating.end(); ){
			bool flag = false;
			if(*it < *kit){
				if(comparePoints(*kit, *it)>1){
					flag = true;
				}
			} 
			if(flag){
				kit = kdominating.erase(kit);
			}
			else{
				++kit;
			}
		}
	}
}

void twopass(string infile){
	ifstream file(infile.c_str());
	assert(file.is_open());
	string line;
	while (getline(file, line)){
	    vector<double> obj;
		istringstream iss(line);
	    double a;
	    iss >> a;
	    for(int i=0;i<dimension;i++){
			iss >> a;
			obj.push_back(a);
		}
		Data.push_back(obj);
		checkdominating((int)Data.size()-1);
	}
	removeFalsePositive();
}

int main(){
	string infilename;
	string outfilename;
	cin >> infilename >> outfilename >> kparam >> dimension;
	clock_t t1,t2;
	kdominating.clear();
	notkdominating.clear();
    t1=clock();
	twopass(infilename);
	t2=clock();
    double diff ((double)t2-(double)t1);

	sort(kdominating.begin(), kdominating.end());
	ofstream myfile;
	myfile.open(outfilename.c_str());
	myfile << "Time taken : " << (1000*(double)diff)/CLOCKS_PER_SEC << " milliseconds" <<endl;
	myfile << "Comparisons : " << comparisons << endl;
	myfile << "Size of k-dominating skyline set: " << kdominating.size() << endl;

	cout << "Twopass2\t" << infilename << "\t" << kparam << "\t" << (1000*(double)diff)/CLOCKS_PER_SEC <<"\t" << comparisons << "\t" << kdominating.size() << endl;
	
	vector<int>::iterator iter;
	for(iter=kdominating.begin(); iter!=kdominating.end(); ++iter){
		myfile << *iter << ", ";
	}
	myfile << endl;
	myfile.close();
	return 0;
}