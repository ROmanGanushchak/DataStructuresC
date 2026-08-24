#include "../types.h"

int lis(Vi& a) {
    int n = a.size();
    const int INF = 1e9;
    Vi d(n+1, INF);
    d[0] = -INF;
 
    for (int i = 0; i < n; i++) {
        for (int l = 1; l <= n; l++) {
            if (d[l-1] < a[i] && a[i] < d[l])
                d[l] = a[i];
        }
    }
 
    int ans = 0;
    for (int l = 0; l <= n; l++) {
        if (d[l] < INF)
            ans = l;
    }
    return ans;
}