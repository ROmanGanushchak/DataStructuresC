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
        for (k++; k < a.size(); k += k & -k)
            a[k] += val;
    }

    ll get(int k) {
        ll res = 0;
        for (k++; k >= 1; k -= k & -k)
            res += a[k];
        return res;
    }

    // returns the first index in array where prefix_sum of array is bigger than provided prefix_sum
    int binary_lifting(ll prefix_sum) {
        int num = 0;
        ll sum = 0;
        for (int i=30; i>=0; i--) {
            if ((num + (1 << i) < a.size()) && (sum + a[num + (1 << i)] <= prefix_sum)) {
                num += (1 << i);
                sum += a[num];
            }
        }
        return num;
    }

    void print() {
        printf("a: ");
        for (int i=0; i<a.size(); i++) 
            printf("%lld ", a[i]);
        printf("\n");
    }
};

int main() {
    vector<ll> arr = {1, 3, 4, 0, 0, 6, 1, 4, 2};
    BinaryIndexedTree tree;
    tree.init(arr);
    printf("%d\n", tree.binary_lifting(8));
    return 0;
    tree.update(6, 3);
    printf("1: %lld\n", tree.get(6));
}