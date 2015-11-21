#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream> 
#include <fstream> 
using namespace std;
vector< vector<double> > kdominating;
vector< vector<double> > notkdominating;
int kparam, dimension;
long long comparisons = 0;
int comparePoints(vector<double> obj1, vector<double> obj2) {
	comparisons += 1;
	int a=0,b=0,c=0;
	for(int k=1;k<=dimension;k++) {
		if(obj1[k]<obj2[k])
			a++;
		else if(obj2[k]<obj1[k])
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
bool kdominate(vector<double> obj1, vector<double> obj2){
	bool greater = false;
	int kcount = 0;
	for(int index=1; index<= dimension; index++){
		if(obj1[index] <= obj2[index]){
			if(obj1[index] < obj2[index]){
				greater = true;
			}
			kcount++;
		}
	}

	if(greater && kcount >= kparam){
		return true;
	} else{
		return false;
	}
}

void checkdominating(vector<double> obj){
	bool isDominating = true;
	vector< vector<double> >::iterator it;
	for(it=kdominating.begin(); it!=kdominating.end();){
		int c = comparePoints(*it, obj);
		// if(kdominate(*it, obj)){
		// 	isDominating = false;
		// }
		// if(kdominate(obj, *it)){
		// 	notkdominating.push_back(*it);
		// 	it = kdominating.erase(it);
		// } else{
		// 	++it;
		// }
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
		kdominating.push_back(obj);
	} else{
		notkdominating.push_back(obj);
	}
}

void removeFalsePositive(){
	vector< vector<double> >::iterator it;
	vector< vector<double> >::iterator kit;
	for(it=notkdominating.begin(); it!=notkdominating.end(); ++it){
		for(kit=kdominating.begin(); kit!=kdominating.end(); ){
			bool flag = false;
			vector<double> obj1 = *it;
			vector<double> obj2 = *kit;
			if(obj1[0] < obj2[0]){
				if(comparePoints(obj1, obj2)%2==1){
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
	string line;
	while (getline(file, line)){
	    vector<double> obj;
		istringstream iss(line);
	    double a;
	    for(int i=0;i<= dimension;i++){
			iss >> a;
			obj.push_back(a);
		}
		checkdominating(obj);
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

	vector< vector<double> >::iterator it;
	vector<int> result;
	for(it=kdominating.begin(); it!=kdominating.end(); ++it){
		std::vector<double> obj = *it;
		result.push_back(int (obj[0])); 
	}
	sort(result.begin(), result.end());
	ofstream myfile;
	myfile.open(outfilename.c_str());
	myfile << "Time taken : " << ((double)diff)/CLOCKS_PER_SEC << " seconds" <<endl;
	myfile << "Comparisons : " << comparisons << endl;
	myfile << "Size of k-dominating skyline set: " << result.size() << endl;

	cout << "Twopass\t" << infilename << "\t" << kparam << "\t" << (1000*(double)diff)/CLOCKS_PER_SEC <<"\t" << comparisons << "\t" << result.size() << endl;
	
	vector<int>::iterator iter;
	for(iter=result.begin(); iter!=result.end(); ++iter){
		myfile << *iter << ", ";
	}
	myfile << endl;
	myfile.close();
	return 0;
}