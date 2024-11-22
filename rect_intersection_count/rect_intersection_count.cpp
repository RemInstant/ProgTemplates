#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using graph = vector< vector<int> >;
using pii = pair<int,int>;

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
    
    ll get_by_bounds(size_t id, size_t l, size_t r, ll ql, ll qr)
    {
        if (_data[id].max_x2 <= ql || _data[id].min_x1 >= qr) return 0;
        
        push(id, l, r);
        
        if (ql < _data[id].min_x2 && _data[id].max_x1 < qr)
        {
            return _data[id].value;
        }
        
		ull m = (l+r)/2;
		return get_by_bounds(id*2, l, m, ql, qr) + get_by_bounds(id*2+1, m+1, r, ql, qr);
	}
    
    void setInternal(size_t id, size_t l, size_t r, size_t q, ll x)
    {
		push(id, l, r);
        
		if (l == r)
        {
            _data[id].value = x;
            return;
		}
        
		ull m = (l+r)/2;
        
		if(q <= m)
        {
			setInternal(id*2, l, m, q, x);
		}
        else
        {
            setInternal(id*2+1, m+1, r, q, x);
        }
        
        _data[id].value = _data[id*2].value + _data[id*2+1].value;
	}
    
	void updateInternal(size_t id, size_t l, size_t r, ll ql, ll qr, ll x)
    {
        if (_data[id].max_x2 <= ql || _data[id].min_x1 >= qr) return;
        
        if (ql < _data[id].min_x2 && _data[id].max_x1 < qr)
        {
            _data[id].update += x;
            return;
        }
        
        push(id, l, r);
        
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

struct rect
{
    ll x1, y1, x2, y2;
};

struct event
{
    size_t rect_id;
    ll x, y1, y2;
    bool open;
};

struct y_interval
{
    size_t id;
    ll y1, y2;
};

vector<ll> dump_solve(vector<rect> rects)
{
    size_t n = rects.size();
    vector<ll> intersections(n, 0);
    
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = i+1; j < n; ++j)
        {
            rect &a = rects[i];
            rect &b = rects[j];
            if (a.x1 < b.x2 && a.x2 > b.x1 && a.y1 < b.y2 && a.y2 > b.y1)
            {
                intersections[i]++;
                intersections[j]++;
            }
        }
    }
    
    return intersections;
}

vector<ll> clever_solve(vector<rect> rects)
{
    size_t n = rects.size();
    
    vector<event> events(2*n);
    vector<y_interval> identified_y_intervals(n);
    vector<pair<ll,ll>> sorted_y_intervals(n);
    unordered_map<int, int> y_id_mapper;
    vector<ll> finalized_intersections(n, 0);
    
    clock_t timer = clock();
    
    for (size_t i = 0; i < n; ++i)
    {
        ll x1 = rects[i].x1;
        ll y1 = rects[i].y1;
        ll x2 = rects[i].x2;
        ll y2 = rects[i].y2;
        events[2*i+0] = { i, x1, y1, y2, true };
        events[2*i+1] = { i, x2, y1, y2, false };
        identified_y_intervals[i] = { i, y1, y2 };
    }
    
    sort(events.begin(), events.end(), [](event a, event b){
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
    
    sort(identified_y_intervals.begin(), identified_y_intervals.end(),
            [](y_interval a, y_interval b)
            {
                return a.id < b.id;
            });
    
    
    cout << "Data prepate time: " << ((double) (clock() - timer) / CLOCKS_PER_SEC) << " s" << endl;
    timer = clock();
    
    interval_tree y_interval_activity(sorted_y_intervals);
    segtree y_intersection_counter(sorted_y_intervals);
    
    cout << "Structure build time: " << ((double) (clock() - timer) / CLOCKS_PER_SEC) << " s" << endl;
    
    clock_t open_time = 0;
    clock_t close_time = 0;
    
    clock_t get_active_time = 0;
    clock_t update_prev_time = 0;
    clock_t set_cur_time = 0;
    clock_t set_active_time = 0;
    
    for (event e : events)
    {
        size_t id = y_id_mapper[e.rect_id];
        
        if (e.open)
        {
            timer = clock();
            ll cnt = y_interval_activity.count_intersections(e.y1, e.y2);
            // ll cnt = y_interval_activity.get_by_bounds(e.y1, e.y2);
            get_active_time += clock() - timer;
            
            timer = clock();
            y_intersection_counter.update_by_bounds(e.y1, e.y2, 1);
            update_prev_time += clock() - timer;
            
            timer = clock();
            y_intersection_counter.set_by_id(id, cnt);
            set_cur_time += clock() - timer;
            
            timer = clock();
            y_interval_activity.add_interval(e.y1, e.y2);
            // y_interval_activity.set_by_id(id, 1);
            set_active_time += clock() - timer;
        }
        else
        {
            timer = clock();
            finalized_intersections[e.rect_id] = y_intersection_counter.get_by_id(id);
            y_interval_activity.remove_interval(e.y1, e.y2);
            // y_interval_activity.set_by_id(id, 0);
            close_time += clock() - timer;
        }
    }
    
    open_time = get_active_time + update_prev_time + set_cur_time + set_active_time;
    
    cout << "Process opening time: " << ((double) open_time / CLOCKS_PER_SEC) << " s" << endl;
    cout << "| Getting active: " << ((double) get_active_time / CLOCKS_PER_SEC) << " s" << endl;
    cout << "| Updating previous: " << ((double) update_prev_time / CLOCKS_PER_SEC) << " s" << endl;
    cout << "| Setting current: " << ((double) set_cur_time / CLOCKS_PER_SEC) << " s" << endl;
    cout << "| Set active: " << ((double) set_active_time / CLOCKS_PER_SEC) << " s" << endl;
    cout << "Process closing time: " << ((double) close_time / CLOCKS_PER_SEC) << " s" << endl;
    
    return finalized_intersections;
}


int main() {
    clock_t timer;
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    #ifndef DEBUG
    #ifndef ONLINE_JUDGE
    freopen("rect_intersection_count/rect_test_data_2.txt", "r", stdin);
    freopen("_output.txt", "w", stdout);
    #endif
    #endif
    
    size_t n;
    cin >> n;
    
    size_t output_n = min(n, 10ull);
    vector<rect> rects(n);
    
    for (size_t i = 0; i < n; ++i)
    {
        cin >> rects[i].x1 >> rects[i].y1 >> rects[i].x2 >> rects[i].y2;
    }
    
    cout << fixed << setw(10);
    
    
    
    timer = clock();
    auto dump = dump_solve(rects);
    cout << "Dump time: " << ((double) (clock() - timer) / CLOCKS_PER_SEC) << " s" << endl;
    
    for (size_t i = 0; i < output_n; ++i)
    {
        cout << dump[i] << ' ';
    }
    cout << endl;
    
    
    
    timer = clock();
    auto clever = clever_solve(rects);
    cout << "Clever time: " << ((double) (clock() - timer) / CLOCKS_PER_SEC) << " s" << endl;
    
    for (size_t i = 0; i < output_n; ++i)
    {
        cout << clever[i] << ' ';
    }
    cout << endl;
    
    
    
    bool ok_flag = true;
    
    for (size_t i = 0; i < n; ++i)
    {
        ok_flag &= dump[i] == clever[i];
    }
    
    cout << (ok_flag ? "TEST PASSED" : "TEST FAILED") << endl;
}