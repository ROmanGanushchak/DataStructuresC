#if defined(__GNUC__) && !defined(__clang__)
#include <bits/stdc++.h>
#else
#include "bits/stdc++.h"
#endif
using namespace std;

#define F first
#define S second
#define V vector
#define A array
#define M map
#define PQ priority_queue
#define all(a) a.begin(), a.end()
#define fastio() ios::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr)
using str = string;
using ll = long long;
using pii = pair<int, int>;
using Vi = V<int>;
using Vl = V<ll>;
using Vb = V<bool>;
using Vii = V<pii>;
#define fori(n) for (int i=0; i<n; i++)
void scan(pii& a) { scanf("%d %d", &a.F, &a.S); }
void scan(Vi& a) { for (int i=0; i<a.size(); i++) scanf("%d", &a[i]); }
void scan(Vl& a) { for (int i=0; i<a.size(); i++) scanf("%lld", &a[i]); }
void scan(Vii& a) { for (int i=0; i<a.size(); i++) scanf("%d %d", &a[i].F, &a[i].S); }

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;