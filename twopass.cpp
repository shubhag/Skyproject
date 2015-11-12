#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream> 
#include <fstream> 
using namespace std;
vector< vector<float> > kdominating;
vector< vector<float> > notkdominating;
int k, dimension;
int comparisons = 0;
bool kdominate(vector<float> obj1, vector<float> obj2){
	bool greater = false;
	comparisons += 1;
	int kcount = 0;
	for(int index=1; index<= dimension; index++){
		if(obj1[index] <= obj2[index]){
			if(obj1[index] < obj2[index]){
				greater = true;
			}
			kcount++;
		}
	}

	if(greater && kcount >= k){
		return true;
	} else{
		return false;
	}
}

void checkdominating(vector<float> obj){
	bool isDominating = true;
	vector< vector<float> >::iterator it;
	for(it=kdominating.begin(); it!=kdominating.end();){
		if(kdominate(*it, obj)){
			isDominating = false;
		}
		if(kdominate(obj, *it)){
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
				if(kdominate(obj1, obj2)){
					flag = true;
					cout << obj1[0] << " " << obj2[0] << endl;
					cout << obj1[1] << " " << obj1[2] << " " << obj1[3] << " " << obj1[4] << " " << obj1[5] <<endl;
					cout << obj2[1] << " " << obj2[2] << " " << obj2[3] << " " << obj2[4] << " " << obj2[5] <<endl;
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
	cout << kdominating.size() << endl;
	removeFalsePositive();
}

int main(){
	string infilename = "sample_ant.txt";
	string outfilename = "output_2pass.txt";
	clock_t t1,t2;
    t1=clock();
	kdominating.clear();
	notkdominating.clear();
	k = 5;
	dimension = 5;
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

	vector<int>::iterator iter;
	for(iter=result.begin(); iter!=result.end(); ++iter){
		myfile << *iter << ", ";
	}
	myfile << endl;
	myfile.close();
	return 0;
}