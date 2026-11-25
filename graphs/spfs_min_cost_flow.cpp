#include "../types.h"

// O(F · V · E)
// does support negative edges (not cycles)
template <int MAXN = 1000, int MAXM = 100000>
struct minimum_cost_flow {
    static constexpr ll INF = LLONG_MAX / 4;

    struct cost_flow_edge_list {
        int size, begin[MAXN], dest[MAXM], next[MAXM];
        ll cost[MAXM], flow[MAXM];

        void clear(int n) {
            size = 0;
            fill(begin, begin + n, -1);
        }

        cost_flow_edge_list(int n = MAXN) {
            assert(n < MAXN);
            clear(n);
        }

        // c - cost, f - capacity
        void add_edge(int u, int v, ll c, ll f) {
            dest[size] = v;
            next[size] = begin[u];
            cost[size] = c;
            flow[size] = f;
            begin[u] = size++;
            dest[size] = u;
            next[size] = begin[v];
            cost[size] = -c;
            flow[size] = 0;
            begin[v] = size++;
        }
    };

    int n, s, t, prev[MAXN], q[MAXN];
    ll dist[MAXN];
    bool occur[MAXN];

    bool augment(cost_flow_edge_list &e) {
        fill(dist, dist + n, INF);
        fill(occur, occur + n, false);
        dist[s] = 0;
        occur[s] = true;
        // ring buffer: occur[] keeps each vertex in the queue at most once
        int head = 0, tail = 0, cnt = 1;
        q[tail++] = s;
        while (cnt) {
            int x = q[head];
            head = head + 1 == n ? 0 : head + 1;
            --cnt;
            occur[x] = false;
            for (int i = e.begin[x]; ~i; i = e.next[i]) {
                int y = e.dest[i];
                if (e.flow[i] && dist[y] > dist[x] + e.cost[i]) {
                    dist[y] = dist[x] + e.cost[i];
                    prev[y] = i;
                    if (!occur[y]) {
                        occur[y] = true;
                        q[tail] = y;
                        tail = tail + 1 == n ? 0 : tail + 1;
                        ++cnt;
                    }
                }
            }
        }
        return dist[t] < INF;
    }

    // returns {flow, cost}; maxf limits total flow (needed if an s-t path
    // consists only of INF-capacity edges)
    pair<ll, ll> solve(
        cost_flow_edge_list &e, int n_, int s_, int t_, ll maxf = INF) {
        n = n_;
        s = s_;
        t = t_;
        pair<ll, ll> ans = make_pair(0, 0);
        if (s == t) return ans;
        while (ans.first < maxf && augment(e)) {
            ll num = maxf - ans.first;
            for (int i = t; i != s; i = e.dest[prev[i] ^ 1])
                num = min(num, e.flow[prev[i]]);
            assert(num < INF);  // unbounded flow: pass a finite maxf
            ans.first += num;
            for (int i = t; i != s; i = e.dest[prev[i] ^ 1]) {
                e.flow[prev[i]] -= num;
                e.flow[prev[i] ^ 1] += num;
                ans.second += num * e.cost[prev[i]];
            }
        }
        return ans;
    }
};
