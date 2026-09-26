#define ll long long
#define MOD (ll)(1000000007)
#define MAXN (int)(2*1e5+1)

constexpr ll modPow(ll a, ll b) {
    long long res = 1;
    while (b > 0) {
        if (b & 1) res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}

struct CompileTimeFact {
    int fact[MAXN+1];
    int invFact[MAXN+1];

    constexpr CompileTimeFact() : fact{}, invFact{} {
        fact[0] = 1;
        for (int i = 1; i <= MAXN; i++) fact[i] = (ll)fact[i - 1] * i % MOD;

        invFact[MAXN] = modPow(fact[MAXN], MOD - 2);
        for (int i = MAXN; i > 0; i--) invFact[i - 1] = (ll)invFact[i] * i % MOD;
    }
};
CompileTimeFact C;

ll choice(int n, int r) {
    if (r < 0 || r > n) return 0;
    return C.fact[n] * C.invFact[r] % MOD * C.invFact[n - r] % MOD;
}