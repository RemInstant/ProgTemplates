#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <functional>

using namespace std;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;

// implicit treap (implicit cartesian tree)
// it represents a sequence of elements and implements such O(logn)-operations
// as insert, erase, get, set, compute target function on any subsegment,
// update any subsegment (set*/add), reverse any subsegment
// *by default
template<typename T, typename targetFunctor = plus<T>>
class implicitTreap {
	
	enum treapUpdMode {
		SET,
		ADD
	};
	
	// treapGetMode?
	
	struct node {
		uint cnt;		// count of nodes in the subtree
		int prior;		// priority of node (used to achieve balancity of tree)
		bool rev;		// is subTree reversed?
		bool upd;		// does val have to be updated?
		T val;			// value of node
		T subVal;		// value of target function on subtree (subsegment)
		T updVal;		// value that will update current one
		node* left;		// left child
		node* right;	// right child
		
		node(T _val, int _prior): cnt(1), prior(_prior), rev(false), upd(false) {
			left = right = nullptr;
			subVal = val = _val;
			updVal = T();
		}
	};
	
	using pnode = node*;
	
  private:
	pnode root_;
	mt19937 rng_;
	targetFunctor targetF_;
	treapUpdMode updMode_;
	
  public:
	implicitTreap() {
		root_ = nullptr;
		rng_ = mt19937(chrono::steady_clock::now().time_since_epoch().count());
		targetF_ = targetFunctor();
		updMode_ = SET;
	}
	
	implicitTreap(uint n): implicitTreap() {
		for(uint i = 0; i < n; ++i) {
			pnode tn = new node(T(), rng_());
			root_ = merge(root_, tn);
		}
	}
	
	implicitTreap(vector<T> arr): implicitTreap() {
		for(uint i = 0; i < arr.size(); ++i) {
			pnode tn = new node(arr[i], rng_());
			root_ = merge(root_, tn);
		}
	}
	
	// prints the sequence
	void print() { printInternal(root_); cout << '\n'; } 
	
	// switches update mode to ADD ( in O(n) )
	void switchUpdateModeToAdd() {
		pushAll(root_);
		updMode_ = ADD;
	}
	
	// switches update mode to SET ( in O(n) )
	void switchUpdateModeToSet() {
		pushAll(root_);
		updMode_ = SET;
	}
	
	// inserts a new element before the pth element
	void insert(uint p, T x) {
		pnode tLeft = nullptr, tRight = nullptr;
		pnode tNew = new node(x, rng_());
		split(root_, p, tLeft, tRight);
		root_ = merge(tLeft, merge(tNew, tRight));
	}
	
	// erases the pth element
	void erase(uint p) {
		pnode tLeft, tRight, tMid;
		tLeft = tRight = tMid = nullptr;
		split(root_, p+1, tLeft, tRight);
		split(tLeft, p, tLeft, tMid);
		delete tMid;
		root_ = merge(tLeft, tRight);
	}
	
	// returns the pth element
	T get(uint p) {
		pnode tLeft, tRight, tMid;
		tLeft = tRight = tMid = nullptr;
		split(root_, p+1, tLeft, tRight);
		split(tLeft, p, tLeft, tMid);
		T ans = getVal(tMid);
		root_ = merge(tLeft, merge(tMid, tRight));
		return ans;
	}
	
	// returns the value of target function on the subsegment [l;r]
	T get(uint l, uint r) {
		pnode tLeft, tRight, tMid;
		tLeft = tRight = tMid = nullptr;
		split(root_, r+1, tLeft, tRight);
		split(tLeft, l, tLeft, tMid);
		T ans = getSubVal(tMid);
		root_ = merge(tLeft, merge(tMid, tRight));
		return ans;
	}
	
	// sets the value of pth element to x
	void set(uint p, T x) {
		pnode tLeft, tRight, tMid;
		tLeft = tRight = tMid = nullptr;
		split(root_, p+1, tLeft, tRight);
		split(tLeft, p, tLeft, tMid);
		tMid->val = x;
		root_ = merge(tLeft, merge(tMid, tRight));
	}
	
	// updates the subsegment [l;r] with value x (by default adds x)
	void update(uint l, uint r, T x) {
		pnode tLeft, tRight, tMid;
		tLeft = tRight = tMid = nullptr;
		split(root_, r+1, tLeft, tRight);
		split(tLeft, l, tLeft, tMid);
		tMid->upd = true;
		tMid->updVal = x;
		root_ = merge(tLeft, merge(tMid, tRight));
	}
	
	// reverses the subsegment [l;r]
	void reverse(uint l, uint r) {
		pnode tLeft, tRight, tMid;
		tLeft = tRight = tMid = nullptr;
		split(root_, r+1, tLeft, tRight);
		split(tLeft, l, tLeft, tMid);
		tMid->rev ^= true;
		root_ = merge(tLeft, merge(tMid, tRight));
	}
	
	// TODO
	// move segment?
	
  private:
	uint getCnt(pnode t) { return t == nullptr ? 0 : t->cnt; }
	T getVal(pnode t) { return t == nullptr ? T() : t->val; }
	T getSubVal(pnode t) { return t == nullptr ? T() : t->subVal; }
	T updateVal(T a, T b) { return updMode_ == ADD ? (a + b) : b; }
	
	T getRangedTargetF(T val, uint cnt) {
		if(val == targetF_(val, val)) return val;
		if(2*val == targetF_(val, val)) return val * cnt;
		cout << "RANGED GET ERROR\n";
		return T();
	}
	
	void updateCnt(pnode t) {
		if(t == nullptr) return;
		t->cnt = 1 + getCnt(t->left) + getCnt(t->right);
		if(t->upd) {
			t->subVal = updateVal(t->subVal, getRangedTargetF(t->updVal, t->cnt));
			t->upd = false;
			t->updVal = T();
			return;
		}
		t->subVal = t->val;
		t->subVal = targetF_(t->subVal, getSubVal(t->left));
		t->subVal = targetF_(t->subVal, getSubVal(t->right));
	}
	
	void push(pnode t) {
		if(t == nullptr) return;
		push(t, t->left);
		push(t, t->right);
		if(t->rev) swap(t->left, t->right);
		if(t->upd) t->val = updateVal(t->val, t->updVal);
		t->rev = false;
	}
	
	void push(pnode t, pnode child) {
		if(t == nullptr || child == nullptr) return;
		child->rev ^= t->rev;
		if(t->upd) {
			child->upd = t->upd;
			child->updVal = t->updVal;
		}
	}
	
	void pushAll(pnode t) {
		if(t == nullptr) return;
		push(t);
		pushAll(t->left);
		pushAll(t->right);
		updateCnt(t);
	}
	
	void split(pnode t, uint x0, pnode& t1, pnode& t2) {
		if(t == nullptr) {
			t2 = t1 = nullptr;
			return;
		}
		push(t);
		if(x0 >= 1 + getCnt(t->left)) {
			split(t->right, x0 - (1 + getCnt(t->left)), t->right, t2);
			t1 = t;
		} else {
			split(t->left, x0, t1, t->left);
			t2 = t;
		}
		updateCnt(t);
	}
	
	pnode merge(pnode t1, pnode t2) {
		if(t1 == nullptr) return t2;
		if(t2 == nullptr) return t1;
		if(t1->prior > t2->prior) {
			push(t1);
			t1->right = merge(t1->right, t2);
			updateCnt(t1);
			return t1;
		} else {
			push(t2);
			t2->left = merge(t1, t2->left);
			updateCnt(t2);
			return t2;
		}
	}
	
	void printInternal(pnode t) {
		if(t == nullptr) return;
		push(t);
		printInternal(t->left);
		cout << t->val  << ' ';
		printInternal(t->right);
		updateCnt(t);
	}
};

template<typename T, typename targetFunctor = plus<T>>
using iTreap = implicitTreap<T, targetFunctor>;

template<typename T>
struct mx { operator()(T a, T b) { return max(a, b); } };

int main() {
	
	vector<int> arr = {1, 2, 3};
	iTreap<int> t(arr);
	// equals to
	// iTreap<int, plus<int>> t(arr);
	
	t.switchUpdateModeToAdd();			// change update mode to add
	t.print();							// {1, 2, 3}
	t.insert(0, 5);						// insert 5 at pos 0
	t.print();							// {5, 1, 2, 3}
	cout << t.get(0) << '\n';			// get 1st elem -> 5
	t.set(1, 7);						// set 2nd elem value to 7
	t.print();							// {5, 7, 2, 3}
	cout << t.get(1, 3) << '\n';		// get sum of {7, 2, 3} -> 12;
	t.update(2, 3, 6);					// add 6 to 3rd and 4th elements
	t.print();							// {5, 7, 8, 9}
	t.reverse(0, 2);					// reverse subsegment from 1st to 3rd element
	t.reverse(1, 3);					// reverse subsegment from 2nd to 4th element
	t.print();							// {8, 9, 5, 7}
	t.erase(2);							// erase 3rd element -> 5
	cout << t.get(2) << '\n';			// get 3rd element -> 7
	t.erase(0);							// erase 1st element -> 8
	t.print();							// {9, 7}
	t.switchUpdateModeToSet();			// change update mode to set
	t.update(0, 1, 3);					// set all values to 3
	t.print();							// {3, 3}
}