#include <iostream>
#include <vector>

using namespace std;
using ll = long long;
using pll = pair<ll, ll>;

const int MAX_D = 20;

struct wedge {
	int v;
	ll w;
};

using graph = vector< vector<int> >;
using wgraph = vector< vector<wedge> >;

// lowest common ancestor (in tree ofc)
// build O(n*MAX_D) <- O(nlogn)
// query O(logn)
class lca {
	
  private:
	int n_;						// vertex count
	vector<int> d_;				// depth of vertices
	vector< vector<int> > up_; 	// "binary" ancestors of a vertex
								// up[v][0] - 1st ancestor of v,
								// up[v][1] - 2nd ancestor of v,
								// up[v][2] - 4th ancestor of v,
								// up[v][3] - 8th ancestor of v...
	vector< vector<ll> > data_;	// max data (example)
	
  public:
	// dfs to fill vector d and first ancestors in vector up
	void dfs(const graph& g, int u, int prev) {
		for(int v : g[u]) {
			if(v == prev) continue;
			d_[v] = d_[u] + 1;
			up_[v][0] = u;
			dfs(g, v, u);
		}
	}
	
	void dfs(const wgraph& g, int u, int prev) {
		for(wedge wdg : g[u]) {
			int v = wdg.v;
			int w = wdg.w;
			if(v == prev) continue;
			d_[v] = d_[u] + 1;
			up_[v][0] = u;
			data_[v][0] = w;
			dfs(g, v, u);
		}
	}
	
	lca(const graph& g, int root) {
		n_ = g.size();
		d_.assign(n_, 0);
		up_.assign(n_, vector<int>(MAX_D, 0));
		data_.assign(n_, vector<ll>(MAX_D, 0));
		up_[root][0] = 0;
		dfs(g, root, root);
		for(int j = 1; j < MAX_D; ++j) {
			for(int u = 0; u < n_; ++u) {
				up_[u][j] = up_[up_[u][j-1]][j-1];
				data_[u][j] = max(data_[u][j-1], data_[up_[u][j-1]][j-1]);
			}
		}
	}
	
	lca(const wgraph& g, int root) {
		n_ = g.size();
		d_.assign(n_, 0);
		up_.assign(n_, vector<int>(MAX_D, 0));
		data_.assign(n_, vector<ll>(MAX_D, 0));
		up_[root][0] = 0;
		dfs(g, root, root);
		for(int j = 1; j < MAX_D; ++j) {
			for(int u = 0; u < n_; ++u) {
				up_[u][j] = up_[up_[u][j-1]][j-1];
				data_[u][j] = max(data_[u][j-1], data_[up_[u][j-1]][j-1]);
			}
		}
	}
	
	// adds a leaf-node to the tree
	// u - the parent of new node
	void addLeaf(int u) {
		d_.push_back(d_[u]+1);
		up_.push_back(vector<int>(MAX_D));
		up_[n_][0] = u;
		for(int j = 1; j < MAX_D; ++j) {
			up_[n_][j] = up_[up_[n_][j-1]][j-1];
			data_[n_][j] = max(data_[n_][j-1], data_[up_[n_][j-1]][j-1]);
		}
		++n_;
	}
	
	// checks if vertex u is the parent of vertex v
	bool isParent(int u, int v) {
		if(d_[u] > d_[v]) return false;
		int delta = d_[v] - d_[u];
		for(int j = MAX_D-1; j >= 0; --j) {
			if(delta & (1 << j)) {
				v = up_[v][j];
			}
		}
		return u == v;
	}
	
	// finds the lca of vertices u and v
	int find(int u, int v) {
		if(d_[u] > d_[v]) swap(u,v);
		int delta = d_[v] - d_[u];
		for(int j = MAX_D-1; j >= 0; --j) {
			if(delta & (1 << j)) {
				v = up_[v][j];
			}
		}
		if(u == v) return u;
		for(int j = MAX_D-1; j >= 0; --j) {
			if(up_[u][j] != up_[v][j]) {
				u = up_[u][j];
				v = up_[v][j];
			}
		}
		return up_[u][0];
	}
	
	// finds the distance between vertices u and v
	int getLen(int u, int v) {
		if(d_[u] > d_[v]) swap(u,v);
		int delta = d_[v] - d_[u];
		int res = delta;
		for(int j = MAX_D-1; j >= 0; --j) {
			if(delta & (1 << j)) {
				v = up_[v][j];
			}
		}
		if(u == v) return res;
		for(int j = MAX_D-1; j >= 0; --j) {
			if(up_[u][j] != up_[v][j]) {
				u = up_[u][j];
				v = up_[v][j];
				res += 2 * (1 << j);
			}
		}
		return res + 2;
	}
	
	// finds max data between vertices u and v (example)
	int getMaxData(int u, int v) {
        if(d_[u] > d_[v]) swap(u,v);
        int delta = d_[v] - d_[u];
        ll res = -2e9;
        for(int j = MAX_D-1; j >= 0; --j) {
            if(delta & (1 << j)) {
                res = max(res, data_[v][j]);
                v = up_[v][j];
            }
        }
        if(u == v) return res;
        for(int j = MAX_D-1; j >= 0; --j) {
            if(up_[u][j] != up_[v][j]) {
                res = max(max(res, data_[u][j]), data_[v][j]);
                u = up_[u][j];
                v = up_[v][j];
            }
        }
        return max(max(res, data_[u][0]), data_[v][0]);
    }
};
 
int main() {
	cout << "Hello lca!\n";
	
	int n = 7;
	wgraph g(n);
	//for(int i = 1; i < n; ++i) {
	//	int u, v, w;
	//	cin >> u >> v >> w;
	//	g[u].push_back({v, w});
	//}
	
	g[0].push_back({1, 1});
	g[1].push_back({2, 1});
	g[1].push_back({3, 8});
	g[0].push_back({4, 2});
	g[4].push_back({5, 3});
	g[5].push_back({6, 5});
	
	//        0
	//    (1)/ \(2)
	//      /   4
	//     1     \(3)
	// (1)/ \(8)  5
	//   2   3     \(5)
	//              6
	
	lca c(g, 0);
	cout << c.getLen(2, 6) << '\n';		// 5
	cout << c.getMaxData(2, 6) << '\n';	// 5
	cout << c.getMaxData(2, 5) << '\n';	// 3
	cout << c.getMaxData(3, 5) << '\n';	// 8
}