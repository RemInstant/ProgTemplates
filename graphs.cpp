#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;

struct wedge {
    int v;
    ll w;
};

using ll = long long;
using pll = pair<ll, ll>;
using graph = vector< vector<int> >;
using wgraph = vector< vector<wedge> >;

// breadth-first search
// find the min path from given vertex to each other in graph
// g - the graph
// st - the given vertex
// returns vector of distances
vector<ll> bfs(const graph& g, int st) {
    vector<ll> d(g.size(), -1);
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

// depth-first search
// find the min path from given vertex to each other in graph
// g - the graph
// st - the given vertex
// returns vector of distances
vector<ll> dfs(const graph &g, int st) {
    vector<ll> d(g.size(), 1e18);
    vector<int> visited(g.size(), 0);
    d[st] = 0;
    dfs(g, st, d, visited);
    return d;
}

void dfs(const graph &g, int u, vector<ll> &d, vector<int> &visited) {
    visited[u] = 1;
    for(int v : g[u]) {
        if(visited[v]) continue;
        d[v] = min(d[v], d[u]+1);
        dfs(g, v, d, visited);
    }
}

// Floyde-Warshall algorithm
// find the min path between each pair of vertices in wgraph without negative cycles
// g - the wgraph
// st - the given vertex
// returns matrix of distances
vector< vector<ll> > fw(const wgraph &g) {
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
}

// Dijkstra's algorithm for sparce wgraphs ( O(nlogn + mlogn) )
// find the min path from given vertex to each other in wgraph without negative weights
// g - the wgraph
// st - the given vertex
// returns vector of distances
vector<ll> djk(const wgraph &g, int st) {
    vector<ll> d(g.size(), 1e18);
    set<pll> s;
    d[st] = 0;
    s.insert({0, st});
    while(!s.empty()) {
        pll du = *s.begin();
        int u = du.second;
        s.erase(du);
        for(wedge edg : g[u]) {
            int v = edg.v;
            int w = edg.w;
            if(d[u] + w < d[v]) {
                s.erase({d[v], v});
                d[v] = d[u] + w;
                s.insert({d[v], v});
            }
        }
    }
}

// Dijkstra's algorithm for dense wgraphs ( O(n^2 + m) )
// todo

int main() {
}