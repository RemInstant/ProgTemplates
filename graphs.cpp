#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;
using ll = long long;
using pll = pair<ll, ll>;

struct wedge {
	int v;
	ll w;
};

using graph = vector< vector<int> >;
using wgraph = vector< vector<wedge> >;

// breadth-first search
// finds the min path from given vertex to each other in graph
// g - the graph
// st - the given vertex
// returns vector of distances
vector<int> bfs(const graph& g, int st) {
	vector<int> d(g.size(), -1);
	d[st] = 0;
	queue<int> q;
	q.push(st);
	while(q.size() > 0) {
		int u = q.front();
		q.pop();
		for(int v : g[u]) {
			if(d[v] == -1) {
				d[v] = d[u] + 1;
				q.push(v);
			}
		}
	}
	return d;
}

// recursion for dfs
void dfsRec(const graph& g, int u, vector<int>& d, vector<char>& visited ) {
	visited[u] = 1;
	for(int v : g[u]) {
		if(visited[v]) continue;
		d[v] = min(d[v], d[u]+1);
		dfsRec(g, v, d, visited);
	}
}


// depth-first search
// finds the min path from given vertex to each other in graph
// g - the graph
// st - the given vertex
// returns vector of distances
vector<int> dfs(const graph& g, int st) {
	vector<int> d(g.size(), 1e9);
	vector<char> visited(g.size(), 0);
	d[st] = 0;
	dfsRec(g, st, d, visited);
	return d;
}

// Floyde-Warshall algorithm
// finds the min path between each pair of vertices in wgraph without negative cycles
// g - the wgraph
// st - the given vertex
// returns matrix of distances
vector< vector<ll> > fw(const wgraph& g) {
	int n = g.size();
	vector< vector<ll> > d(n, vector<ll>(n, 1e18));
	for(int i = 0; i < n; ++i) {
		d[i][i] = 0;
	}
	for(int u = 0; u < n; ++u) {
		for(wedge wdg : g[u]) {
			int v = wdg.v;
			ll w = wdg.w;
			d[u][v] = w;
		}
	}
	for(int k = 0; k < n; ++k) {
		for(int u = 0; u < n; ++u) {
			for(int v = 0; v < n; ++v) {
				d[u][v] = min(d[u][v], d[u][k]+d[k][v]);
			}
		}
	}
	return d;
}

// Dijkstra's algorithm for sparse wgraphs ( O(nlogn + mlogn) )
// finds the min path from given vertex to each other in wgraph without negative weights
// g - the wgraph
// st - the given vertex
// returns vector of distances
vector<ll> djk(const wgraph& g, int st) {
	vector<ll> d(g.size(), 1e18);
	set<pll> s;
	d[st] = 0;
	s.insert({0, st});
	while(!s.empty()) {
		pll dpu = *s.begin();
		int u = dpu.second;
		s.erase(dpu);
		for(wedge wdg : g[u]) {
			int v = wdg.v;
			int w = wdg.w;
			if(d[u] + w < d[v]) {
				s.erase({d[v], v});
				d[v] = d[u] + w;
				s.insert({d[v], v});
			}
		}
	}
	return d;
}

// Dijkstra's algorithm for dense wgraphs ( O(n^2 + m) )
// todo
	
// Prim's algorithm
// finds the weight of min spanning tree of the given wgraph
// g - the given wgraph
ll prim(const wgraph& g) {
	ll total = 0;
	vector<ll> d(g.size(), 1e18);
	vector<char> visited(g.size(), 0);
	set<pll> s;
	s.insert({0, 0});
	while(!s.empty()) {
		pll dpu = *s.begin();
		int u = dpu.second;
		s.erase(dpu);
		if(visited[u]) continue;
		visited[u] = 1;
		total += d[u];
		for(wedge wdg : g[u]) {
			int v = wdg.v;
			int w = wdg.w;
			if(w < d[v]) {
				s.erase({d[v], v});
				d[v] = w;
				s.insert({d[v], v});
			}
		}
	}
	return total;
}

// recursion for kuhn
bool dfsKuhn(const graph& g, int u, vector<int>& mt, vector<char>& visited) {
    if(visited[u]) return false;
    visited[u] = 1;
    for(int v : g[u]) {
        if(mt[v] == -1) {
            mt[v] = u;
            return true;
        }
        if(dfsKuhn(g, mt[v], mt, visited)) {
            mt[v] = u;
            return true;
        }
    }
    return false;
}

// Kuhn's algorithm
// finds max matching in the given bipartite graph
// g - the given bipartite graph
// st - starting vertex (optional)
int kuhn(const graph& g, int st = 0) {
	int n = g.size();
	int ans = 0;
	vector<int> d = bfs(g, st);
    vector<int> mt(n, -1);
    for(int i = 0; i < n; ++i) {
        if(d[i] & 1) continue;
        vector<char> visited(n, 0);
        ans += dfsKuhn(g, i, mt, visited);
    }
	return ans;
}



int main() {
	cout << "Hello graphs!\n";
	
	int n = 6;
	graph g(n);
	g[0].push_back(2);
	g[0].push_back(3);
	g[0].push_back(4);
	g[1].push_back(3);
	g[1].push_back(4);
	g[1].push_back(5);
	g[2].push_back(0);
	g[3].push_back(0);
	g[3].push_back(1);
	g[4].push_back(0);
	g[4].push_back(1);
	g[5].push_back(1);
	
	cout << kuhn(g) << '\n';
	cout << kuhn(g, 1) << '\n';
	cout << kuhn(g, 2) << '\n';
	cout << kuhn(g, 5) << '\n';
}