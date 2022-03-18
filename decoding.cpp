#include <iostream> 
#include <fstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

struct Interval{
    char sym;
    int key;
    int left;
    int right;
};

struct Check{
	bool operator() (Interval left, Interval right){
		return left.key > right.key;
	}
};

list<Interval> CreateTab(map<char, int> Symbols){
    list<Interval> List;
    for(const auto& x : Symbols){
        Interval p;
        p.sym = x.first;
        p.key = x.second;
        List.push_back(p);
    }
    List.sort(Check());
    List.begin()->right = List.begin()->key;
    List.begin()->left = 0;
    list<Interval>::iterator it1 = List.begin(), it2;
    it2 = it1;
    it1++;
    for(; it1 != List.end(); it1++){
        it1->left = it2->right;
        it1->right = it1->left + it1->key;
        it2++;
    }
    return List;
}

int main(){
    int count = 0;
    ifstream In("B.txt", ios::binary);
	if (!In.is_open()){
        cout << "error";
        return 0;
    }
	ofstream Out("C.txt", ios::binary);
	if (!Out.is_open()){
        cout << "error";
        return 0;
    }
