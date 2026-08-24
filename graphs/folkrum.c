/*
    there are n childs, and f drinks and d dishes. Every child has a list of accpatable food and drinks. 
    One food or drink can be consumed at most by one child.
    Find the biggest number of childs that will get drink and food.
    Solve with max 
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#define i32 int
#define u32 unsigned int
#define i8 char
#define u8 unsigned char
#define u64 unsigned long long
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define NODE_CNT 403
#define START 0
#define END 1

bool visited[NODE_CNT];
u32 dfs(i8 flows[NODE_CNT][NODE_CNT], u32 from, u32 end, u32 flow, u32 N) {
    if (from == end) return flow;
    visited[from] = true;
    for (i32 i=0; i<N; i++) {
        if (visited[i] || !flows[from][i]) continue;
        u32 cap = min(flow, (u64)flows[from][i]);
        u32 pushed = dfs(flows, i, end, cap, N);
        if (!pushed) continue;
        flows[from][i] -= pushed;
        flows[i][from] += pushed;
        return pushed;
    }
    return 0;
}

u32 folkrum(i8 flows[NODE_CNT][NODE_CNT], u32 N) {
    assert(N <= NODE_CNT);
    u32 flow = 0;
    while (1) {
        memset(visited, 0, sizeof(visited));
        u32 _flow = dfs(flows, START, END, INT_MAX, N);
        if (!_flow) break;
        flow += _flow;
    }
    return flow;
}

i32 main() {
    int n, f, d;
    i8 flow[NODE_CNT][NODE_CNT];
    while (fscanf(stdin, "%d %d %d", &n, &f, &d) != EOF) {
        assert(n+n+f+d+2 < NODE_CNT);
        memset(flow, 0, sizeof(flow));

        const i32 F = 2, P = F+f, P1 = P+n, D = P1+n, N=D+d; // index starts with this + 1;
        for (i32 p=0, fi, di, nx; p<n; p++) {
            scanf("%d %d", &fi, &di);
            for (i32 i=0; i<fi; i++) {
                scanf("%d", &nx); nx--;
                flow[F+nx][P+p] = 1;
            }
            for (i32 i=0; i<di; i++) {
                scanf("%d", &nx); nx--;
                flow[P1+p][D+nx] = 1;
            }
        }
        for (i32 i=F; i<P; i++) 
            flow[START][i] = 1;
        for (i32 i=0; i<n; i++)
            flow[P+i][P1+i] = 1;
        for (i32 i=D; i<N; i++)
            flow[i][END] = 1;
        
        printf("%u\n", folkrum(flow, N));
    }
}