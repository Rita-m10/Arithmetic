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

list<Interval> CreateTab(const map<char, int>& Symbols){
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
    
    ifstream In("A.txt", ios::binary);
    if (!In.is_open()){
        cout << "error";
        return 0;
    }
    ofstream Out("B.txt", ios::binary);
    if (!Out.is_open()){
        cout << "error";
        return 0;
    }
    int count = 0;
    map<char, int> Symbols;
    char c;
    while(In.get(c)){
        Symbols[c]++;
        count++;
    }
	
    list<Interval> Tab = CreateTab(Symbols);
	
    //Печатаем в начало файла количество символов в таблице и пары символ-количество
    int size = Symbols.size();
    Out.write((char*)(&size), sizeof(size));
    for(const auto& x : Symbols){
        Out.write((char*)(&x.first), sizeof(x.first));
        Out.write((char*)(&x.second), sizeof(x.second));
    }
    In.clear();
    In.seekg(0);
    
    //
    int low = 0, 
        high = 65535, 
        i = 0, 
        div = count; 
    int qtr1 = (high + 1)/4, 
        qtr2 = qtr1*2, 
        qtr3 = qtr1*3, 
        bits_to_follow = 0;
		
    char tmp = 0;
    count = 0;
    char c;
    while(In.get(c)){
        i++;
        auto it1 = Tab.begin();
        for(; it1 != Tab.end() && c != it1->sym; it1++);
        int l = low;
        low = l + it1->left*(high - l + 1)/div;
        high = l + it1->right*(high - l + 1)/div - 1;
        
        while(1){
            if(high < qtr2){
                count++;
                if(count == 8){
                    Out << tmp;
		    count ^= count;
                    tmp ^= tmp;
                }
                for(; bits_to_follow > 0; bits_to_follow--){
                    tmp = tmp | (1 << (7 - count));
                    count++;
                    if(count == 8){
                        Out << tmp;
			count ^= count;
                        tmp ^= tmp;
                    }
                }
            } else 
		    if(low >= qtr2){
                    tmp = tmp | (1 << (7 - count));
                    count++;
                    if(count == 8){
                        Out << tmp;
			count ^= count;
                        tmp ^= tmp;
                    }                
                    for(; bits_to_follow > 0; bits_to_follow--){
                        count++;
                        if(count == 8){
                            Out << tmp;
			    count ^= count;
                            tmp ^= tmp;
                        }
                    }
                    low -= qtr2;
                    high -= qtr2;
                } else 
		    if(low >= qtr1 && high < qtr3){
                        bits_to_follow++;
                        low -= qtr1;
                        high -= qtr1;
                    }
                    else 
		        break;
            low += low;
            high += high + 1;
        }
    }
    Out << tmp;
    In.close();
    Out.close();
    return 0;
}
