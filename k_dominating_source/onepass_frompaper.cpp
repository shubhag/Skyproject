#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream> 
#include <fstream> 
using namespace std;
vector< vector<double> > data;
vector< vector<double> > kdominating;
vector< vector<double> > notkdominating;
int k, dimension;
int comparisons = 0;
bool kdominate(vector<double> obj1, vector<double> obj2){
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

bool dominating(vector<double> obj1, vector<double> obj2){
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

bool checkSame(vector<double> obj1, vector<double> obj2){
	for(int index=1; index<=dimension; index++){
		if(obj1[index]!=obj2[index]) return false;
	}
	return true;
}

bool boolcmp(vector<double> const& v1, vector<double> const& v2){
	double sum1 = 0;
	double sum2 = 0;
	int i;
	for(i=1; i<=dimension; i++){
		sum1 += v1[i];
	}
	for(i=1; i<=dimension; i++){
		sum2 += v2[i];
	}
	return sum1 > sum2 ;
}

// void print(vector<double> data){
// 	int i;
// 	double sum = 0;
// 	for(i=1; i<=dimension; i++){
// 		cout << data[i] << " ";
// 		sum += data[i];
// 	}
// 	cout << endl << sum << endl;

// }

void onepass(string infile){
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
		data.push_back(obj);
	}
	sort(data.begin(), data.end(), boolcmp);
	vector< vector<double> >::iterator zit;
	cout << data.size() << endl;
	for(zit=data.begin(); zit!=data.end(); ++zit){
		vector<double> obj = *zit;
		bool isUniqueSkyline = true;
		vector< vector<double> >::iterator it;
		for(it=notkdominating.begin();it!=notkdominating.end(); ){
			bool flag = true;
			if(dominating(obj, *it)){
				it = notkdominating.erase(it);
				flag = false;
			} else if(dominating(*it, obj)){
				isUniqueSkyline = false;
				break;
			} 
			if(flag) ++it;
		}
		if(isUniqueSkyline){
			bool isDominant = true;
			vector< vector<double> >::iterator it;
			for(it=kdominating.begin(); it!=kdominating.end();){
				if(kdominate(*it, obj)){
					isDominant = false;
				}
				if(kdominate(obj, *it)){
					notkdominating.push_back(*it);
					it = kdominating.erase(it);
				} else{
					++it;
				}
			}
			if(isDominant){
				kdominating.push_back(obj);
			} 
			else{
				notkdominating.push_back(obj);
			}
		}
	}
}

int main(){
	string infilename = "test.txt";
	string outfilename = "output_1pass_1.txt";
	clock_t t1,t2;
    t1=clock();
	kdominating.clear();
	notkdominating.clear();
	k = 3;
	dimension = 4;
	onepass(infilename);
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

	vector<int>::iterator iter;
	for(iter=result.begin(); iter!=result.end(); ++iter){
		myfile << *iter << ", ";
	}
	myfile << endl;
	myfile.close();
	return 0;
}