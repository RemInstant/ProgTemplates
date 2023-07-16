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
struct lca {
	int n;						// vertex count
	vector<int> d;				// depth of vertices
	vector< vector<int> > up; 	// "binary" ancestors of a vertex
								// up[v][0] - 1st ancestor of v,
								// up[v][1] - 2nd ancestor of v,
								// up[v][2] - 4th ancestor of v,
								// up[v][3] - 8th ancestor of v...
	vector< vector<ll> > data;	// max data (example)
	
	// dfs to fill vector d and first ancestors in vector up
	void dfs(const graph& g, int u, int prev) {
		for(int v : g[u]) {
			if(v == prev) continue;
			d[v] = d[u] + 1;
			up[v][0] = u;
			dfs(g, v, u);
		}
	}
	
	void dfs(const wgraph& g, int u, int prev) {
		for(wedge wdg : g[u]) {
			int v = wdg.v;
			int w = wdg.w;
			if(v == prev) continue;
			d[v] = d[u] + 1;
			up[v][0] = u;
			data[v][0] = w;
			dfs(g, v, u);
		}
	}
	
	lca(const graph& g, int root) {
		n = g.size();
		d.assign(n, 0);
		up.assign(n, vector<int>(MAX_D, 0));
		data.assign(n, vector<ll>(MAX_D, 0));
		up[root][0] = 0;
		dfs(g, root, root);
		for(int j = 1; j < MAX_D; ++j) {
			for(int u = 0; u < n; ++u) {
				up[u][j] = up[up[u][j-1]][j-1];
				data[u][j] = max(data[u][j-1], data[up[u][j-1]][j-1]);
			}
		}
	}
	
	lca(const wgraph& g, int root) {
		n = g.size();
		d.assign(n, 0);
		up.assign(n, vector<int>(MAX_D, 0));
		data.assign(n, vector<ll>(MAX_D, 0));
		up[root][0] = 0;
		dfs(g, root, root);
		for(int j = 1; j < MAX_D; ++j) {
			for(int u = 0; u < n; ++u) {
				up[u][j] = up[up[u][j-1]][j-1];
				data[u][j] = max(data[u][j-1], data[up[u][j-1]][j-1]);
			}
		}
	}
	
	// adds a leaf-node to the tree
	// u - the parent of new node
	void addLeaf(int u) {
		d.push_back(d[u]+1);
		up.push_back(vector<int>(MAX_D));
		up[n][0] = u;
		for(int j = 1; j < MAX_D; ++j) {
			up[n][j] = up[up[n][j-1]][j-1];
			data[n][j] = max(data[n][j-1], data[up[n][j-1]][j-1]);
		}
		++n;
	}
	
	// checks if vertex u is the parent of vertex v
	bool isParent(int u, int v) {
		if(d[u] > d[v]) return false;
		int delta = d[v] - d[u];
		for(int j = MAX_D-1; j >= 0; --j) {
			if(delta & (1 << j)) {
				v = up[v][j];
			}
		}
		return u == v;
	}
	
	// finds the lca of vertices u and v
	int find(int u, int v) {
		if(d[u] > d[v]) swap(u,v);
		int delta = d[v] - d[u];
		for(int j = MAX_D-1; j >= 0; --j) {
			if(delta & (1 << j)) {
				v = up[v][j];
			}
		}
		if(u == v) return u;
		for(int j = MAX_D-1; j >= 0; --j) {
			if(up[u][j] != up[v][j]) {
				u = up[u][j];
				v = up[v][j];
			}
		}
		return up[u][0];
	}
	
	// finds the distance between vertices u and v
	int getLen(int u, int v) {
		if(d[u] > d[v]) swap(u,v);
		int delta = d[v] - d[u];
		int res = delta;
		for(int j = MAX_D-1; j >= 0; --j) {
			if(delta & (1 << j)) {
				v = up[v][j];
			}
		}
		if(u == v) return res;
		for(int j = MAX_D-1; j >= 0; --j) {
			if(up[u][j] != up[v][j]) {
				u = up[u][j];
				v = up[v][j];
				res += 2 * (1 << j);
			}
		}
		return res + 2;
	}
	
	// finds max data between vertices u and v (example)
	int getMaxData(int u, int v) {
        if(d[u] > d[v]) swap(u,v);
        int delta = d[v] - d[u];
        ll res = -2e9;
        for(int j = MAX_D-1; j >= 0; --j) {
            if(delta & (1 << j)) {
                res = max(res, data[v][j]);
                v = up[v][j];
            }
        }
        if(u == v) return res;
        for(int j = MAX_D-1; j >= 0; --j) {
            if(up[u][j] != up[v][j]) {
                res = max(max(res, data[u][j]), data[v][j]);
                u = up[u][j];
                v = up[v][j];
            }
        }
        return max(max(res, data[u][0]), data[v][0]);
    }
};
 
int main() {
	cout << "Hello lca!";
	
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
	
	lca c(g, 0);
	cout << '\n';
	cout << c.getLen(2, 6) << '\n';
	cout << c.getMaxData(2, 6) << '\n';
	cout << c.getMaxData(2, 5) << '\n';
	cout << c.getMaxData(3, 5) << '\n';
}