#include <iostream>
#include <vector>

using namespace std;
using ll = long long;
using ull = unsigned long long;

// segtree
// build	O(n)
// get		O(flogn), where f - complexity of push (in this case O(1))
// update	O(logn)
template<typename T> 
class segtree {
	
  private:
	ull n_;
	vector<T> data_, delay_;
	
  public:
	// build segment tree over the empty vector of size _n
	segtree(ull n): n_(n), data_(4 * n, T()), delay_(4 * n, T()) {}
	
	// builds segment tree over the given vector
	segtree(const vector<T>& a): segtree(a.size()) { build(1, 1, n_, a); }
	
	// returns the pth element of the vector
	T get(ull p) { return getInternal(1, 1, n_, p, p); }
	
	// computes the sum of the elements of subvector [ql; qr]
	T get(ull l, ull r) { return getInternal(1, 1, n_, l, r); }
	
	// adds x to pth element of the vector
	void update(ull p, T x) { updateInternal(1, 1, n_, p, p, x); }
	
	// adds x to the elements of the subvector [l; r]
	void update(ull l, ull r, T x) { updateInternal(1, 1, n_, l, r, x); }
	
  private:
	void build(ull id, ull l, ull r, const vector<T>& a) {
		if(l == r) {
			data_[id] = a[l - 1];
			return;
		}
		ull m = (l+r)/2;
		build(id*2, l, m, a);
		build(id*2+1, m+1, r, a);
		data_[id] = data_[id*2] + data_[id*2+1];
	}
	
	void push(ull id, ull l, ull r) {
		if(delay_[id] == T()) return;
		if(id*2 < 4*n_) delay_[id*2] += delay_[id];
		if(id*2+1 < 4*n_) delay_[id*2+1] += delay_[id];
		data_[id] += delay_[id]*(r-l+1);
		delay_[id] = T();
	}
	
	T getInternal(ull id, ull l, ull r, ull ql, ull qr) {
		push(id, l, r);
		if(ql <= l && qr >= r) {
			return data_[id];
		}
		ull m = (l+r)/2;
		if(qr <= m) {
			return getInternal(id*2, l, m, ql, qr);
		}
		if(ql >= m+1) {
			return getInternal(id*2+1, m+1, r, ql, qr);
		}
		return getInternal(id*2, l, m, ql, qr) + getInternal(id*2+1, m+1, r, ql, qr);
	}
	
	void updateInternal(ull id, ull l, ull r, ull ql, ull qr, T x) {
		push(id, l, r);
		if(ql > r || qr < l) return;
		if(ql <= l && qr >= r) {
			delay_[id] += x;
			push(id, l, r);
			return;
		}
		ull m = (l+r)/2;
		updateInternal(id*2, l, m, ql, qr, x);
		updateInternal(id*2+1, m+1, r, ql, qr, x);
		data_[id] = data_[id*2] + data_[id*2+1];
	}
};

int main() {
	
	vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8};
	segtree<int> seg(arr);

	cout << seg.get(1, 3) << '\n';		// 6
	cout << seg.get(4, 8) << '\n';		// 30
	cout << seg.get(1, 8) << "\n\n";	// 36
	seg.update(1, 8, 1);				// {2, 3, 4, 5, 6, 7, 8, 9}
	cout << seg.get(1, 3) << '\n';		// 9
	cout << seg.get(4, 8) << '\n';		// 35
	cout << seg.get(1, 8) << "\n\n";	// 44
}