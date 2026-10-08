#include "types.h"
class UnionSet {
public:
    #define DP_SIZE 100001
    int dp[DP_SIZE];
    int size[DP_SIZE];
    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) {
            if (size[a] < size[b])
                swap(a, b);
            dp[b] = a;
            size[a] += size[b];
        }
    }

    void simple_unite(int x, int y) {
        dp[find(x)] = find(y);
    }

    int find(int x) {
        if (dp[x] == x) 
            return x;
        return dp[x] = find(dp[x]);
    }

    void init() {
        for (int i=0; i<DP_SIZE; i++) {
            dp[i] = i;
            size[i] = 1;
        }
    }
};