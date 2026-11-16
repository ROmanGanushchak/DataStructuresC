#include "types.h"
// Goal: compactly store the set of all subset-XORs of an array
// That set is closed under XOR, so a number already in it adds nothing
// Only numbers that were new matter; each one doubles the set (k of them -> 2^k values)
// Replacing a number by its XOR with another one doesn't change the set, so rearrange them
// so each basis number owns a unique top bit
// Then testing / inserting x is forced: go through bits top to bottom,
//      if x has the bit, XOR with the owner of that bit (insert x there if nobody owns it)
// x is in the set iff it reduces to 0
// All 2^k values are made equally often: 2^(n-k) subsets each
// (so subsets with XOR 0 = 2^(n-k), incl. empty)

ll b[60];  // values must be < 2^60

bool add(ll x) {  // true if x was new (basis grew)
    for (int i = 59; i >= 0; i--) if (x >> i & 1) { if (!b[i]) return b[i] = x, 1; x ^= b[i]; }
    return 0;
}

bool in(ll x) {   // true if x is an XOR of some subset of the added numbers
    for (int i = 59; i >= 0; i--) if (x >> i & 1) x ^= b[i];
    return !x;
}

vector<ll> get() {  // the basis elements (non-empty slots), highest bit first if you reverse
    vector<ll> v;
    for (ll x : b) if (x) v.push_back(x);
    return v;
}