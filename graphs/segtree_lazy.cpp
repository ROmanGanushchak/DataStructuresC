#include <cassert>
#include <vector>
#include <stdio.h>
using namespace std;

#define ll long long
class LazySegTree {
private:
    void push(int i, int l, int r) {
        v[i] += lazy[i] * (r - l + 1); // !
        if (l != r) {
            lazy[2*i] += lazy[i];
            lazy[2*i+1] += lazy[i];
        }
        lazy[i] = 0;
    }

    void update(int ql, int qr, int i, int l, int r, ll val) {
        push(i, l, r);
        if (l >= ql && r <= qr) {
            lazy[i] += val; // !
            push(i, l, r);
            return;
        }
        if (l > r || l > qr || ql > r) return;
        assert(l != r);
        int m = (l + r) >> 1;
        update(ql, qr, 2*i, l, m, val);
        update(ql, qr, 2*i+1, m+1, r, val);
        v[i] = v[2*i] + v[2*i+1]; // !
    }

    ll get(int ql, int qr, int i, int l, int r) {
        push(i, l, r);
        if (l >= ql && r <= qr)
            return v[i];
        if (l > r || l > qr || ql > r)
            return 0; // !
        assert(l != r);
        int m = (l + r) >> 1;
        return get(ql, qr, 2*i, l, m) +
            get(ql, qr, 2*i+1, m+1, r);
    }
public:
    int n, _n;
    vector<ll> lazy, v;
    void init(const vector<ll>& arr) {
        _n = (ll) arr.size();
        for (n = 1; n < _n; n <<= 1);
        v.resize(2*n);
        lazy.resize(2*n);
        fill(lazy.begin(), lazy.end(), 0);
        for (int i=0; i<arr.size(); i++)
            v[i+n] = arr[i];
        fill(v.begin() + n + arr.size(), v.end(), 0); // !
        for (int i=n-1; i>=1; i--)
            v[i] = v[2*i] + v[2*i+1]; // !
    }

    ll query(int ql, int qr) {
        return get(ql, qr, 1, 0, n-1);
    }

    void update(int ql, int qr, ll add) {
        update(ql, qr, 1, 0, n-1, add);
    }
};

int main() {
    vector<ll> arr = {5, 8, 6, 3, 2, 7, 2, 6};
    LazySegTree t;
    t.init(arr);
    t.update(0, 4, 2);
    for (int i=0; i<arr.size(); i++)
        printf("%lld ", t.query(i, i));
    printf("\n");
}