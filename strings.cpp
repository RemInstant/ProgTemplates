#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

// z-function
// finds z-function of the given string
// (z[i] = max x < n-i+1, such that [0; x-1] segment equals to [i; i+x-1] segment)
vector<int> zf(string s) {
	int n = s.size();
	vector<int> z(n, 0);
	z[0] = n;
	int r = 0, l = 0; 	// [l; r] - the boundaries of the rightmost suffix prefix
						// that is equal to a corresponding string prefix
						// [0; r] - processed string prefix
	for(int i = 1; i < n; ++i) {
		if(i <= r) {
			// i in [0; r], already processed data can be used
			z[i] = min(z[i-l], r-i+1);
		}
		while(i+z[i] < n && s[z[i]] == s[i+z[i]]) {
			// default comparisons
			++z[i];
		}
		if(i+z[i]-1 > r) {
			// updating [l; r]
			l = i;
			r = i+z[i]-1;
		}
	}
	return z;
}

// pi-function
// finds pi-function of the given string
// (p[i] = max x < i+1, such that [0; x-1] segment equals to [i-x+1; i] segment)
vector<int> pf(string s) {
	int n = s.size();
	vector<int> p(n, 0);
	for(int i = 1; i < n; ++i) {
		int j = p[i-1];
		while(j > 0 && s[j] != s[i]) {
			j = p[j-1];
		}
		if(s[j] == s[i]) ++j;
		p[i] = j;
	}
	return p;
}

// TODO Rabin-Karp?

int main() {
	string test = "aabaababaaba";
	vector<int> zt = zf(test);
	vector<int> pt = pf(test);
	
	cout << ' ';
	for(size_t i = 0; i < test.size(); ++i) cout << test[i] << ' ';
	cout << '\n';
	for(size_t i = 0; i < test.size(); ++i) cout << zt[i] << ' ';
	cout << '\n' << '\n' << ' ';
	for(size_t i = 0; i < test.size(); ++i) cout << test[i] << ' ';
	cout << '\n' << ' ';
	for(size_t i = 0; i < test.size(); ++i) cout << pt[i] << ' ';
	cout << '\n' << '\n';
	
	string pattern = "aaba";
	
	string search = pattern + "$" + test;
	vector<int> z = zf(search);
	vector<int> p = pf(search);
	
	cout << ' ';
	for(size_t i = 0; i < search.size(); ++i) cout << search[i] << ' ';
	cout << '\n';
	for(size_t i = 0; i < search.size(); ++i) cout << z[i] << ' ';
	cout << '\n' << '\n' << ' ';
	for(size_t i = 0; i < search.size(); ++i) cout << search[i] << ' ';
	cout << '\n' << ' ';
	for(size_t i = 0; i < search.size(); ++i) cout << p[i] << ' ';
	cout << '\n' << '\n';
	
	for(size_t i = 0; i < search.size(); ++i) {
		if(z[i] == (int) pattern.size()) cout << "matching starts at " << i << '\n';
		if(p[i] == (int) pattern.size()) cout << "matching ends at " << i << '\n';
	}
}