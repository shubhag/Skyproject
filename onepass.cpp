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
int comparisons = 0;
int comparePoints(vector<float> obj1, vector<float> obj2) {
	// comparisons += 1;
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
		if(obj1[index] < obj2[index]){
			greater = true;
		}
		if(obj1[index] <= obj2[index]){
			kcount++;
		}
	}

	if(greater && kcount >= kparam){
		return true;
	} else{
		return false;
	}
}

bool dominating(vector<float> obj1, vector<float> obj2){
	bool flag = false;
	for(int index=1; index<= dimension; ++index){
		if(obj1[index] < obj2[index])
			flag = true;
		else if(obj1[index] > obj2[index]){
			return false;
		}
	}
	return flag;
}

bool checkSame(vector<float> obj1, vector<float> obj2){
	for(int index=1; index<=dimension; index++){
		if(obj1[index]!=obj2[index]) return false;
	}
	return true;
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
		bool isUniqueSkyline = true;
		bool pruning = false;
		vector< vector<float> >::iterator it;
		for(it=notkdominating.begin();it!=notkdominating.end(); ){
			bool flag = true;
			comparisons += 1;
			if(dominating(obj, *it)){
				it = notkdominating.erase(it);
				flag = false;
			} else if(dominating(*it, obj)){
				isUniqueSkyline = false;
				break;
			} 
			if(comparePoints(*it, obj) == 1){
				pruning = true;
			}
			if(flag) ++it;
		}
		if(isUniqueSkyline){
			bool isDominant = true;
			vector< vector<float> >::iterator it;
			for(it=kdominating.begin(); it!=kdominating.end();){
				int c = comparePoints(*it, obj);
				comparisons += 1 ;
				// if(kdominate(*it, obj)){
				// 	isDominant = false;
				// }
				// if(kdominate(obj, *it)){
				// 	notkdominating.push_back(*it);
				// 	it = kdominating.erase(it);
				// } else{
				// 	++it;
				// }
				if(c&1){
					isDominant = false;
				}
				if(c>1){
					notkdominating.push_back(*it);
					it = kdominating.erase(it);
				} else{
					++it;
				}
			}
			if(!pruning && isDominant){
				kdominating.push_back(obj);
 			} else{
 				notkdominating.push_back(obj);
 			}
		}
	}
}

int main(){
	string infilename = "sample_ant.txt";
	string outfilename = "output_1pass.txt";
	clock_t t1,t2;
    t1=clock();
	kdominating.clear();
	notkdominating.clear();
	kparam = 5;
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