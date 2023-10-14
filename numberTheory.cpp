#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

// returns greatest common divisor of two numbers
ll gcd(ll a, ll b) {
	while(a != 0) {
		b %= a;
		swap(a, b);
	}
	return b;
}

// sieve of Eratosthenes O(nlog(logn))
// finds all prime numbers up to a given integer n
// returns vector in which [i]=1 means that i is a prime number
vector<char> sieve(ll n) {
	vector<char> r(n+1, 1);
	r[0] = 0; r[1] = 0;
	for (ll i = 2; i*i < n; ++i) {
		if(r[i] == 0) continue;
		for (ll j = i*i; j < n; j += i) {
			r[j] = 0;
		}
	}
	return r;
}

// binary power O(logn)
// returns x raised to the nth power modulo mod
ll bpow(ll x, ll n, ll mod = 1e18) {
	ll res = 1;
	while(n > 0) {
		if(n & 1) {
			res = res * x % mod;
		}
		x = x * x % mod;
		n = n >> 1;
	}
	return res;
}

// Euler's totient function O(√n)
// finds the number of positive integers up to a given
// integer n that are relatively prime to n
ll phi(ll n) {
	ll res = n;
	for (ll i = 2; i*i <= n; ++i) {
		if(n % i == 0) {
			while(n % i == 0) {
				n /= i;
			}
			res = res - res/i;
		}
	}
	if(n > 1) {
		res = res - res/n;
	}
	return res;
}

// finds the modular multiplicative inverse of a given integer x modulo mod
// (x * rev(x, mod) % mod = 1)
ll rev(ll x, ll mod, bool isPrime = false) {
	if(isPrime) return bpow(x, mod-2, mod);
	if(gcd(x, mod) != 1) return -1;
	return bpow(x, phi(mod)-1, mod);
}

int main() {
	for(int i = 1; i < 30; ++i) {
		cout << i << ' ' << phi(i) << '\n';
	}
}