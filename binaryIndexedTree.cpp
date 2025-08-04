#include <vector>
#include <stdio.h>
using namespace std;
/* A subarray sum dynamic data structure */

#define ll long long
class BinaryIndexedTree {
public:
    vector<ll> a;
    void init(vector<ll>& arr) {
        a.resize(arr.size()+1);
        a[0] = 0;
        a[1] = arr[0];
        for (int i=1; i<arr.size(); i++) {
            a[i+1] = a[i] + arr[i];
        }
        for (int i=a.size()-1; i>=1; i--) {
            ll prev = a[i - (i & -i)];
            a[i] = a[i] - prev;
        }
    }

    void update(int k, int val) {
        k++;
        while (k <= a.size()) {
            a[k] += val;
            k += k & -k;
        }
    }

    ll get(int k) {
        k++;
        ll res = 0;
        while (k >= 1) {
            res += a[k];
            k -= k & -k; 
        }
        return res;
    }

    void print() {
        printf("a: ");
        for (int i=0; i<a.size(); i++) 
            printf("%lld ", a[i]);
        printf("\n");
    }
};

int main() {
    vector<ll> arr = {1, 3, 4, 8, 6, 1, 4, 2};
    BinaryIndexedTree tree;
    tree.init(arr);
    tree.update(6, 3);
    printf("1: %lld\n", tree.get(6));
}