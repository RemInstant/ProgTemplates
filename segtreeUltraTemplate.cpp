#include <iostream>
#include <vector>
#include <functional>

using namespace std;
using ll = long long;
using ull = unsigned long long;

// segtree - a struct for
// computing a target function on any subvector (sum/min/max)
// and updating any subsegment (set*/add)
// *by default
// build, changeUpdMode		O(n)
// get, update				O(logn)
template<typename T, typename targetFunctor = plus<T>> 
class segtree {
	
	enum segtreeUpdMode {
		SET,
		ADD
	};
	
  private:
	ull n_;
	vector<T> data_, delay_;
	targetFunctor targetF_;
	segtreeUpdMode updMode_;

  public:
	// build segment tree over the empty vector of size _n
	segtree(ull n): n_(n), data_(4 * n, T()), delay_(4 * n, T()) {}
	
	// builds segment tree over the given vector
	segtree(const vector<T>& a): segtree(a.size()) {
		targetF_ = targetFunctor();
		updMode_ = SET;
		build(1, 1, n_, a); 
	}
	
	// switches update mode to ADD ( in O(n) )
	void switchUpdateModeToAdd() {
		pushAll(1, 1, n_);
		updMode_ = ADD;
	}
	
	// switches update mode to SET ( in O(n) )
	void switchUpdateModeToSet() {
		pushAll(1, 1, n_);
		updMode_ = SET;
	}
	
	// returns the pth element of the vector
	T get(ull p) { return getInternal(1, 1, n_, p, p); }
	
	// computes the getFunction on subvector [ql; qr]
	T get(ull l, ull r) { return getInternal(1, 1, n_, l, r); }
	
	// executes updFunction on the pth element of the vector
	void update(ull p, T x) { updateInternal(1, 1, n_, p, p, x); }
	
	// executes updFunction on the elements of subvector [l; r]
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
		data_[id] = targetF_(data_[id*2], data_[id*2+1]);
	}
	
	T doRangedGet(ull l, ull r, T x) {
		if(x == targetF_(x, x)) return x;
		if(2*x == targetF_(x, x)) return x*(r-l+1);
		cout << "RANGED GET ERROR\n";
		return T();
	}
	
	T updateVal(T a, T b) { return updMode_ == ADD ? (a + b) : b; }
	
	void push(ull id, ull l, ull r) {
		if(delay_[id] == T()) return;
		if(id*2 < 4*n_) delay_[id*2] = updateVal(delay_[id*2], delay_[id]);
		if(id*2+1 < 4*n_) delay_[id*2+1] = updateVal(delay_[id*2+1], delay_[id]);
		data_[id] = updateVal(data_[id], doRangedGet(l, r, delay_[id]));
		delay_[id] = T();
	}
	
	void pushAll(ull id, ull l, ull r) {
		push(id, l, r);
		if(l == r) return;
		ull m = (l+r)/2;
		pushAll(id*2, l, m);
		pushAll(id*2+1, m+1, r);
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
		return targetF_(getInternal(id*2, l, m, ql, qr), getInternal(id*2+1, m+1, r, ql, qr));
	}
	
	void updateInternal(ull id, ull l, ull r, ull ql, ull qr, T x) {
		push(id, l, r);
		if(ql > r || qr < l) return;
		if(ql <= l && qr >= r) {
			delay_[id] = updateVal(delay_[id], x);
			push(id, l, r);
			return;
		}
		ull m = (l+r)/2;
		updateInternal(id*2, l, m, ql, qr, x);
		updateInternal(id*2+1, m+1, r, ql, qr, x);
		data_[id] = targetF_(data_[id*2], data_[id*2+1]);
	}
};

template<typename T>
struct mx { T operator()(T a, T b) { return max(a, b); } };

int main() {

	vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8};
	
	segtree<int> segPlus(arr);
	// equals to
	// segtree<int, plus<int>> segPlus(arr);
	
	cout << segPlus.get(1, 3) << '\n';		// 6
	cout << segPlus.get(4, 8) << '\n';		// 30
	segPlus.update(1, 3);					// {3, 2, 3, 4, 5, 6, 7, 8}
	cout << segPlus.get(1, 8) << "\n\n";	// 38
	
	segPlus.switchUpdateModeToAdd();
	
	cout << segPlus.get(1, 3) << '\n';		// 6
	cout << segPlus.get(4, 8) << '\n';		// 30
	segPlus.update(1, 8, 10);				// {11, 12, 13, 14, 15, 16, 17, 18}
	cout << segPlus.get(1, 8) << "\n\n";	// 116 (+8*10)
	
	segtree<int, mx<int>> segMxS(arr);
	
	cout << segMxS.get(1, 3) << '\n';		// 3
	cout << segMxS.get(4, 8) << '\n';		// 8
	segMxS.update(5, 8, 1);					// {1, 2, 3, 4, 1, 1, 1, 1}
	cout << segMxS.get(1, 8) << '\n';		// 4
}