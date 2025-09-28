#define ll long long
#define MOD (ll)(998244353)
#define MAXN (int)(2*1e5+1)
ll fact[MAXN + 1];
ll invFact[MAXN + 1];

ll modPow(ll a, ll b) {
    long long res = 1;
    while (b > 0) {
        if (b & 1) res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}

void precompute() {
    fact[0] = 1;
    for (int i = 1; i <= MAXN; i++)
        fact[i] = (fact[i - 1] * i) % MOD;

    invFact[MAXN] = modPow(fact[MAXN], MOD - 2); // Fermat’s little theorem
    for (int i = MAXN; i > 0; i--)
        invFact[i - 1] = (invFact[i] * i) % MOD;
}

ll choice(int n, int r) {
    if (r < 0 || r > n) return 0;
    return ((fact[n] * invFact[r]) % MOD * invFact[n - r]) % MOD;
}