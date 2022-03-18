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
int x, y;
	char sym;
    int fullLen = 0;
	map<char, int> Symbols;														
	In.read((char*) & x, sizeof(x));	
    while (x > 0){
		In.read((char*) & sym, sizeof(sym));	
		In.read((char*) & y, sizeof(y));	
        fullLen += y;
		x--;
		Symbols[sym] = y;
    }

    list<Interval> table = CreateTab(Symbols);
    list<Interval>::iterator it;
    int low = 0, 
        high = 65535, 
        divisor = fullLen; 
    int qtr1 = (high + 1)/4, 
        qtr2 = qtr1*2, 
        qtr3 = qtr1*3;
    int value = (In.get() << 8) | In.get();
    char c = In.get();
    count = 0;
    while(fullLen){
        int freq = ((value - low + 1)*divisor - 1)/(high - low + 1);
        for(it = table.begin(); it->right <= freq; it++);
        int l = low;
        low = low + (it->left)*(high - low + 1)/divisor;
        high = l + (it->right)*(high - l + 1)/divisor - 1;
        while(1){
            if(high >= qtr2)
		if(low >= qtr2){
                    low -= qtr2; 
                    high -= qtr2; 
                    value -= qtr2;
                }else 
		    if((low >= qtr1) && (high < qtr3)){
                        low -= qtr1; 
                        high -= qtr1; 
                        value -= qtr1;
                    }else 
			break;
            low += low; 
            high += high + 1;
            value += value + (((short)c >> (7 - count)) & 1);
            count ++;
            if(count == 8){
                c = In.get();
                count = 0;
            }
        }
        Out << it->sym;
        fullLen--;
    }
    In.close();
    Out.close();
	
    return 0;
}
