#include <cassert>
#include <vector>
#include <math.h>
#include <queue>
#include <iostream>
using namespace std;

// not tested
#define ll long long
struct Edge {
    int v, u;
    ll cap, flow;
    Edge(int v, int u, ll cap) : v(v), u(u), cap(cap), flow(0) {}
};

class Dinics {
    const ll flow_inf = 1e18;
    int s, t, m;
    vector<Edge> es;
    vector<vector<int>> adj;
    vector<int> level, ptr;
    queue<int> q;
public:
    Dinics(int n, int s, int t) : s(s), t(t), m(0) {
        level.resize(n);
        ptr.resize(n);
        adj.resize(n);
    }
    void add_edge(int v, int u, ll cap) {
        es.emplace_back(v, u, cap);
        es.emplace_back(u, v, 0);
        adj[v].push_back(m);
        adj[u].push_back(m+1);
        m += 2;
    }
    bool bfs() {
        while (q.size()) {
            int v = q.front(); q.pop();
            for (int idx : adj[v]) {
                auto& e = es[idx];
                if (e.cap - e.flow < 1 || level[e.u] != -1)
                    continue;
                level[e.u] = level[v] + 1;
                q.push(e.u);
            }
        }
        return level[t] != -1;
    }
    ll dfs(int v, ll pushed) {
        if (v == t || pushed == 0)
            return pushed;
        for (int& i=ptr[v]; i<adj[v].size(); i++) {
            int idx = adj[v][i];
            auto& e = es[idx];
            if (level[v] + 1 != level[e.u] || e.cap - e.flow < 1) continue;
            ll tr = dfs(e.u, min(pushed, e.cap - e.flow));
            if (tr == 0) continue;
            e.flow += tr;
            es[idx ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }
    ll flow() {
        ll f = 0;
        while (true) {
            fill(level.begin(), level.end(), -1);
            level[s] = 0;
            q.push(s);
            if (!bfs()) break;
            fill(ptr.begin(), ptr.end(), 0);
            while (ll p = dfs(s, flow_inf)) 
                f += p;
        }
        return f;
    }
};

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;

    Dinics dinic(n, s, t);
    for (int i = 0; i < m; i++) {
        int v, u; ll cap;
        cin >> v >> u >> cap;
        dinic.add_edge(v, u, cap);
    }

    cout << dinic.flow() << "\n";
    return 0;
}