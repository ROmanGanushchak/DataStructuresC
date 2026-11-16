#include "types.h"

struct TwoSat {
	int N, t = 0, c = 0;
    V<Vi> g;
    Vi values, val, com, z;

	TwoSat(int n = 0) : N(n), g(2*n) {}

	int addVar() { g.resize(2*N+2); return N++; }

	void either(int a, int b) {                                      // a | b
		a = max(2*a, -1-2*a);
        b = max(2*b, -1-2*b);
		g[a].push_back(b^1), g[b].push_back(a^1);
	}
	void setValue(int x) { either(x, x); }                      // a
	void implies(int a, int b) { either(~a, b); }                 // a -> b
	void equal(int a, int b) { implies(a, b), implies(b, a); }  // a == b
	void xorr(int a, int b) { equal(a, ~b); }                     // a != b
	void atMostOne(const Vi& l) {
		if (l.size() < 2) return;
		int cur = ~l[0];
		for (int i = 2; i < l.size(); i++) {
			int nx = addVar();
			either(cur, ~l[i]);
            either(cur, nx);
            either(~l[i], nx);
			cur = ~nx;
		}
		either(cur, ~l[1]);
	}

	int dfs(int i) {
		int low = val[i] = ++t, x; z.push_back(i);
		for (int e : g[i]) if (!com[e]) low = min(low, val[e] ?: dfs(e));
		if (low == val[i] && ++c) do {
            x = z.back();
            z.pop_back(); 
            com[x] = c; 
        } while (x != i);
		return val[i] = low;
	}

	bool solve() {
		values.assign(N, 0);
        val.assign(2*N, 0);
        com = val;
		forn(i, 2*N) if (!com[i]) dfs(i);
		forn(i, N) {
            if (com[2*i] == com[2*i+1]) return 0;
            values[i] = com[2*i] > com[2*i+1];
        }
        // values stores result
		return 1;
	}
};
