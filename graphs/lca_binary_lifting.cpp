#include "../types.h"
constexpr int L = 18;
array<Vi, L> parent; // size is Lx(N+1)
Vi depth;

void construct(Vi& next_first_layer) { // no parent equals n
    int n = next_first_layer.size();
    parent[0] = next_first_layer;
    parent[0].push_back(n);
    for(int i=1; i<L; i++) {
        parent[i].assign(n+1, 0);
        for(int j=0; j<=n; j++) {
            parent[i][j] = parent[i-1][parent[i-1][j]];
        }
    }

    depth.assign(n + 1, -1);
    depth[n] = 0;
    for(int i = 0; i < n; i++) {
        int j = i, c = 0;
        while(depth[j] < 0) j = parent[0][j], c++;
        for(int k = i, d = depth[j] + c; depth[k] < 0; k = parent[0][k])
            depth[k] = d--;
    }
}

int lca(int i, int j) {
    if(depth[i] > depth[j]) swap(i, j);
    int diff = depth[j] - depth[i];
    for(int k=L-1; k>=0; k--) {
        if((diff >> k) & 1) {
            j = parent[k][j];
        }
    }
    assert(depth[i] == depth[j]);
    if(i == j) return j;
    for(int k=L-1; k>=0; k--) {
        int a = parent[k][i], b = parent[k][j];
        if(a != b) {
            i = a, j = b;
        }
    }
    assert(parent[0][i] == parent[0][j]);
    return parent[0][i];
}
