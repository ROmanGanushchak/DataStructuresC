#include <bits/stdc++.h>
using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int main() {
    int min = 0, max = 10;
    printf("%d\n", rng() % (max - min + 1) + min);
}