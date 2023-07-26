#include <iostream>
#include <vector>
#include <functional>

using namespace std;
using ll = long long;
using ull = unsigned long long;

// segtree - a struct for
// computing a binary _getFunction_ on any subvector (sum/min/max)
// and executing a binary _updFunction_ on any subvector to modify it (sum/set)
// build	O(n)
// get		O(logn)
// update	O(logn)
template<typename T, typename getFunctor = plus<T>, typename updFunctor = plus<T>> 
struct segtree {
	ull n;
	vector<T> data, delay;
	getFunctor getF;
	updFunctor updF;
	
	// build segment tree over the empty vector of size _n
	segtree(ull _n): n(_n), data(4 * _n, T()), delay(4 * _n, T()) {}
	
	// builds segment tree over the given vector
	segtree(const vector<T>& a): segtree(a.size()) { build(1, 1, n, a); }
	
	void build(ull id, ull l, ull r, const vector<T>& a) {
		if(l == r) {
			data[id] = a[l - 1];
			return;
		}
		ull m = (l+r)/2;
		build(id*2, l, m, a);
		build(id*2+1, m+1, r, a);
		data[id] = getF(data[id*2], data[id*2+1]);
	}
	
	T doRangedGet(ull l, ull r, T x) {
		if(x == getF(x, x)) return x;
		if(2*x == getF(x, x)) return x*(r-l+1);
		cout << "RANGED GET ERROR\n";
		return T();
	}
	
	void push(ull id, ull l, ull r) {
		if(delay[id] == T()) return;
		if(id*2 < 4*n) delay[id*2] = updF(delay[id*2], delay[id]);
		if(id*2+1 < 4*n) delay[id*2+1] = updF(delay[id*2+1], delay[id]);
		data[id] = updF(data[id], doRangedGet(l, r, delay[id]));
		delay[id] = T();
	}
	
	// returns the pth element of the vector
	T get(ull p) { return get(1, 1, n, p, p); }
	
	// computes the getFunction on subvector [ql; qr]
	T get(ull l, ull r) { return get(1, 1, n, l, r); }
	
	T get(ull id, ull l, ull r, ull ql, ull qr) {
		push(id, l, r);
		if(ql <= l && qr >= r) {
			return data[id];
		}
		ull m = (l+r)/2;
		if(qr <= m) {
			return get(id*2, l, m, ql, qr);
		}
		if(ql >= m+1) {
			return get(id*2+1, m+1, r, ql, qr);
		}
		return getF(get(id*2, l, m, ql, qr), get(id*2+1, m+1, r, ql, qr));
	}
	
	// executes updFunction on the pth element of the vector
	void update(ull p, T x) { update(1, 1, n, p, p, x); }
	
	// executes updFunction on the elements of subvector [l; r]
	void update(ull l, ull r, T x) { update(1, 1, n, l, r, x); }
	
	void update(ull id, ull l, ull r, ull ql, ull qr, T x) {
		push(id, l, r);
		if(ql > r || qr < l) return;
		if(ql <= l && qr >= r) {
			delay[id] = updF(delay[id], x);
			push(id, l, r);
			return;
		}
		ull m = (l+r)/2;
		update(id*2, l, m, ql, qr, x);
		update(id*2+1, m+1, r, ql, qr, x);
		data[id] = getF(data[id*2], data[id*2+1]);
	}
};

template<typename T>
struct mx {
	T operator()(T a, T b) { return max(a, b); }
};

template<typename T>
struct set {
	T operator()(T a, T b) { return b; }
};

int main() {

	vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8};
	
	segtree<int, plus<int>, set<int>> segPS(arr);
	
	cout << segPS.get(1, 3) << '\n';
	cout << segPS.get(4, 8) << '\n';
	segPS.update(1, 3);
	cout << segPS.get(1, 8) << "\n\n";
	
	segtree<int> segPP(arr);
	// equals to
	// segtree<int, plus<int>, plus<int>> segPP(arr);
	
	cout << segPP.get(1, 3) << '\n';
	cout << segPP.get(4, 8) << '\n';
	segPP.update(1, 8, 10);
	cout << segPP.get(1, 8) << " (+10*8)\n\n";
	
	segtree<int, mx<int>, set<int>> segMxS(arr);
	
	cout << segMxS.get(1, 3) << '\n';
	cout << segMxS.get(4, 8) << '\n';
	segMxS.update(5, 8, 1);
	cout << segMxS.get(1, 8) << '\n';
}