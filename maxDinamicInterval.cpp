#include <assert.h>
#include <algorithm>
#include <vector>
using namespace std;
/* Allows to find the biggest sum of any interval in the array, whose values might be chagned */

struct Node {
    int sum, pref, suf, best;
};

class SubTree {
private:
    vector<Node> ns;
    int _n;
    Node merge(Node a, Node b) {
        return (Node) {
            .sum=a.sum + b.sum,
            .pref = max(a.pref, a.sum + b.pref),
            .suf  = max(b.suf, b.sum + a.suf),
            .best = max(max(a.best, b.best), a.suf + b.pref)  
        };
    }
    inline Node newNode(int val) {
        int a = max(0, val);
        return (Node) { val, a, a, a };
    }
public:
    void init(int N) {
        int n = 1 << (32 - __builtin_clz(N - 1));
        _n = n;
        ns.resize((n << 1) - 1);
        for (int i=n-1; i<(n << 1) - 1; i++)
            ns[i] = newNode(-1);
        for (int i=n-2; i>=0; i--)
            ns[i] = merge(ns[(i<<1)+1], ns[(i<<1)+2]);
    }
    void update(int i, int val) {
        int n = i+_n-1;
        ns[n] = newNode(val);
        while(n > 0) {
            n = (n - 1) >> 1;
            ns[n] = merge(ns[(n<<1)+1], ns[(n<<1)+2]);
        }
    }
    int get_max() {
        return ns[0].best;
    }
};