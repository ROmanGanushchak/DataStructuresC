#include <stdio.h>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
using namespace std;

void bellman_ford(vector<tuple<int, int, int>>& edges, int n, int start) {
    // Complexity: O(n*m)
    vector<int> dist(n, INT_MAX);
    dist[start] = 0;
    for (int i=0; i<n-1; i++) {
        for (auto &e : edges) {
            int a = get<0>(e), b = get<1>(e), w = get<2>(e);
            dist[b] = min(dist[b], dist[a] + w);
        }
    }

    // Afterwards algorithm can check weather there is any negative cycle
    bool is_negative_cycle = false;
    for (auto &e : edges) {
        int a = get<0>(e), b = get<1>(e), w = get<2>(e);
        if (dist[a] + w < dist[b]) {
            is_negative_cycle = true;
            break;
        }
    }
}

void dijkstra() {
    int n; scanf("%d", &n);

    vector<vector<pair<int, int>>> neigh(n);
    // init neigh
    vector<int> dist(n, INT_MAX);
    vector<bool> processed(n, false);
    priority_queue<pair<int, int>> q;

    int start = 0;
    q.push({0, start});
    while (q.size()) {
        int e = q.top().second; q.pop();
        if (processed[e]) continue;
        processed[e] = true;
        for (auto n : neigh[e]) {
            if (dist[e] + n.second < dist[n.first]) {
                dist[n.first] = dist[e] + n.second;
                q.push({-dist[n.first], n.first});
            }
        }
    }
}