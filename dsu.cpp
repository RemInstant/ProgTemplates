#include <iostream>
#include <vector>
 
using namespace std;
 
// disjoint set union
// build O(n)
// query O(α(n)) ~ O(1)
class dsu {
	
  private:
	vector<int> leader_; // leader of set
	vector<int> sz_;     // size of set
	vector<int> mx_;     // max element of set (example)
	
  public:
	dsu(int n) {
		sz_.resize(n, 1);
		mx_.resize(n);
		leader_.resize(n);
		for(int i = 0; i < n; ++i) {
			mx_[i] = i+1;
			leader_[i] = i;
		}
	}
	
	// finds the leader of set that contains the element a
	int find(int a) {
		if(leader_[a] == a) return a;
		leader_[a] = find(leader_[a]);
		return leader_[a];
	}
	
	// merges sets which include elements u and v
	// returns false if these vertices already belong 
	// to the same set, and true otherwise
	bool unite(int u, int v) {
		int lu = find(u);
		int lv = find(v);
		if(lu == lv) return false;
		if(sz_[lu] > sz_[lv]) swap(lu, lv);
		leader_[lu] = lv;
		sz_[lv] += sz_[lu];
		mx_[lv] = max(mx_[lv], mx_[lu]);
		return true;
	}
};

int main() {
	cout << "Hello dsu!\n";
}