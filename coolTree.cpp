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


class CoolTree
{

private:

    struct Node
    {
        ll l, r;                // min left and max right bound in subtree
        ll pivot;               // pivot point (left ~ <= than pivot; right ~ >= than pivot)
        map<ll, ll> kvp_map;    // map for equal intervals
        ll update;              // delayed update
    };
    
    vector<Node> _data;
    
public:
    
    CoolTree(vector<pair<ll, ll>> const &intervals):
            _data(9 * intervals.size())
    {
        vector<Node> raw_data(intervals.size()); // 3? maybe less
        
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            raw_data[i].l = intervals[i].first;
            raw_data[i].r = intervals[i].second;
            raw_data[i].kvp_map[i] = 0;
            raw_data[i].update = 0;
        }
        
        build(raw_data, 0);
    }
    
    bool test(vector<pair<ll, ll>> const &intervals)
    {
        for (size_t i = 0; i < _data.size(); ++i)
        {
            for (size_t j = 0; j < intervals.size(); ++j)
            {
                if (_data[i].l == intervals[j].first && _data[i].r == intervals[j].second)
                {
                    if (!_data[i].kvp_map.contains(j))
                    {
                        return false;
                    }
                }
            }
        }
        
        return true;
    }


private:

    void build(vector<Node> &raw_data, size_t idx)
    {
        if (raw_data.empty())
        {
            return;
        }
        
        vector<Node> raw__datato_left;
        vector<Node> raw__datato_right;
        vector<Node> raw__datato_bottom;
        
        _data[idx].pivot = find_median(raw_data);
        
        for (size_t i = 0; i < raw_data.size(); ++i)
        {
            if (raw_data[i].r <= _data[idx].pivot)
            {
                raw__datato_left.push_back(std::move(raw_data[i]));
            }
            else if (raw_data[i].l >= _data[idx].pivot)
            {
                raw__datato_right.push_back(std::move(raw_data[i]));
            }
            else
            {
                raw__datato_bottom.push_back(std::move(raw_data[i]));
            }
        }
        
        raw_data.clear();
        
        if (raw__datato_left.empty() && raw__datato_bottom.empty())
        {
            _data[idx].l = raw__datato_right[0].l;
            _data[idx].r = raw__datato_right[0].r;
            
            for (Node node : raw__datato_right)
            {
                for (auto kvp : node.kvp_map)
                {
                    _data[idx].kvp_map.insert(kvp);
                }
            }
        }
        else
        {
            build(raw__datato_left,   3*idx + 1);
            build(raw__datato_bottom, 3*idx + 2);
            build(raw__datato_right,  3*idx + 3);
            
            _data[idx].l = min(_data[3*idx + 1].l, min(_data[3*idx + 2].l, _data[3*idx + 3].l));
            _data[idx].r = max(_data[3*idx + 1].r, max(_data[3*idx + 2].r, _data[3*idx + 3].r));
        }
    }
    
    ll find_median(vector<Node> const &intervals) // TODO speed up
    {
        // vector<ll> points(2 * intervals.size());
        
        // for (size_t i = 0; i < intervals.size(); ++i)
        // {
        //     points[2*i] = intervals[i].l;
        //     points[2*i+1] = intervals[i].r;
        // }
        
        // sort(points.begin(), points.end());
        
        // return points[intervals.size()];
        
        ll sum = 0;
        
        for (Node node : intervals)
        {
            sum += node.l + node.r;
        }
        
        double mean = 0.5 * sum / intervals.size();
        ll pivot = intervals[0].l;
        
        for (Node node : intervals)
        {
            if (abs(mean - node.l) < abs(mean - pivot))
            {
                pivot = node.l;
            }
            if (abs(mean - node.r) < abs(mean - pivot))
            {
                pivot = node.r;
            }
        }
        
        return pivot;
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
    
    clock_t __speedstart__ = clock();
    
    segtree tree(intervals);
    
    for (size_t i = 0; i < intervals.size(); ++i)
    {
        ll j = i+1;
        tree.update_by_bounds(j, j+1, 1);
        tree.update_by_bounds(-(j*j % 20), (j*j % 20), 1);
        if (output_flag)
        {
            cout << format("add 1 on ({}, {})", j, j+1) << endl;
            cout << format("add 1 on ({}, {})", -(j*j % 20), (j*j % 20)) << endl;
        }
    }
    
    for (size_t i = 0; i < intervals.size(); ++i)
    {
        ll q = tree.get_by_id(i);
        if (output_flag) cout << q << ' ';
    }
    if (output_flag) cout << endl;
    
    cout << "tree time: " << fixed << setw(10) <<
            (double)(clock() - __speedstart__)/CLOCKS_PER_SEC << " s" << endl;
    
    __speedstart__ = clock();
    
    dump_array arr(intervals);
    
    for (size_t i = 0; i < intervals.size(); ++i)
    {
        ll j = i+1;
        arr.update_by_bounds(j, j+1, 1);
        arr.update_by_bounds(-(j*j % 20), (j*j % 20), 1);
    }
    
    bool ok_flag = true;
    
    for (size_t i = 0; i < intervals.size(); ++i)
    {
        ok_flag &= tree.get_by_id(i) == arr.get_by_id(i);
        if (output_flag) cout << arr.get_by_id(i) << ' ';
    }
    if (output_flag) cout << endl;
    
    if (!ok_flag) cout << "ERROR" << endl;
    
    cout << "tests time: " << fixed << setw(10) <<
            (double)(clock() - __speedstart__)/CLOCKS_PER_SEC << " s" << endl << endl;
    cout.flush();
}

int main()
{
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
    
    vector<pair<ll, ll>> intervals(n);
    
    for (size_t i = 0; i < n; ++i)
    {
        ll q;
        cin >> intervals[i].first >> q >> intervals[i].second >> q;
    }
    
    
    //test(intervals, "No sort");
    
    sort(intervals.begin(), intervals.end(), [](pair<ll,ll> a, pair<ll,ll> b) {
        if (a.first == b.first) return a.second < b.second;
        return a.first < b.first;
    });
    test(intervals, "x1 ASC sort then x2 ASC sort");
    
    // sort(intervals.begin(), intervals.end(), [](pair<ll,ll> a, pair<ll,ll> b) {
    //     if (a.first == b.first) return a.second > b.second;
    //     return a.first < b.first;
    // });
    // test(intervals, "x1 ASC sort then x2 DESC sort");
    
    // sort(intervals.begin(), intervals.end(), [](pair<ll,ll> a, pair<ll,ll> b) {
    //     if (a.second == b.second) return a.first < b.first;
    //     return a.second < b.second;
    // });
    // test(intervals, "x2 ASC sort then x1 ASC sort");
    
    // sort(intervals.begin(), intervals.end(), [](pair<ll,ll> a, pair<ll,ll> b) {
    //     if (a.second == b.second) return a.first > b.first;
    //     return a.second < b.second;
    // });
    // test(intervals, "x2 ASC sort then x1 DESC sort");
    
    
}

