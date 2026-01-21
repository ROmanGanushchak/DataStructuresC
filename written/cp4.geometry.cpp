// #include <bits/stdc++.h>
#include <bitset>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <numeric>
#include <cmath>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()
using namespace std;

struct point {
    double x, y;
    point(double x, double y) : x(x), y(y) {}
};
struct point_i {
    int x, y;
    point_i(int x, int y) : x(x), y(y) {}
};
const double EPS = 1e-9;

// LINE
// line formula ax + by + c == 0, if b is zero, line is vertical
struct line { double a, b, c; };
void pointsToLine(const point &p1, const point &p2, line &l) {
    if (fabs(p1.x-p2.x) < EPS) // vertical line
        l = {1.0, 0.0, -p1.x}; // default values
    else
        l = {-(double)(p1.y-p2.y) / (p1.x-p2.x),
        1.0, // IMPORTANT: b = 1.0
        -(double)(l.a*p1.x) - p1.y};
}
bool areParallel(line l1, line l2) { // check a & b
    return (fabs(l1.a-l2.a) < EPS) && (fabs(l1.b-l2.b) < EPS);
}
bool areSame(line l1, line l2) { // also check c
    return areParallel(l1, l2) && (fabs(l1.c-l2.c) < EPS);
}

bool areIntersect(line l1, line l2, point &p) {
    if (areParallel(l1, l2)) return false; // no intersection
    // solve system of 2 linear algebraic equations with 2 unknowns
    p.x = (l2.b*l1.c - l1.b*l2.c) / (l2.a*l1.b - l1.a*l2.b);
    // special case: test for vertical line to avoid division by zero
    if (fabs(l1.b) > EPS) p.y = -(l1.a*p.x + l1.c);
    else p.y = -(l2.a*p.x + l2.c);
    return true;
}

// VECTOR
struct vec { double x, y; // name: ‘vec’ is different from STL vector
    vec(double _x, double _y) : x(_x), y(_y) {}
};
vec toVec(const point &a, const point &b) { // convert 2 points
    return vec(b.x-a.x, b.y-a.y); // to vector a->b
}
vec scale(const vec &v, double s) { // s = [<1..1..>1]
    return vec(v.x*s, v.y*s); // shorter/eq/longer
} // return a new vec
point translate(const point &p, const vec &v) { // translate p
    return point(p.x+v.x, p.y+v.y); // according to v
}

double dot(vec a, vec b) { return (a.x*b.x + a.y*b.y); }
double norm_sq(vec v) { return v.x*v.x + v.y*v.y; }
double dist(const point& a, const point& b) { return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y)); }

double angle(const point &a, const point &o, const point &b) {
    vec oa = toVec(o, a), ob = toVec(o, b); // a != o != b
    return acos(dot(oa, ob) / sqrt(norm_sq(oa) * norm_sq(ob)));
}

double cross(vec a, vec b) { return a.x*b.y - a.y*b.x; }

// returns true if point r is on the left side of line pq
bool ccw(point p, point q, point r) {
    return cross(toVec(p, q), toVec(p, r)) > EPS;
}
// returns true if point r is on the same line as the line pq
bool collinear(point p, point q, point r) {
    return fabs(cross(toVec(p, q), toVec(p, r))) < EPS;
}

// returns the distance from p to the line defined by
// two points a and b (a and b must be different)
// the closest point is stored in the 4th parameter (byref)
double distToLine(point p, point a, point b, point &c) {
    vec ap = toVec(a, p), ab = toVec(a, b);
    double u = dot(ap, ab) / norm_sq(ab);
    // formula: c = a + u*ab
    c = translate(a, scale(ab, u)); // translate a to c
    return dist(p, c); // Euclidean distance
}

// returns the distance from p to the line segment ab defined by
// two points a and b (technically, a has to be different than b)
// the closest point is stored in the 4th parameter (byref)
double distToLineSegment(point p, point a, point b, point &c) {
    vec ap = toVec(a, p), ab = toVec(a, b);
    double u = dot(ap, ab) / norm_sq(ab);
    if (u < 0.0) { // closer to a
        c = point(a.x, a.y);
        return dist(p, a); // dist p to a
    }
    if (u > 1.0) { // closer to b
        c = point(b.x, b.y);
        return dist(p, b); // dist p to b
    }
    return distToLine(p, a, b, c); // use distToLine
}

// CIRCLE
int insideCircle(const point &p, const point &c, double r) {
    int dx = p.x-c.x, dy = p.y-c.y;
    int Euc = dx*dx + dy*dy, rSq = r*r; // all integer
    return Euc < rSq ? 1 : (Euc == rSq ? 0 : -1); // in/border/out
}

// returns a center of circle that contains 2 provided points
bool circle2PtsRad(point p1, point p2, double r, point &c) {
    double d2 = (p1.x-p2.x) * (p1.x-p2.x) + (p1.y-p2.y) * (p1.y-p2.y);
    double det = r*r/d2 - 0.25;
    if (det < EPS) return false;
    double h = sqrt(det);
    // to get the other center, reverse p1 and p2
    c.x = (p1.x+p2.x) * 0.5 + (p1.y-p2.y) * h;
    c.y = (p1.y+p2.y) * 0.5 + (p2.x-p1.x) * h;
    return true;
}

// TRIANGLE
/*
    4. A triangle with three sides: a, b, c has perimeter p = a + b + c and semi-perimeter s = 0.5 ⇥ p.
    5. A triangle with 3 sides: a, b, c and semi-perimeter s has area
    A = p(s ⇥ (s  a) ⇥ (s  b) ⇥ (s  c)) ||| This formula is called the Heron’s Formula
*/

// Poligon
// returns the area of polygon P
double area(const vector<point> &P) {
    double ans = 0.0;
    for (int i = 0; i < (int)P.size()-1; ++i) // Shoelace formula
        ans += (P[i].x*P[i+1].y - P[i+1].x*P[i].y);
    return fabs(ans)/2.0; // only do / 2.0 here
}

// returns true if we always make the same turn
// while examining all the edges of the polygon one by one
bool isConvex(const vector<point> &P) {
    int n = (int)P.size();
    // a point/sz=2 or a line/sz=3 is not convex
    if (n <= 3) return false;
    bool firstTurn = ccw(P[0], P[1], P[2]); // remember one result,
    for (int i = 1; i < n-1; ++i) // compare with the others
        if (ccw(P[i], P[i+1], P[(i+2) == n ? 1 : i+2]) != firstTurn)
    return false; // different -> concave
    return true; // otherwise -> convex
}