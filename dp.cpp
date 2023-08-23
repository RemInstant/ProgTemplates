#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pll = pair<ll, ll>;

// DYNAMIC PROGRAMMING EXAMPLES

// The knapsack problem
// There is a set of items, each of which has Weight and Value
// and there is a knapsack that carries a limited weight
// The problem is to figure out max value that can be received depending on the capacity of the knapsack
// Time complexity O(nW), n - the number of items, W - sum of all weights
ll solveKnapsackProblemByWeightDP(vector<pll> stuff, ll maxW) {
	ll n = stuff.size();
	// DP definition
	// dp[i][j] - max value of a subset of the first (i+1) elements whose total weight is not greater than j
	// only dp[i] is required to fill dp[i+1] so dp[n] can be compressed into dp[2]
	vector< vector<ll> > dp(2, vector<ll>(maxW+1, 0));
	for(ll j = 0; j <= maxW; ++j) {
		if(stuff[0].first > j) continue;
		dp[0][j] = stuff[0].second;
	}
	
	// DP rules
	// dp[i][j] is maximum among
	// 1. dp[i-1][j] - using the same subset
	// 2. dp[i-1][j-curW] + curV - adding item with weight of curW to subset with a total weight of (j-curW)
	for(ll i = 1; i < n; ++i) {
		for(ll j = 0; j <= maxW; ++j) {
			dp[1][j] = dp[0][j];
			ll curW = stuff[i].first;
			ll curV = stuff[i].second;
			if(j >= curW) dp[1][j] = max(dp[1][j], dp[0][j-curW] + curV);
		}
		for(ll j = 0; j <= maxW; ++j) dp[0][j] = dp[1][j];
	}
	
	// The answer is the max value of a subset of the vector stuff
	// whose total weight is not greater than maxW - dp[n-1][maxW-1] -> dp[1][maxW-1]
	return dp.back().back();
}

// The knapsack problem
// Time complexity O(nV), n - the number of items, V - sum of all values
ll solveKnapsackProblemByValueDP(vector<pll> stuff, ll maxW) {
	ll n = stuff.size();
	ll sumVal = 0;
	for(pll item : stuff) sumVal += item.second;
	
	// DP definition
	// dp[i][j] - min weight of a subset of the first (i+1) elements whose total value is not less than j
	// only dp[i] is required to fill dp[i+1] so dp[n] can be compressed into dp[2]
	vector< vector<ll> > dp(2, vector<ll>(sumVal+1, 1e18));
	for(ll j = 0; j <= sumVal; ++j) {
		if(j > stuff[0].second) break;
		dp[1][j] = stuff[0].first;
	}
	
	// DP rules
	// dp[i][j] is minimum among
	// 1. dp[i-1][j] - using the same subset
	// 2. dp[i-1][j-curV] + curW - adding item with value of curV to subset with a total value of (j-curV)
	// 3. curW - replacing the whole subset with one item if its value is not less than j
	for(ll i = 1; i < n; ++i) {
		ll curW = stuff[i].first;
		ll curV = stuff[i].second;
		for(ll j = 0; j <= sumVal; ++j) {
			dp[0][j] = dp[1][j];
		}
		for(ll j = 0; j <= sumVal; ++j) {
			if(curV <= j) dp[1][j] = min(dp[1][j], dp[0][j-curV] + curW);
			if(curV >= j) dp[1][j] = min(dp[1][j], curW);
		}
	}
	
	// The answer is the maximum among the total values (j) of subsets whose weight does not exceed the capacity of the knapsack
	for(ll j = sumVal; j >= 0; --j) {
		if(dp[1][j] <= maxW) return j;
	}
	return 0;
}

// The Longest Increasing Subsequence problem
// Time complexity - O(nlogn)
vector<ll> solveLISProblem(vector<ll> a) {
	ll n = a.size();
	ll maxLen = 0;
	
	// DP definition
	// dp[i]   - the minimum last number in a subsequence of length i
	// ind[i]  - the index of this number
	// link[i] - the index of number preceding number a[i]
	vector<ll> dp(n+1, 1e18);
	vector<ll> ind(n+1, -1);
	vector<ll> link(n, -1);
	dp[0] = -1e18;
	
	// DP rules
	// The min position pos is determined for each number X of the initial array so that dp[pos-1] < X
	// then if X < dp[i], dp[i] = X
	// pos can be figured out using a binary search
	for(ll i = 0; i < n; ++i) {
		ll pos = upper_bound(dp.begin(), dp.end(), a[i]) - dp.begin();
		if(dp[pos-1] < a[i] && a[i] < dp[pos]) {
			dp[pos] = a[i];
			ind[pos] = i;
			link[i] = ind[pos-1];
			maxLen = max(maxLen, pos);
		}
	}
	
	// Restoration of the LIS
	ll p = ind[maxLen];
	vector<ll> lis(0);
	while(p != -1) {
		lis.push_back(a[p]);
		p = link[p];
	}
	reverse(lis.begin(), lis.end());
	return lis;
}

using graph = vector< vector<ll> >;

ll doSalesmanRecursion(vector< vector<ll> >& dp, graph& g, ll i, ll mask, ll n);

// The Traveling Salesman Problem
// Map out a route of minimum cost passing through all towns and starting in the first of them
// Time complexity O(2^(n-1) * n^2)
// Memory complexity O(2^n * n) ( Can be upgraded to O(2^(n-1) * n) )
ll solveTravelingSalesmanProblem(graph& g) {
	ll n = g.size();
	if(n == 1) return 0;
	
	// DP definition
	// dp[i][j] - the min cost of a route ending in town i and passing through all towns specified by bitmask j
	vector< vector<ll> > dp(n, vector<ll>(1 << n, -1));
	
	for(int i = 0; i < n; ++i) {
		dp[i][1 << i | 1] = g[0][i];
	}
	
	ll ans = 1e18;
	for(int i = 1; i < n; ++i) {
		ans = min(ans, doSalesmanRecursion(dp, g, i, (1 << n)-1, n));
	}
	
	return ans;
}

// Returns the min cost of a route ending in town i and passing through all towns specified by bitmask mask
// (i.e. returns dp[i][mask])
ll doSalesmanRecursion(vector< vector<ll> >& dp, graph& g, ll i, ll mask, ll n) {
	if(dp[i][mask] != -1) return dp[i][mask];
	ll res = 1e18;
	ll mask2 = mask ^ (1 << i);	// all towns without the last one
	if(mask2 == 0) return res;
	for(int j = 1; j < n; ++j) {
		if(0 == (mask2 & (1 << j))) continue;
		// {route} = {last town} + {route} / {last town}
		// min cost of {route}/{last town} is determines in the same way
		res = min(res, g[j][i] + doSalesmanRecursion(dp, g, j, mask2, n));
	}
	dp[i][mask] = res;
	return res;
}

int main() {
	
	ll capacity = 20;
	vector<pll> stuff = {
		{11, 15}, {10, 10}, {10, 10}
	};
	
	cout << solveKnapsackProblemByWeightDP(stuff, capacity) << '\n';	// 20
	cout << solveKnapsackProblemByValueDP(stuff, capacity) << '\n';		// 20
	cout << '\n';
	
	vector<ll> sequence = {1, 5, 100, 7, 4, 5, 200, 10, 50};
	for(ll val : solveLISProblem(sequence)) {
		cout << val << ' ';	// {1, 4, 5, 10, 50}
	}
	cout << "\n\n";
	
	graph g = {
		{  0,  5,  10},
		{100,  0, 100},
		{ 10, 10,   0}
	};
	
	cout << solveTravelingSalesmanProblem(g) << '\n';	// 20 ( route = {0, 3, 2} )
}