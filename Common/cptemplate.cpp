#include <bits/stdc++.h>

/*
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>template
*/

using namespace std;
// using namespace __gnu_pbds;

// typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;

typedef long long ll;
typedef unsigned long long ull;
typedef long double lld;
typedef pair<int, int> pii;
typedef pair<long long, long long> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<vi> vvi;
typedef vector<vll> vvll;
typedef vector<pii> vpii;
typedef vector<pll> vpll;

#define pb push_back
#define fi first
#define se second
#define forn(i, n) for(int i = 0; i < n; i++)
#define forv(x, a) for(auto &x : a)
#define ford(i, n) for(int i = n - 1; i >= 0; i--)
#define for1(i, n) for(int i = 1; i <= n; i++)
#define forr(i, l, r) for(int i = l; i <= r; i++)
#define forrd(i, l, r) for(int i = r; i >= l; i--)
#define all(a) (a).begin(), (a).end()
#define rall(a) (a).rbegin(), (a).rend()
#define len(a) (int)(a).size()

#ifdef SARTHAK_LOCAL
#include "/Users/sarthakkumar/work/Cpp/Templates/Common/debug.hpp"
#else
#define debug(...) ((void)0)
#endif

template <typename T> inline T sq(const T &x) { return x * x; }
template <typename T> inline bool cmin(T &a, const T &b) { return b < a ? a = b, true : false; }
template <typename T> inline bool cmax(T &a, const T &b) { return b > a ? a = b, true : false; }

void Main(int tc) {
}

#define MULTI // comment for single test

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << setprecision(12) << fixed;

    int tt = 1;
#ifdef MULTI
    cin >> tt;
#endif

    for(int t = 1; t <= tt; t++) {
#ifdef SARTHAK_LOCAL
        cerr << "Test Case: #" << t << '\n';
        cerr << "---------------" << '\n';
#endif

        Main(t);

#ifdef SARTHAK_LOCAL
        cerr << "---------------" << '\n';
#endif
    }

#ifdef SARTHAK_LOCAL
    cerr << "Total Time: " << (double)clock() * 1e3 / CLOCKS_PER_SEC << "ms" << '\n';
#endif
    return 0;
}
