class UnionSet {
public:
    #define DP_SIZE 100001
    int dp[DP_SIZE];
    void unite(int x, int y) {
        dp[find(x)] = find(y);
    }

    int find(int x) {
        if (dp[x] == x) 
            return x;
        return dp[x] = find(dp[x]);
    }

    void init() {
        for (int i=0; i<DP_SIZE; i++)
            dp[i] = i;
    }
};