#include <iostream>
#include <chrono>
#include <random>

using namespace std;

// treap (cartesian tree)
// it represents a set of elements (the pathetic parody of inimitable std::set)
template<typename T>
class treap {
	
	struct node {
		T val;		    // (x) value of node
		int prior;		// (y) priority of node (used to achieve balancity of tree)
		node* left;		// left child
		node* right;	// right child
		
		node(T _val, int _prior): val(_val), prior(_prior) {
			left = right = nullptr;
		}
	};
	
	using pnode = node*;
	
  private:
	pnode root_;
	mt19937 rng_;
	
  public:
	treap() {
		root_ = nullptr;
		rng_ = mt19937(chrono::steady_clock::now().time_since_epoch().count());
	}
	
	// prints the set values
	void print() { printInternal(root_); cout << '\n'; } 
	
	// inserts a new element 
	void insert(T x) {
		pnode tLeft = nullptr, tRight = nullptr;
		pnode tNew = new node(x, rng_());
		split(root_, x, tLeft, tRight);
		root_ = merge(tLeft, merge(tNew, tRight));
	}
	
	// erases the element
	void erase(T x) {
		pnode tLeft, tRight, tMid;
		tLeft = tRight = tMid = nullptr;
		split(root_, x, tLeft, tRight, true);
		split(tLeft, x, tLeft, tMid, false);
		delete tMid;
		root_ = merge(tLeft, tRight);
	}
	
  private:
	void split(pnode t, T x, pnode& t1, pnode& t2, bool xToLeft = false) {
		if(t == nullptr) {
			t2 = t1 = nullptr;
			return;
		}
		if((!xToLeft && x > t->val)||(xToLeft && x >= t->val)) {
			split(t->right, x, t->right, t2, xToLeft);
			t1 = t;
		} else {
			split(t->left, x, t1, t->left, xToLeft);
			t2 = t;
		}
	}
	
	pnode merge(pnode t1, pnode t2) {
		if(t1 == nullptr) return t2;
		if(t2 == nullptr) return t1;
		if(t1->prior > t2->prior) {
			t1->right = merge(t1->right, t2);
			return t1;
		} else {
			t2->left = merge(t1, t2->left);
			return t2;
		}
	}
	
	void printInternal(pnode t) {
		if(t == nullptr) return;
		printInternal(t->left);
		cout << t->val  << ' ';
		printInternal(t->right);
	}
};

int main() {
	
	vector<int> arr = {1, 2, 3};
	treap<int> t;
	
	for(int i = 5; i > 0; --i) t.insert(i);
	t.print();
	t.erase(3);
	t.print();
	
	treap<double> tt;
	tt.insert(2.0002);
	tt.insert(2.0004);
	tt.insert(2.0001);
	tt.insert(2.0003);
	tt.insert(2.0005);
	tt.print();
	tt.erase(2.0004);
	tt.print();
}