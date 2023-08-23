#include <iostream>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;
using graph = vector< vector<int> >;


// flow network
class network {
	struct edge {
		int u, v;
		ll f, c;
		
		edge(int _u, int _v, int _c): u(_u), v(_v), f(0), c(_c) {}
	};
	
  private:
	int n_;					// vertex count
	vector<edge> edges_;	// vector of edges
	graph g_;				// "pointers" on edges
	vector<int> d_;			// distances from start
	vector<size_t> ptr_;	// speeds up finding augmenting paths in dfs
	
  public:
	network(int n): n_(n), g_(n) {}
	
	//adds edge and inverse one to the network
	void add_edge(int u, int v, int cap) {
		g_[u].push_back(edges_.size());
		edges_.push_back(edge(u, v, cap));
		g_[v].push_back(edges_.size());
		edges_.push_back(edge(v, u, 0));
	}
	
	friend ostream& operator<<(ostream& out, const network& net) {
		int m = net.edges_.size();
		for (int i = 0; i < m; i+=2) {
			edge dg = net.edges_[i];
			if(dg.f > 0) {
				out << dg.u+1 << ' ' << dg.v+1 << ' ' << dg.f << '\n';
			}
		}
		return out;
	}
	
	// finds max flow in the network using Dinic's algorithm
	int getMaxFlow(int start, int finish) {
		d_.assign(n_, -1);
		ptr_.assign(n_, 0);
		int res = 0;
		for(int pw = 30; pw >= 0; --pw) {
			res += dinic(start, finish, 1 << pw);
		}
		return res;
	}
	
  private:
	void bfs(int u, int b) {
		d_.assign(n_, -1);
		queue<int> q;
		d_[u] = 0;
		q.push(u);
		while (!q.empty()) {
			u = q.front();
			q.pop();
			for(int ind : g_[u]) {
				int v = edges_[ind].v;
				int c = edges_[ind].c;
				int f = edges_[ind].f;
				if(c - f < b) continue;
				if(d_[v] == -1) {
					d_[v] = d_[u] + 1;
					q.push(v);
				}
			}
		}
	}
	
	int dfs(int u, int finish, int b, int fp) {
		if(u == finish) {
			return fp;
		}
		for(; ptr_[u] < g_[u].size(); ++ptr_[u]) {
			int ind = g_[u][ptr_[u]];
			int v = edges_[ind].v;
			int c = edges_[ind].c;
			int f = edges_[ind].f;
			if(c - f < b) continue;
			if(d_[v] != d_[u]+1) continue;
				
			int tmp = dfs(v, finish, b, min(c - f, fp));
			if(tmp > 0) {
				edges_[ind].f += tmp;
				edges_[ind^1].f -= tmp;
				return tmp;
			}
		}
		return 0;
	}
	
	int dinic(int start, int finish, int b) {
		int res = 0;
		while(1) {
			bfs(start, b);
			if(d_[finish] == -1) {
				break;
			}
			ptr_.assign(n_, 0);
			while(1) {
				int f = dfs(start, finish, b, 2e9);
				if(f > 0) {
					res += f;
				} else break;
			}
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
	
	//   _________[1]_________
	//  |  (100)   |   (100)  |
	//-[0]         |(1)      [3]-
	//  |_________[2]_________|
	//     (100)       (100)
	
	cout << net.getMaxFlow(0, 3) << '\n'; // 200
}