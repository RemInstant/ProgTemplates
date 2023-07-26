#include <iostream>
#include <vector>

using namespace std;
using ll = long long;
using ull = unsigned long long;

// segtree
// getFunction - computes the sum of the values of any subvector
// updFunction - adds any value to each element of any subvector
// build	O(n)
// get		O(flogn), where f - complexity of push (often O(1))
// update	O(logn)
template<typename T> 
struct segtree {
	ull n;
	vector<T> data, delay;
	
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
		data[id] = data[id*2] + data[id*2+1];
	}
	
	void push(ull id, ull l, ull r) {
		if(delay[id] == T()) return;
		if(id*2 < 4*n) delay[id*2] += delay[id];
		if(id*2+1 < 4*n) delay[id*2+1] += delay[id];
		data[id] += delay[id]*(r-l+1);
		delay[id] = T();
	}
	
	// returns the pth element of the vector
	T get(ull p) { return get(1, 1, n, p, p); }
	
	// computes the sum of the elements of subvector [ql; qr]
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
		return get(id*2, l, m, ql, qr) + get(id*2+1, m+1, r, ql, qr);
	}
	
	// adds x to pth element of the vector
	void update(ull p, T x) { update(1, 1, n, p, p, x); }
	
	// adds x to the elements of the subvector [l; r]
	void update(ull l, ull r, T x) { update(1, 1, n, l, r, x); }
	
	void update(ull id, ull l, ull r, ull ql, ull qr, T x) {
		push(id, l, r);
		if(ql > r || qr < l) return;
		if(ql <= l && qr >= r) {
			delay[id] += x;
			push(id, l, r);
			return;
		}
		ull m = (l+r)/2;
		update(id*2, l, m, ql, qr, x);
		update(id*2+1, m+1, r, ql, qr, x);
		data[id] = data[id*2] + data[id*2+1];
	}
};

int main() {
	
	vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8};
	segtree<int> seg(arr);

	cout << seg.get(1, 3) << '\n';
	cout << seg.get(4, 8) << '\n';
	cout << seg.get(1, 8) << "\n\n";
	seg.update(1, 8, 1);
	cout << seg.get(1, 3) << '\n';
	cout << seg.get(4, 8) << '\n';
	cout << seg.get(1, 8) << "\n\n";
}