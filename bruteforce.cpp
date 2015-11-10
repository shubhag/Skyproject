#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream> 
#include <fstream> 
using namespace std;
vector< vector<float> > kdominating;
vector< vector<float> > possible;
int k, dimension;
int comparisons = 0;
bool kdominate(vector<float> obj1, vector<float> obj2){
	bool greater = false;
	comparisons += 1;
	int kcount = 0;
	for(int index=1; index<= dimension; index++){
		if(obj1[index] < obj2[index]){
			greater = true;
		}
		if(obj1[index] <= obj2[index]){
			kcount++;
		}
	}

	if(greater && kcount >= k){
		return true;
	} else{
		return false;
	}
}

void onepass(string infile){
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
		possible.push_back(obj);
	}
	vector< vector<float> >::iterator it1;
	vector< vector<float> >::iterator it2;

	for(it1 = possible.begin(); it1!= possible.end(); ++it1){
		bool flag = true;
		vector<float> obj1 = *it1;
		for(it2 = possible.begin(); it2!= possible.end(); ++it2){
			vector<float> obj2 = *it2;
			if(obj1[0] != obj2[0]){
				if(kdominate(obj2, obj1)){
					flag = false;
					break;
				}
			}
		}
		if(flag){
			kdominating.push_back(obj1);
		}
	}
}

int main(){
	string infilename = "sample_ind.txt";
	string outfilename = "output_brutepass.txt";
	clock_t t1,t2;
    t1=clock();
	kdominating.clear();
	possible.clear();
	k = 4;
	dimension = 5;
	onepass(infilename);
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