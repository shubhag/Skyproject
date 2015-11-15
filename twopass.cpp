#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream> 
#include <fstream> 
using namespace std;
vector< vector<float> > kdominating;
vector< vector<float> > notkdominating;
int kparam, dimension;
long long comparisons = 0;
int comparePoints(vector<float> obj1, vector<float> obj2) {
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
bool kdominate(vector<float> obj1, vector<float> obj2){
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

void checkdominating(vector<float> obj){
	bool isDominating = true;
	vector< vector<float> >::iterator it;
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
	vector< vector<float> >::iterator it;
	vector< vector<float> >::iterator kit;
	for(it=notkdominating.begin(); it!=notkdominating.end(); ++it){
		for(kit=kdominating.begin(); kit!=kdominating.end(); ){
			bool flag = false;
			vector<float> obj1 = *it;
			vector<float> obj2 = *kit;
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
	    vector<float> obj;
		istringstream iss(line);
	    float a;
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
    t1=clock();
	kdominating.clear();
	notkdominating.clear();
	// kparam = 4;
	// dimension = 5;
	twopass(infilename);
	t2=clock();
    float diff ((float)t2-(float)t1);

	vector< vector<float> >::iterator it;
	vector<int> result;
	for(it=kdominating.begin(); it!=kdominating.end(); ++it){
		std::vector<float> obj = *it;
		result.push_back(int (obj[0])); 
	}
	sort(result.begin(), result.end());
	ofstream myfile;
	myfile.open(outfilename.c_str());
	myfile << "Time taken : " << ((float)diff)/CLOCKS_PER_SEC << " seconds" <<endl;
	myfile << "Comparisons : " << comparisons << endl;
	myfile << "Size of k-dominating skyline set: " << result.size() << endl;

	cout << "Twopass\t" << infilename << "\t" << kparam << "\t" << ((float)diff)/CLOCKS_PER_SEC <<"\t" << comparisons << "\t" << result.size() << endl;
	
	vector<int>::iterator iter;
	for(iter=result.begin(); iter!=result.end(); ++iter){
		myfile << *iter << ", ";
	}
	myfile << endl;
	myfile.close();
	return 0;
}