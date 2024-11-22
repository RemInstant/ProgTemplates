#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>

using namespace std;

using ll = long long;
using ull = unsigned long long;

ll generate()
{
    static mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    return ((ll) gen() % 200000) - 100000;
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
    
    size_t group_cnt = 100;
    size_t group_member_cnt = 1000;
    
    cout << group_cnt * group_member_cnt << endl;
    
    for (size_t i = 0; i < group_cnt; ++i)
    {
        ll x_offset = generate() * (generate() / 100);
        ll y_offset = generate() * (generate() / 100);
        
        for (size_t j = 0; j < group_member_cnt; ++j)
        {
            ll x1 = generate();
            ll y1 = generate();
            ll x2 = x1 + 1ll + (ll) abs(generate());
            ll y2 = y1 + 1ll + (ll) abs(generate());
            
            x1 += x_offset;
            y1 += y_offset;
            x2 += x_offset;
            y2 += y_offset;
            
            cout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << endl;
            
            if (x1 >= x2 || y1 >= y2)
            {
                cout << "ERROR" << endl;
                return 0;
            }
        }
    }
}