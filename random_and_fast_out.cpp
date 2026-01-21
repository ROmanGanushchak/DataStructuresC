#include <bits/stdc++.h>
#include <cstdio>
#include <assert.h>
#include <iostream>
#include <sstream>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int main() {
    int min = 0, max = 10;
    printf("%d\n", rng() % (max - min + 1) + min);
}

void _main() {
    int n; scanf("%d", &n);
    std::ostringstream buffer;
    buffer << n-1 << "\n";
    for (int i=1; i<n; i++)
        buffer << i << " 1\n";
    cout << buffer.str();
}
