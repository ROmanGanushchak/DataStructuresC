#include <climits>
#include <cstdio>
#include <assert.h>
#include <algorithm>
#include <vector>
using namespace std;

/* given multiset of numbers finds the closest smaller number to k that 
    can be constructed as sum of subset of multiset 
    O(k*sqrt(n)) */
/*source: https://codeforces.com/blog/entry/146172 E2 */

#define MAX_K 200001
template<int maxn>
int knapsack(const vector<int>& a, int k) {
    if (maxn <= k+1) {
        return knapsack<min(MAX_K, maxn*2)>(a, k);
    }
    int n = a.size();
    int sum = 0;
    for (int i=0; i<a.size(); i++)
        sum += a[i];

    // uses sqrt decompression
    int limit = sqrt(sum);
    vector<int> cnts(limit, 0);
    vector<int> to_add;
    for (int i=0; i<a.size(); i++) {
        if (a[i] < limit) {
            cnts[a[i]]++;
        } else {
            to_add.push_back(a[i]);
        }
    }

    for (int i=1; i<limit; i++) {
        if (!cnts[i]) continue;
        int num = 2;
        while (num - 1 <= cnts[i]) {
            to_add.push_back((num >> 1) * i);
            num <<= 1;
        }
        int remained = cnts[i] - ((num >> 1) - 1);
        if (remained != 0)
            to_add.push_back(remained * i);
    }

    bitset<maxn> set;
    set[0] = 1;
    for (int v : to_add) {
        set |= set << v;
    }

    // finds closest less or equal
    int closest = INT_MAX;
    for (int i=k; i>0; i--) {
        if (set[i]) 
            return i;
    }
    return -1;
}

int main() {
    vector<int> a = {2, 4, 8, 16, 32, 64, 128, 256, 512};
    printf("%d\n", knapsack<1>(a, 1));
}