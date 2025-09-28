#include <vector>
#include <stdio.h>
using namespace std;

#define ll long long
#define llu unsigned long long
class SegTree {
public:
    int n;
    vector<ll> v;
    void init(const vector<ll>& arr) {
        ll _n = (ll) arr.size();
        for (n = 1; n < _n; n <<= 1);
        v.resize(n + arr.size());
        for (int i=0; i<_n; i++)
            v[i+n] = arr[i];
        for (int i=n-1; i>=1; i--) 
            v[i] = v[2*i] + v[2*i+1];
    }

    ll get(int a, int b) {
        a += n; b += n;
        ll res = 0;
        while (a <= b) {
            if (a & 1) res += v[a++];
            if ( !(b & 1) ) res += v[b--];
            a >>= 1; b>>= 1;
        }
        return res;
    }

    void update(int idx, ll val) {
        idx += n;
        v[idx] = val;
        for (idx >>= 1; idx >= 1; idx >>= 1)
            v[idx] = v[2*idx] + v[2*idx + 1];
    }
};

int main() {
    vector<ll> arr = {5, 8, 6, 3, 2, 7, 2, 6};
    SegTree t;
    t.init(arr);
    printf("1: %lld\n", t.get(5, 7));
}