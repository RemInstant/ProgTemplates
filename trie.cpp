#include <iostream>
#include <vector>
#include <map>

using namespace std;

// trie or prefix tree
// search O(n)
// insert O(n)
// delete O(n)
class trie {
	using dict = map<char, int>;
	
	// letter-node
	struct node {
		dict go;	// points on children
		int n;		// the number of words that ends on this letter
		int depth;	// the depth of node
		int subN;	// the number of words in this node's subtree
		
		node(bool _n, int _d): n(_n), depth(_d), subN(0) {};
	};
	
  private:
	vector<node> data_;

  public:
	trie() { create_node(0, 0); }
	
	int create_node(bool e, int d) {
		data_.push_back(node(e, d));
		return data_.size()-1;
	}
	
	// insert word s in the tree
	void insert(const string& s) {
		if(s.size() == 0) return;
		int u = 0;
		for(size_t i = 0; i < s.size(); ++i) {
			++data_[u].subN;
			if(!can_go(u, s[i])) {
				data_[u].go[s[i]] = create_node(0, data_[u].depth+1);
			}
			u = go(u, s[i]);
		}
		++data_[u].subN;
		++data_[u].n;
	}
	
	// counts the word s in the tree
	int count(const string& s) {
		int u = 0;
		for (char c : s) {
			if(!can_go(u,c)) return 0;
			u = go(u,c);
		}
		return data_[u].n;
	}
	
	// counts words with prefix s in the tree
	int subCount(const string& s) {
		int u = 0;
		for (char c : s) {
			if(!can_go(u,c)) return 0;
			u = go(u,c);
		}
		return data_[u].subN;
	}
	
	// deletes one word s from the tree
	void erase(const string& s) {
		int u = 0;
		for (char c : s) {
			--data_[u].subN;
			u = go(u,c);
		}
		--data_[u].subN;
		--data_[u].n;
	}
	
	// finds the kth word alphabetically in the tree (1-indexing)
	string get(int k) {
		int u = 0;
		string s = "";
		while(1) {
			k -= data_[u].n;
			if(k <= 0) return s;
			for(pair<char, int> q : data_[u].go) {
				int v = q.second;
				if(k <= data_[v].subN) {
					s += q.first;
					u = v;
					break;
				}
				k -= data_[v].subN;
			}
		}
	}
	
  private:
	bool can_go(int u, char c) { return data_[u].go.count(c); }
	
	int go(int u, char c) { return data_[u].go[c]; }
};
 
int main() {
	
	trie t;
	t.insert("aaa");
	t.insert("aaba");
	t.insert("aaba");
	t.insert("aab");
	t.insert("ba");
	t.insert("ab");
	t.insert("b");
	t.insert("a");
	
	cout << '\n';
	for(int i = 1; i <= 8; ++i) {
		cout << i << ' ' << t.get(i) << '\t' << i+8 << ' ' << t.get(i+8) << '\n';
	}
	
	t.erase("aaba");
	
	cout << '\n';
	for(int i = 1; i <= 7; ++i) {
		cout << i << ' ' << t.get(i) << '\t' << i+7 << ' ' << t.get(i+7) << '\n';
	}
}