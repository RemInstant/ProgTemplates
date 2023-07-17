#include <iostream>
#include <vector>
 
using namespace std;
 
// disjoint set union
// build O(n)
// query O(α(n)) ~ O(1)
struct dsu {
	vector<int> leader; // leader of set
	vector<int> sz;     // size of set
	vector<int> mx;     // max element of set (example)
	
	dsu(int n) {
		sz.resize(n, 1);
		mx.resize(n);
		leader.resize(n);
		for(int i = 0; i < n; ++i) {
			mx[i] = i+1;
			leader[i] = i;
		}
	}
	
	// finds the leader of set that contains the element a
	int find(int a) {
		if(leader[a] == a) return a;
		leader[a] = find(leader[a]);
		return leader[a];
	}
	
	// merges sets which include elements u and v
	// returns false if these vertices already belong 
	// to the same set, and true otherwise
	bool unite(int u, int v) {
		int lu = find(u);
		int lv = find(v);
		if(lu == lv) return false;
		if(sz[lu] > sz[lv]) swap(lu, lv);
		leader[lu] = lv;
		sz[lv] += sz[lu];
		mx[lv] = max(mx[lv], mx[lu]);
		return true;
	}
};

int main() {
	cout << "Hello dsu!\n";
}