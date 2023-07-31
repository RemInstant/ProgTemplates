#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using ll = long long;

// The nim game
// Suppose there are n heaps each with some number of stones
// Two players take turns removing m>0 stones from one of these heaps
// The player with no possible moves loses
// Function takes as an argument a vector consisting of the sizes of these heaps
// and returns true if the initial position is winning, false otherwise
bool solveStandardNim(vector<ll>& heaps) {
	int xr = 0;
	for(ll i = 0; i < heaps.size(); ++i) xr ^= heaps[i];
	return xr > 0;
}

// minimal excluded
// returns minimal non-negative x that is not included in the vector
int mex(vector<ll> a) {
	ll n = a.size();
	sort(a.begin(), a.end());
	if(a[0] > 0) return 0;
	for(ll i = 0; i < n-1; ++i)
		if(a[i] + 1 < a[i+1]) return a[i]+1;
	return a.back()+1;
}

// The restricted nim game
// Suppose there are n heaps each with some number of stones
// Two players take turns choosing pth heap and removing m∈[1;k[p]] stones from it
// The player with no possible moves loses
// Function takes as arguments two vectors; one consisting of the sizes of these heaps
// and the other consist of natural numbers limiting removal in one move from corresponding heap
// As a result function returns true if the initial position is winning, false otherwise
bool solveRestrictedNim(vector<ll>& heaps, vector<ll>& limits) {
	// Sprague–Grundy theorem
	// Every position of the nim-like games can be represented as a position in the standard nim
	// Let sg-function convert the initial position into the equal position of the standard nim
	// Transition rule: sg[i] = mex{sg[j] | j - position immediately preceding position i}
	vector<ll> altHeaps(0);
	for(ll t = 0; t < heaps.size(); ++t) {
		vector<ll> sg(heaps[t]+1);
		ll k = limits[t];
		sg[0] = 0; // position (0) is obviously losing
		for(ll i = 1; i < sg.size(); ++i) {
			vector<ll> precedingPositions(0); // sg-function of positions, that lead in position (i)
			for(ll j = max(0ll, i-k); j < i; ++j) {
				precedingPositions.push_back(sg[j]);
			}
			sg[i] = mex(precedingPositions);
		}
		altHeaps.push_back(sg.back());
		// actually in this problem
		// sg[i] = i % (k + 1), so altHeaps[t] = heaps[t] % (limits[t] + 1)
	}
	return solveStandardNim(altHeaps);
}

// Another game example..
// Suppose there is a row of n cells
// Two players take turns marking cells with X
// The player who marks a cell next to the marked one loses
// Function takes as argument size of row
// and returns true if the initial position is winning, false otherwise
bool solveXGame(ll n) {
	vector<ll> sg(n+1);
	sg[0] = 0;
	for(ll i = 1; i <= n; ++i) {
		vector<ll> precedingPositions(0);
		for(ll j = 1; j <= i; ++j) {
			// when a cell is marked, game splits into two others
			// EXAMPLE:
			// [0000000] 1st players marks 3rd cell in row of size 7
			// [00x0000] -> [0]{0x0}[000]
			// row [0] equals to the standard nim heap of size sg[1]
			// row [000] equals to the standard nim heap of size sg[3]
			// the position (sg[1], sg[3]) of standard nim
			// equals to the position (sg[1] ^ sg[3]) = (equalHeap),
			// which immediately precedes the positiong sg[7]
			ll leftSize = max(0ll, j-2);
			ll rightSize = max(0ll, i - j - 1);
			ll leftEqualHeap = sg[leftSize];
			ll rightEqualHeap = sg[rightSize];
			ll equalHeap = leftEqualHeap ^ rightEqualHeap; 
			// equalHeap = solveStandardNim(leftEqualHeap, rightEqualHeap)
			precedingPositions.push_back(equalHeap);
		}
		sg[i] = mex(precedingPositions);
	}
	return sg[n] > 0;
}

int main() {
	
}