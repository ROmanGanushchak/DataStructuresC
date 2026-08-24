#include "../types.h"
int lcs(string &s1, string &s2) {
    int m = s1.length(), n = s2.length();
    Vi dp(n + 1, 0);

    for (int i = 1; i <= m; ++i) {
        int prev = dp[0];

        for (int j = 1; j <= n; j++) {
            int temp = dp[j];
            if (s1[i - 1] == s2[j - 1])
                dp[j] = 1 + prev;
            else
                dp[j] = max(dp[j - 1], dp[j]);
            prev = temp;
        }
    }
    return dp[n];
}