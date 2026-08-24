#include <cassert>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <array>
using namespace std;

// not tested yet
#define ll long long
constexpr int L = 20; /*! change depending on the size, should equal to ceil(log2(SIZE)), 
    where SIZE is max depth + 1*/
class LCA {
private:
    int l, timer;
    vector<int> tin, tout;
    void dfs(vector<vector<int>>& adj, int v, int p) {
        tin[v] = timer++;
        up[v][0] = p;
        for (int i=1; i<=l; i++)
            up[v][i] = up[up[v][i-1]][i-1];
        
        for (auto neigh : adj[v])
            if (neigh != p)
                dfs(adj, neigh, v);
        tout[v] = timer++;
    }
public:
    vector<array<int, L+1>> up;
    bool is_parent(int v, int u) {
        return tin[v] <= tin[u] && tout[u] <= tout[v];
    }
    void init(vector<vector<int>>& adj, int root) {
        int n = adj.size();
        l = ceil(log2(n));
        assert(l <= L);
        tin.resize(n);
        tout.resize(n);
        up.resize(n);
        timer = 0;
        dfs(adj, root, root);
    }
    int lca(int v, int u) {
        if (is_parent(v, u)) return v;
        if (is_parent(u, v)) return u;
        for (int i=l; i>=0; i--) {
            int n = up[v][i];
            if (!is_parent(n, u))
                v = n;
        }
        return up[v][0];
    }
};

int main() {
    int n; scanf("%d", &n);
    vector<vector<int>> adj(n);
    for (int i=1; i<n; i++) {
        int a, b; scanf("%d %d", &a, &b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    LCA lca;
    lca.init(adj, 0);
    int q; scanf("%d", &q);
    while (q--) {
        int a, b; scanf("%d %d", &a, &b);
        printf("%d\n", lca.lca(a, b));
    }
}