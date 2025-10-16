#include <cassert>
#include <vector>
#include <stdio.h>
using namespace std;

#define ll long long
class LazySegTree {
private:
    void push(int i, int l, int r) {
        v[i] += lazy[i]; // !
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
        v[i] = max(v[2*i], v[2*i+1]); // !
    }

    // assumes that there is always at least one negative value, for min_seg only
    int first_neg(int l, int r, int i) {
        push(i, l, r);
        if (l == r) {
            assert(v[i] < 0);
            return l;
        }
        int m = (l + r) >> 1;
        int ans;
        if (v[2*i]+lazy[2*i] < 0)
            ans = first_neg(l, m, 2*i);
        else
            ans = first_neg(m+1, r, 2*i+1);
        v[i] = min(v[2*i]+lazy[2*i], v[2*i+1]+lazy[2*i+1]);
        return ans;
    }

    ll get_max(int ql, int qr, int i, int l, int r) {
        push(i, l, r);
        if (l >= ql && r <= qr)
            return v[i];
        if (l > r || l > qr || ql > r)
            return LLONG_MIN; // !
        assert(l != r);
        int m = (l + r) >> 1;
        return max(get_max(ql, qr, 2*i, l, m),
            get_max(ql, qr, 2*i+1, m+1, r));
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
        fill(v.begin() + n + arr.size(), v.end(), LLONG_MIN);
        for (int i=n-1; i>=1; i--)
            v[i] = max(v[2*i], v[2*i+1]); // !
    }

    int first_neg() { return first_neg(0, n-1, 1); }
    ll get_max(int ql, int qr) { return get_max(ql, qr, 1, 0, n-1); }
    void update(int ql, int qr, ll add) { update(ql, qr, 1, 0, n-1, add); }
};

int main() {
    vector<ll> arr = {5, 8, 6, 3, 2, 7, 2, 6};
    LazySegTree t;
    t.init(arr);
    t.update(5, 7, 1);
    t.update(5, 7, -1);
    t.update(0, 4, 10);
    t.update(0, 4, -10);
    t.update(0, 4, 10);
    t.update(0, 4, -10);
    for (int i=0; i<arr.size(); i++)
        printf("%lld ", t.get_max(i, i));
    printf("\n");
}