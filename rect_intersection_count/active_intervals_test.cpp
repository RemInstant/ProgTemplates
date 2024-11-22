#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <iomanip>
#include <format>

#include <set>

using namespace std;
using ll = long long;
using ull = unsigned long long;


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
    
	ll get_by_bounds(ll l, ll r) { return get_by_bounds(1, 1, _n, l, r); }
	
	void set_by_id(size_t id, ll x) { setInternal(1, 1, _n, id+1, x); }
	
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
    
    ll get_by_bounds(size_t id, size_t l, size_t r, ll ql, ll qr) {
        push(id, l, r);
        
        if (l == r)
        {
            if (ql < _data[id].min_x2 && qr > _data[id].min_x1)
            {
                return _data[id].value;
            }
            return 0;
        }
        
        if (_data[id].max_x2 <= ql || _data[id].min_x1 >= qr) return 0;
        
        if (ql < _data[id].min_x2 && _data[id].max_x1 < qr)
        {
            return _data[id].value;
        }
        
		ull m = (l+r)/2;
		return get_by_bounds(id*2, l, m, ql, qr) + get_by_bounds(id*2+1, m+1, r, ql, qr);
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
        _data[id].value = _data[id*2].value + _data[id*2+1].value;
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


class interval_tree {

public:

    struct interval {
        ll l, r;
    };
    
    struct node {
        ll pivot;
        int count;
        std::vector<ll> left_sorted_intervals;
        std::vector<ll> right_sorted_intervals;
        
        node* left;
        node* right;
    };
    
    node *root = nullptr;
    
    interval_tree(vector<pair<ll, ll>> const &intervals)
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
        
        ll median = find_median(intervals);
        
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
                //including.push_back(inter);
            }
        }
        
        intervals.clear();
        
        node *n = new node();
        
        n->pivot = median;
        n->count = 0; // !!!! 
        
        vector<ll> lsi(including.size());
        vector<ll> rsi(including.size());
        
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
        n->right = build_node(greater);
        
        return n;
    }
    
    size_t count_intersections(ll x)
    {
        return count_intersections(x, root);
    }
    
    size_t count_intersections(ll l, ll r)
    {
        if (l == r)
        {
            return count_intersections(l, root);
        }
        
        return count_intersections(l, r, root);
    }
    
    void add_interval(ll l, ll r)
    {
        add_interval(l, r, root);
    }
    
    void remove_interval(ll l, ll r)
    {
        remove_interval(l, r, root);
    }
    
    size_t count_intersections(ll x, node *n)
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
    
    size_t count_intersections(ll l, ll r, node *n)
    {
        if (n == nullptr)
        {
            return 0;
        }
        
        if (l >= n->pivot)
        {
            size_t cnt = n->right_sorted_intervals.end() - upper_bound(
                    n->right_sorted_intervals.begin(), n->right_sorted_intervals.end(), l);
            
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
    
    void add_interval(ll l, ll r, node* n)
    {
        if (n == nullptr)
        {
            return;
        }
        
        if (l > n->pivot)
        {
            return add_interval(l, r, n->right);
        }
        
        if (r < n->pivot)
        {
            return add_interval(l, r, n->left);
        }
        
        n->left_sorted_intervals.insert(lower_bound(
            n->left_sorted_intervals.begin(), n->left_sorted_intervals.end(), l), l);
        n->right_sorted_intervals.insert(lower_bound(
            n->right_sorted_intervals.begin(), n->right_sorted_intervals.end(), r), r);
        
        n->count += 1;
    }
    
    void remove_interval(ll l, ll r, node* n)
    {
        if (n == nullptr)
        {
            return;
        }
        
        if (l > n->pivot)
        {
            return remove_interval(l, r, n->right);
        }
        
        if (r < n->pivot)
        {
            return remove_interval(l, r, n->left);
        }
        
        n->left_sorted_intervals.erase(lower_bound(
            n->left_sorted_intervals.begin(), n->left_sorted_intervals.end(), l));
        n->right_sorted_intervals.erase(lower_bound(
            n->right_sorted_intervals.begin(), n->right_sorted_intervals.end(), r));
        
        n->count -= 1;
    }
    
    ll find_median(vector<interval> const &intervals)
    {
        vector<ll> points(2 * intervals.size());
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            points[2*i] = intervals[i].l;
            points[2*i+1] = intervals[i].r;
        }
        
        auto m = points.begin() + points.size() / 2;
        nth_element(points.begin(), m, points.end());
        
        return points[points.size() / 2];
    }
    
};


class dump_array
{
    
private:

    struct Node {
        ll x1, x2;
        ll value;
    };

    vector<Node> _data;
    
public:

    dump_array(vector<pair<ll,ll>>const &a):
            _data(a.size())
    {
        for (size_t i = 0; i < a.size(); ++i)
        {
            _data[i].x1 = a[i].first;
            _data[i].x2 = a[i].second;
            _data[i].value = 0;
        }
    }
    
    ll get_by_id(size_t id)
    {
        return _data[id].value;
    }
    
    ll get_by_id_range(size_t id1, size_t id2)
    {
        ll ans = 0;
        for (size_t id = id1; id <= id2; ++id)
        {
            ans += _data[id].value;
        }
        return ans;
    }
    
    ll get_by_bounds(ll l, ll r)
    {
        ll ans = 0;
        for (size_t i = 0; i < _data.size(); ++i)
        {
            if (l < _data[i].x2 && r > _data[i].x1)
            {
                ans += _data[i].value;
            }
        }
        return ans;
    }
	
	void set_by_id(size_t id, ll x)
    {
        _data[id].value = x;
    }
    
    void update_by_bounds(ll l, ll r, ll x)
    {
        for (size_t i = 0; i < _data.size(); ++i)
        {
            if (l < _data[i].x2 && r > _data[i].x1)
            {
                _data[i].value += x;
            }
        }
    }
};


void test(vector<pair<ll,ll>> intervals, string msg, bool output_flag = false)
{
    cout << msg << endl;
    
    vector<ll> sg_tree_values, interval_tree_values, arr_values;
    sg_tree_values.reserve(2 * intervals.size());
    interval_tree_values.reserve(2 * intervals.size());
    arr_values.reserve(2 * intervals.size());
    
    clock_t __speedstart__ = clock();
    
    { /* Segment tree */ 
        segtree tree(intervals);
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            tree.set_by_id(i, 1);
            if (output_flag)
            {
                cout << format("set intervals[{}] to 1", i) << endl;
            }
        }
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            ll j = i+1;
            sg_tree_values.push_back(tree.get_by_bounds(-j, j));
            if (output_flag)
            {
                cout << format("active count on ({}; {}) is {}", -j, j, sg_tree_values.back())<< endl;
            }
        }
        
        for (size_t i = 0; i < intervals.size(); i += 3)
        {
            tree.set_by_id(i, 0);
            if (output_flag)
            {
                cout << format("set intervals[{}] to 0", i) << endl;
            }
        }
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            ll j = i+1;
            ll l = -((j*j*j + 2*j) % 20) + (2*j % 10);
            ll r = ((j*j - 2*j + 5) % 20) + (2*j % 10);
            sg_tree_values.push_back(tree.get_by_bounds(l, r));
            if (output_flag)
            {
                cout << format("active count on ({}; {}) is {}", l, r, sg_tree_values.back())<< endl;
            }
        }
    }
    
    cout << "segment tree time: " << fixed << setw(10) <<
            (double)(clock() - __speedstart__)/CLOCKS_PER_SEC << " s" << endl;
    
    __speedstart__ = clock();
    
    { /* Interval tree */
        interval_tree tree(intervals);
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            tree.add_interval(intervals[i].first, intervals[i].second);
            //tree.set_by_id(i, 1);
            if (output_flag)
            {
                cout << format("set intervals[{}] to 1", i) << endl;
            }
        }
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            ll j = i+1;
            interval_tree_values.push_back(tree.count_intersections(-j, j));
            if (output_flag)
            {
                cout << format("active count on ({}; {}) is {}", -j, j, interval_tree_values.back())<< endl;
            }
        }
        
        for (size_t i = 0; i < intervals.size(); i += 3)
        {
            tree.remove_interval(intervals[i].first, intervals[i].second);
            //tree.set_by_id(i, 0);
            if (output_flag)
            {
                cout << format("set intervals[{}] to 0", i) << endl;
            }
        }
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            ll j = i+1;
            ll l = -((j*j*j + 2*j) % 20) + (2*j % 10);
            ll r = ((j*j - 2*j + 5) % 20) + (2*j % 10);
            interval_tree_values.push_back(tree.count_intersections(l, r));
            if (output_flag)
            {
                cout << format("active count on ({}; {}) is {}", l, r, interval_tree_values.back())<< endl;
            }
        }
    }
    
    cout << "interval tree time: " << fixed << setw(10) <<
            (double)(clock() - __speedstart__)/CLOCKS_PER_SEC << " s" << endl;
    
    __speedstart__ = clock();
    
    { /* Dump array */
        dump_array arr(intervals);
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            arr.set_by_id(i, 1);
            if (output_flag)
            {
                cout << format("set intervals[{}] to 1", i) << endl;
            }
        }
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            ll j = i+1;
            arr_values.push_back(arr.get_by_bounds(-j, j));
            if (output_flag)
            {
                cout << format("active count on ({}; {}) is {}", -j, j, arr_values.back()) << endl;
            }
        }
        
        for (size_t i = 0; i < intervals.size(); i += 3)
        {
            arr.set_by_id(i, 0);
            if (output_flag)
            {
                cout << format("set intervals[{}] to 0", i) << endl;
            }
        }
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            ll j = i+1;
            ll l = -((j*j*j + 2*j) % 20) + (2*j % 10);
            ll r = ((j*j - 2*j + 5) % 20) + (2*j % 10);
            arr_values.push_back(arr.get_by_bounds(l, r));
            if (output_flag)
            {
                cout << format("active count on ({}; {}) is {}", l, r, arr_values.back()) << endl;
            }
        }
    }
    
    cout << "dump array time: " << fixed << setw(10) <<
            (double)(clock() - __speedstart__)/CLOCKS_PER_SEC << " s" << endl;
    
    __speedstart__ = clock();
    
    bool sg_ok_flag = true;
    bool interval_ok_flag = true;
    
    for (size_t i = 0; i < 2 * intervals.size(); ++i)
    {
        sg_ok_flag &= sg_tree_values[i] == arr_values[i];
        interval_ok_flag &= interval_tree_values[i] == arr_values[i];
        if (output_flag) cout << sg_tree_values[i] << ' ';
    }
    if (output_flag) cout << endl;
    for (size_t i = 0; i < 2 * intervals.size(); ++i)
    {
        if (output_flag) cout << arr_values[i] << ' ';
    }
    if (output_flag) cout << endl;
    
    if (!sg_ok_flag) cout << "SEGMENT TREE ERROR" << endl;
    if (!interval_ok_flag) cout << "INTERVAL TREE ERROR" << endl;
    
    cout << "tests time: " << fixed << setw(10) <<
            (double)(clock() - __speedstart__)/CLOCKS_PER_SEC << " s" << endl;
    cout << endl;
    cout.flush();
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    #ifndef DEBUG
    #ifndef ONLINE_JUDGE
    freopen("rect_intersection_count/intervals_test_data_2.txt", "r", stdin);
    freopen("_output.txt", "w", stdout);
    #endif
    #endif
    
    size_t n;
    cin >> n;
    
    vector<pair<ll, ll>> intervals(n);
    
    for (size_t i = 0; i < n; ++i)
    {
        cin >> intervals[i].first >> intervals[i].second;
        //cout << intervals[i-j].first << ' ' << intervals[i-j].second << endl;
    }
    
    test(intervals, "No sort");
    
    sort(intervals.begin(), intervals.end(), [](pair<ll,ll> a, pair<ll,ll> b) {
        if (a.first == b.first) return a.second < b.second;
        return a.first < b.first;
    });
    test(intervals, "x1 ASC sort then x2 ASC sort");
    
    sort(intervals.begin(), intervals.end(), [](pair<ll,ll> a, pair<ll,ll> b) {
        if (a.first == b.first) return a.second > b.second;
        return a.first < b.first;
    });
    test(intervals, "x1 ASC sort then x2 DESC sort");
    
    sort(intervals.begin(), intervals.end(), [](pair<ll,ll> a, pair<ll,ll> b) {
        if (a.second == b.second) return a.first < b.first;
        return a.second < b.second;
    });
    test(intervals, "x2 ASC sort then x1 ASC sort");
    
    sort(intervals.begin(), intervals.end(), [](pair<ll,ll> a, pair<ll,ll> b) {
        if (a.second == b.second) return a.first > b.first;
        return a.second < b.second;
    });
    test(intervals, "x2 ASC sort then x1 DESC sort");
}

