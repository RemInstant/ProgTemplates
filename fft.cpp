#include <iostream>
#include <vector>
#include <complex>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using vc = vector< complex<double> >;

const double PI = acos(-1l);
const double EPS = 1e-10;

void dft(vc& a, bool inverse) {
	ull n = a.size();
	if(n == 1) return;
	
	vc a0(0), a1(0);
	for(ull i = 0; i < n; ++i) {
		if(i % 2 == 0) a0.push_back(a[i]);
			else a1.push_back(a[i]);
	}
	
	dft(a0, inverse);
	dft(a1, inverse);
	
	complex<double> wk(1.0), w1;
	if(inverse) w1 = exp(-2*PI*1i / static_cast<double>(n));
		else w1 = exp(2*PI*1i / static_cast<double>(n));
	
	for(ull k = 0; 2*k < n; ++k) {
		ull j = k + n/2;
		a[k] = a0[k] + wk*a1[k];
		a[j] = a0[k] - wk*a1[k];
		if(inverse) {
			a[k] /= 2.0;
			a[j] /= 2.0;
		}
		wk *= w1;
	}
}

// computes the product of two polynomials in O(nlogn), where n is the maximum degree between them
vc fft(vc a, vc b) {
	ull mxsz = max(a.size(), b.size());
	ull n = 1;
	while(n < 2*mxsz) n *= 2;
	
	while(a.size() != n) a.push_back(0);
	while(b.size() != n) b.push_back(0);
	
	dft(a, false);
	dft(b, false);
	
	for(ull i = 0; i < n; ++i) a[i] *= b[i];
	dft(a, true);
	
	while(a.size() > 1 && abs(a.back()) < EPS) a.pop_back();
	
	return a;
}

#include <chrono>
struct timer {
	const chrono::steady_clock::time_point start = chrono::steady_clock::now();
	
	~timer() {
		const chrono::steady_clock::time_point end = chrono::steady_clock::now();
		const std::chrono::duration<double> t = end - start;
		cout << "passed time: " << t.count()*1000 << "ms\n";
	}
};

void compete(ull n) {
	vc p1(n), p2(n), ans;
	for(ull i = 0; i < n; ++i) {
		p1[i] = (n + i*i*n) % 128;
		p2[i] = (50*n + (i+30)*(i+1)) % 256 + 30;
	}
	
	cout << "poly(" << n << ") * poly(" << n << "):\n";
	cout << "processing naive algorithm\n";
	{
		timer t;
		ans.assign(p1.size()+p2.size()-1, 0);
		for(ull i = 0; i < p1.size(); ++i) {
			for(ull j = 0; j < p2.size(); ++j) {
				ans[i+j] += p1[i]*p2[j];
			}
		}
	}
	cout << "processing fft\n";
	{
		timer t;
		ans = fft(p1, p2);
	}
	cout << '\n';
}

int main() {
	
	
	vc p1 = {18, 5, 9, 3, 5};	// 18 +  5x +  9x^2 + 3x^3 + 5x^4
	vc p2 = {17, 13, 18};		// 17 + 13x + 18x^2
	vc ans;	// = p1 * p2 = 306 + 319x + 542x^2 + 258x^3 + 286x^4 + 119x^5 + 90x^6
	
	{
		timer t;
		ans = fft(p1, p2);
	}
	for(auto val : ans) cout << val.real() << ' ';
	cout << '\n';
	
	{
		timer t;
		ans.assign(p1.size()+p2.size()-1, 0);
		for(ull i = 0; i < p1.size(); ++i) {
			for(ull j = 0; j < p2.size(); ++j) {
				ans[i+j] += p1[i]*p2[j];
			}
		}
	}
	for(auto val : ans) cout << val.real() << ' ';
	cout << "\n\n";
	
	// Let's calc something a lot bigger
	compete(250);
	compete(500);
	compete(5000);
	compete(10000);
}