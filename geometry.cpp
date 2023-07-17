#include <iostream>
#include <vector>
#include <cmath>
 
using namespace std;

double PI = acos(-1);
 
struct vec2d {
	double x,y;
	
	vec2d(double _x = 0, double _y = 0) {
		x = _x;
		y = _y;
	}
	
	friend bool operator==(const vec2d& a, const vec2d& b) {
		return (a.x == b.x) && (a.y == b.y);
	}
	
	friend bool operator<(const vec2d& a, const vec2d& b) {
		return (a.x == b.x) ? (a.y < b.y) : (a.x < b.x);
	}
	
	friend vec2d operator+(const vec2d& a, const vec2d& b) {
		return vec2d(a.x+b.x, a.y+b.y);
	}
	
	friend vec2d operator-(const vec2d& a, const vec2d& b) {
		return vec2d(a.x-b.x, a.y-b.y);
	}
	
	friend vec2d operator*(double n, const vec2d& a) {
		return vec2d(a.x*n, a.y*n);
	}
	
	friend vec2d operator*(const vec2d& a, double n) {
		return vec2d(a.x*n, a.y*n);
	}
	
	friend ostream& operator<<(ostream& out, const vec2d& a) {
		out << '(' << a.x << ';' << a.y << ')';
		return out;
	}
	
	// scalar (dot) product
	friend double dot(const vec2d& a, const vec2d& b) {
		return a.x*b.x + a.y*b.y;
	}
	
	// vector (cross) product
	friend double cross(const vec2d& a, const vec2d& b) {
		return a.x*b.y - b.x*a.y;
	}
	
	double len() {
		return sqrt(dot(*this, *this));
	}
	
	// finds distance between two points
	friend double pointDist(const vec2d& a, const vec2d& b) {
		return sqrt((b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y));
	}
	
	// finds an angle between two vectors
	friend double angle(const vec2d& a, const vec2d& b) {
		double phi = atan2(cross(a,b), dot(a,b));
		return abs(phi);
	}
};


// finds the point on the line segment nearest to the given one (using ternary search)
// p --- the given point
// t1 -- the first segment end
// t1 -- the second segment end
// acc - accuracy of ternary search
vec2d NearestPointOfLineSeg(const vec2d& p, const vec2d& t1, const vec2d& t2, int acc = 50) {
	vec2d seg = t2 - t1;
	vec2d lp, rp;
	double ll = 0, rr = 1, ls, rs;
	for(int i = 0; i < acc; ++i) {
		double l = (2*ll + rr) / 3;
		double r = (ll + 2*rr) / 3;   
		lp = t1 + l*seg;
		rp = t1 + r*seg;      
		ls = pointDist(p, lp);
		rs = pointDist(p, rp);       
		if(ls < rs) {
			rr = r;
		} else {
			ll = l;
		}
	}
	return lp;
}

int main() {
	cout << "Hello geometry!\n";
}