#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <unordered_map>
#include <algorithm>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using graph = vector< vector<int> >;
using pii = pair<int,int>;

template<typename T> 
class interval_tree {

public:

    struct interval {
        T l, r;
    };
    
    struct node {
        T pivot;
        int count;
        std::vector<T> left_sorted_intervals;
        std::vector<T> right_sorted_intervals;
        
        node* left;
        node* right;
    };
    
    node *root = nullptr;
    
    interval_tree(vector<pair<T, T>> const &intervals)
    {
        vector<interval> inter(intervals.size());
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            inter[i].l = intervals[i].first;
            inter[i].r = intervals[i].second;
        }
        
        root = build_node(inter);
    }
    
    node *build_node(vector<interval> &intervals)
    {
        if (intervals.empty())
        {
            return nullptr;
        }
        
        T median = find_median(intervals);
        
        vector<interval> less, including, greater;
        
        for (auto inter : intervals)
        {
            if (inter.r < median)
            {
                less.push_back(inter);
            }
            else if (inter.l > median)
            {
                greater.push_back(inter);
            }
            else
            {
                including.push_back(inter);
            }
        }
        
        intervals.clear();
        
        node *n = new node();
        
        n->pivot = median;
        n->count = 0; // !!!! 
        
        vector<T> lsi(including.size());
        vector<T> rsi(including.size());
        
        for (size_t i = 0; i < including.size(); ++i)
        {
            lsi[i] = including[i].l;
            rsi[i] = including[i].r;
        }
        
        sort(lsi.begin(), lsi.end());
        sort(rsi.begin(), rsi.end());
        
        n->left_sorted_intervals = std::move(lsi);
        n->right_sorted_intervals = std::move(rsi);
        
        n->left = build_node(less);
        n->left = build_node(greater);
        
        return n;
    }
    
    size_t count_intersections(T x)
    {
        return count_intersections(x, root);
    }
    
    size_t count_intersections(T l, T r)
    {
        if (l == r)
        {
            return count_intersections(l, root);
        }
        
        return count_intersections(l, r, root);
    }
    
    void add_interval(T l, T r, int addition)
    {
        add_interval(l, r, addition, root);
    }
    
    size_t count_intersections(T x, node *n)
    {
        if (n == nullptr)
        {
            return 0;
        }
        
        if (x > n->pivot)
        {
            size_t cnt = n->right_sorted_intervals.end() - upper_bound(
                    n->right_sorted_intervals.begin(), n->left_sorted_intervals.end(), x);
            
            return cnt + count_intersections(x, n->right);
        }
        
        if (x < n->pivot)
        {
            size_t cnt = lower_bound(n->left_sorted_intervals.begin(),
                    n->left_sorted_intervals.end(), x) - n->left_sorted_intervals.begin();
            
            return cnt + count_intersections(x, n->left);
        }

        size_t tmp1 = lower_bound(n->left_sorted_intervals.begin(),
                n->left_sorted_intervals.end(), x) - n->left_sorted_intervals.begin();
        
        size_t tmp2 = n->right_sorted_intervals.end() - upper_bound(
                n->right_sorted_intervals.begin(), n->left_sorted_intervals.end(), x);
        
        return tmp2 - tmp1;
    }
    
    size_t count_intersections(T l, T r, node *n)
    {
        if (n == nullptr)
        {
            return 0;
        }
        
        if (l >= n->pivot)
        {
            size_t cnt = n->right_sorted_intervals.end() - upper_bound(
                    n->right_sorted_intervals.begin(), n->left_sorted_intervals.end(), l);
            
            return cnt + count_intersections(l, r, n->right);
        }
        
        if (r <= n->pivot)
        {
            size_t cnt = lower_bound(n->left_sorted_intervals.begin(),
                    n->left_sorted_intervals.end(), r) - n->left_sorted_intervals.begin();
            
            return cnt + count_intersections(l, r, n->left);
        }
        
        return n->count + count_intersections(l, r, n->left)
                + count_intersections(l, r, n->right);
    }
    
    void add_interval(T l, T r, int addition, node* n)
    {
        if (n == nullptr)
        {
            return;
        }
        
        if (l >= n->pivot)
        {
            return add_interval(l, r, addition, n->right);
        }
        
        if (r <= n->pivot)
        {
            return add_interval(l, r, addition, n->left);
        }
        
        n->count += addition;
    }
    
    T find_median(vector<interval> const &intervals)
    {
        vector<T> points(2 * intervals.size());
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            points[2*i] = intervals[i].l;
            points[2*i+1] = intervals[i].r;
        }
        
        sort(points.begin(), points.end());
        
        return points[intervals.size()];
    }
    
};

class segtree {
	
private:

    struct Node {
        ll min_x1, min_x2;
        ll max_x1, max_x2;
        ll value, update;
    };
  
  
	size_t _n;
	vector<Node> _data;
	
public:
	segtree(size_t n): _n(n), _data(4 * n, Node()) {}
	
	segtree(const vector<pair<ll,ll>>& a): segtree(a.size()) { build(1, 1, _n, a); }
	
	ll get_by_id(size_t id) { return getInternal(1, 1, _n, id+1, id+1); }
    
	ll get_by_id_range(size_t id1, size_t id2) { return getInternal(1, 1, _n, id1+1, id2+1); }
	
	void set_by_id(size_t id, ll x) { setInternal(1, 1, _n, id, x); }
	
	void update_by_bounds(ll l, ll r, ll x) { updateInternal(1, 1, _n, l, r, x); }
	
  private:
	void build(size_t id, size_t l, size_t r, const vector<pair<ll,ll>>& a) {
		if(l == r) {
            _data[id].min_x1 = _data[id].max_x1 = a[l-1].first;
            _data[id].min_x2 = _data[id].max_x2 = a[l-1].second;
			_data[id].value = _data[id].update = 0;
			return;
		}
        
		size_t m = (l+r)/2;
		build(id*2, l, m, a);
		build(id*2+1, m+1, r, a);
        
        _data[id].min_x1 = min(_data[id*2].min_x1, _data[id*2+1].min_x1);
        _data[id].max_x1 = max(_data[id*2].max_x1, _data[id*2+1].max_x1);
        _data[id].min_x2 = min(_data[id*2].min_x2, _data[id*2+1].min_x2);
        _data[id].max_x2 = max(_data[id*2].max_x2, _data[id*2+1].max_x2);
        _data[id].value = _data[id].update = 0;
	}
	
	void push(size_t id, size_t l, size_t r) {
        if (_data[id].update == 0) return;
		if (id*2 < 4*_n) _data[id*2].update += _data[id].update;
		if (id*2+1 < 4*_n) _data[id*2+1].update += _data[id].update;
        _data[id].value += _data[id].update * (r - l + 1);
		_data[id].update = 0;
	}
	
	ll getInternal(size_t id, size_t l, size_t r, size_t ql, size_t qr) {
        push(id, l, r);
		if(ql <= l && qr >= r) {
			return _data[id].value;
		}
        
		size_t m = (l+r)/2;
        
		if (qr <= m  ) return getInternal(id*2, l, m, ql, qr);
		if (ql >= m+1) return getInternal(id*2+1, m+1, r, ql, qr);
        
		return getInternal(id*2, l, m, ql, qr) + getInternal(id*2+1, m+1, r, ql, qr);
	}
    
    
	
    void setInternal(size_t id, size_t l, size_t r, size_t q, ll x) {
		push(id, l, r);
		if(l == r) {
            _data[id].value = x;
            return;
		}
		ull m = (l+r)/2;
		if(q <= m) {
			setInternal(id*2, l, m, q, x);
		} else {
            setInternal(id*2+1, m+1, r, q, x);
        }
	}
    
	void updateInternal(size_t id, size_t l, size_t r, ll ql, ll qr, ll x) {
		push(id, l, r);
        
        if (l == r)
        {
            if (ql < _data[id].min_x2 && qr > _data[id].min_x1)
            {
                _data[id].update += x;
            }
            return;
        }
        
        if (_data[id].max_x2 <= ql || _data[id].min_x1 >= qr) return;
        
        if (ql < _data[id].min_x2 && _data[id].max_x1 < qr)
        {
            _data[id].update += x;
            push(id, l, r);
            return;
        }
        
		ull m = (l+r)/2;
		updateInternal(id*2, l, m, ql, qr, x);
		updateInternal(id*2+1, m+1, r, ql, qr, x);
	}
};

struct event
{
    int x, y1, y2;
    bool open;
};

struct y_interval
{
    size_t id;
    int y1, y2;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    #ifndef DEBUG
    #ifndef ONLINE_JUDGE
    freopen("_input.txt", "r", stdin);
    freopen("_output.txt", "w", stdout);
    #endif
    #endif
    
    size_t n;
    cin >> n;
    
    vector<event> e(2*n);
    vector<pair<ll,ll>> y_intervals(n);
    vector<y_interval> identified_y_intervals(n);
    vector<pair<ll,ll>> sorted_y_intervals(n);
    unordered_map<int, int> y_id_mapper;
    
    for (size_t i = 0; i < n; ++i)
    {
        int x1,y1,x2,y2;
        cin >> x1 >> y1 >> x2 >> y2;
        
        e[2*i+0] = { x1, y1, y2, true };
        e[2*i+1] = { x2, y1, y2, false };
        y_intervals[i] = { y1, y2 };
        identified_y_intervals[i] = { i, y1, y2 };
    }
    
    sort(e.begin(), e.end(), [](event a, event b){
        if (a.x == b.x)
        {
            return a.open < b.open;
        }
        return a.x < b.x;
    });
    
    sort(identified_y_intervals.begin(), identified_y_intervals.end(),
            [](y_interval a, y_interval b)
            {
                if (a.y1 == b.y1) return a.y2 < b.y2;
                return a.y1 < b.y1;
            });
    
    for (size_t i = 0; i < identified_y_intervals.size(); ++i)
    {
        y_interval interval = identified_y_intervals[i];
        y_id_mapper[interval.id] = i;
        sorted_y_intervals[i] = { interval.y1, interval.y2 };
    }
    
    
    interval_tree<ll> itr(y_intervals);
    segtree y_counter(sorted_y_intervals);
    
    cout << itr.count_intersections(-3, -1) << endl;
    cout << itr.count_intersections(-100, 100) << endl;
    cout << itr.count_intersections(-1, -1) << endl;
    cout << itr.count_intersections(-1, 1) << endl;
    
    
}