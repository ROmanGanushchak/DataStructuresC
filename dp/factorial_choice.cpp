#define ll long long
#define MOD (ll)(1000000007)
#define MAXN (int)(2*1e5+1)

ll modPow(ll a, ll b) {
    long long res = 1;
    while (b > 0) {
        if (b & 1) res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}

int fact[MAXN + 1], invFact[MAXN + 1];
void initFactorials() {
    fact[0] = 1;
    for (int i = 1; i <= MAXN; i++) fact[i] = (ll)fact[i - 1] * i % MOD;

    invFact[MAXN] = modPow(fact[MAXN], MOD - 2);
    for (int i = MAXN; i > 0; i--) invFact[i - 1] = (ll)invFact[i] * i % MOD;
}

ll choice(int n, int r) {
    if (r < 0 || r > n) return 0;
    return (ll)fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

int manual_choice_mod(int n, int k) {
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int res = n % MOD;
    int k_fac = 1;
    for(int i = 2; i <= k; i++) {
        res = (res * (n-i+1)) % MOD;
        k_fac = (k_fac * i) % MOD;
    }
    return res * modPow(k_fac, MOD-2) % MOD;
}

ll manual_choice(int n, int k) {
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int res = n;
    for(int i = 2; i <= k; i++) {
        res *= (n-i+1);
        res /= i;
    }
    return res;
}