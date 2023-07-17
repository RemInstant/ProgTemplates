#include <iostream>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;
using graph = vector< vector<int> >;


// flow network
struct network {
	struct edge {
		int u, v;
		ll f, c;
		
		edge(int _u, int _v, int _c): u(_u), v(_v), f(0), c(_c) {}
	};
	
	int n;				// vertex count
	vector<edge> edges;	// vector of edges
	graph g;			// "pointers" on edges
	vector<int> d;		// distances from start
	vector<size_t> ptr;	// speeds up finding augmenting paths in dfs
	
	network(int _n): n(_n), g(n) {}
	
	//adds edge and inverse one to the network
	void add_edge(int u, int v, int cap) {
		g[u].push_back(edges.size());
		edges.push_back(edge(u, v, cap));
		g[v].push_back(edges.size());
		edges.push_back(edge(v, u, 0));
	}
	
	friend ostream& operator<<(ostream& out, const network& net) {
		int m = net.edges.size();
		for (int i = 0; i < m; i+=2) {
			edge dg = net.edges[i];
			if(dg.f > 0) {
				out << dg.u+1 << ' ' << dg.v+1 << ' ' << dg.f << '\n';
			}
		}
		return out;
	}
	
	void bfs(int u, int b) {
		d.assign(n, -1);
		queue<int> q;
		d[u] = 0;
		q.push(u);
		while (!q.empty()) {
			u = q.front();
			q.pop();
			for(int ind : g[u]) {
				int v = edges[ind].v;
				int c = edges[ind].c;
				int f = edges[ind].f;
				if(c - f < b) continue;
				if(d[v] == -1) {
					d[v] = d[u] + 1;
					q.push(v);
				}
			}
		}
	}
	
	int dfs(int u, int finish, int b, int fp) {
		if(u == finish) {
			return fp;
		}
		for(; ptr[u] < g[u].size(); ++ptr[u]) {
			int ind = g[u][ptr[u]];
			int v = edges[ind].v;
			int c = edges[ind].c;
			int f = edges[ind].f;
			if(c - f < b) continue;
			if(d[v] != d[u]+1) continue;
				
			int tmp = dfs(v, finish, b, min(c - f, fp));
			if(tmp > 0) {
				edges[ind].f += tmp;
				edges[ind^1].f -= tmp;
				return tmp;
			}
		}
		return 0;
	}
	
	int dinic(int start, int finish, int b) {
		int res = 0;
		while(1) {
			bfs(start, b);
			if(d[finish] == -1) {
				break;
			}
			ptr.assign(n, 0);
			while(1) {
				int f = dfs(start, finish, b, 2e9);
				if(f > 0) {
					res += f;
				} else break;
			}
		}
		return res;
	}
	
	// finds max flow in the network using Dinic's algorithm
	int getMaxFlow(int start, int finish) {
		d.assign(n, -1);
		ptr.assign(n, 0);
		int res = 0;
		for(int pw = 30; pw >= 0; --pw) {
			res += dinic(start, finish, 1 << pw);
		}
		return res;
	}
};
 
int main() {
	cout << "Hello flow network!\n";
	
	network net(4);
	net.add_edge(0, 1, 100);
	net.add_edge(0, 2, 100);
	net.add_edge(1, 2, 1);
	net.add_edge(1, 3, 100);
	net.add_edge(2, 3, 100);
	
	cout << net.getMaxFlow(0, 3) << '\n';
}