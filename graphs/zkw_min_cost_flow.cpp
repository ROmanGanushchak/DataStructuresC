#include "../types.h"

// O((F + V·C) · E)
// does not support negative edges
template <int MAXN = 1000, int MAXM = 100000>
struct zkw_flow {
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

  int n, s, t, visit[MAXN];
  ll tf, tc, dis[MAXN], slack[MAXN];

  int modlable() {
    ll delta = INF;
    for (int i = 0; i < n; i++) {
      if (!visit[i] && slack[i] < delta) delta = slack[i];
      slack[i] = INF;
    }
    if (delta == INF) return 1;
    for (int i = 0; i < n; i++)
      if (visit[i]) dis[i] += delta;
    return 0;
  }

  ll dfs(cost_flow_edge_list &e, int x, ll flow) {
    if (x == t) {
      assert(flow < INF);  // unbounded flow: pass a finite maxf
      tf += flow;
      tc += flow * (dis[s] - dis[t]);
      return flow;
    }
    visit[x] = 1;
    ll left = flow;
    for (int i = e.begin[x]; ~i; i = e.next[i])
      if (e.flow[i] > 0 && !visit[e.dest[i]]) {
        int y = e.dest[i];
        if (dis[y] + e.cost[i] == dis[x]) {
          ll delta = dfs(e, y, min(left, e.flow[i]));
          e.flow[i] -= delta;
          e.flow[i ^ 1] += delta;
          left -= delta;
          if (!left) {
            visit[x] = false;
            return flow;
          }
        } else
          slack[y] =
            min(slack[y], dis[y] + e.cost[i] - dis[x]);
      }
    return flow - left;
  }

  // returns {flow, cost}; maxf limits total flow (needed if an s-t path
  // consists only of INF-capacity edges)
  pair<ll, ll> solve(
    cost_flow_edge_list &e, int n_, int s_, int t_, ll maxf = INF) {
    n = n_;
    s = s_;
    t = t_;
    tf = tc = 0;
    if (s == t) return make_pair(0, 0);
    fill(dis, dis + n, 0);
    fill(slack, slack + n, INF);
    do {
      do {
        fill(visit, visit + n, 0);
      } while (tf < maxf && dfs(e, s, maxf - tf));
    } while (tf < maxf && !modlable());
    return make_pair(tf, tc);
  }
};
