#include "types.h"

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int get_random() {
    int min = 0, max = 10;
    return rng() % (max - min + 1) + min;
}

void remove_duplicates(vector<int> e) {
    sort(all(e)); 
    e.erase(unique(all(e)), e.end());
}

void fast_in() {
    int n; scanf("%d", &n);
    std::ostringstream buffer;
    buffer << n-1 << "\n";
    for (int i=1; i<n; i++)
        buffer << i << " 1\n";
    cout << buffer.str();
}
