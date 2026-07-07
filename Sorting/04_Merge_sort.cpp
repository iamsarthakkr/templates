#include <bits/stdc++.h>
using namespace std;

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
#define ford(i, n) for(int i = n - 1; i >= 0; i--)
#define for1(i, n) for(int i = 1; i <= n; i++)
#define forr(i, l, r) for(int i = l; i <= r; i++)
#define forrd(i, l, r) for(int i = r; i >= l; i--)
#define all(a) (a).begin(), (a).end()
#define rall(a) (a).rbegin(), (a).rend()
#define len(a) (int)(a).size()
#define sq(x) (x) * (x)

#ifdef SARTHAK_LOCAL
#include "/Users/sarthakkumar/cpp/templates/CompCoding/debug.cpp"
#else 
#define debug(...) 69
#endif

class Timer {
public:
    Timer() {
        m_start = std::chrono::high_resolution_clock::now();
    }
    void start() {
        m_start = std::chrono::high_resolution_clock::now();
    }
    void time() {
        m_end = std::chrono::high_resolution_clock::now();
        m_duration = (m_end - m_start);
        float ms = m_duration.count() * 1000.0f;
        std::cerr << "[Time] " << ms << " ms.\n"; 
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start, m_end;
    std::chrono::duration<float> m_duration;
};

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
// mt19937_64 rng(69);

class RNG {
typedef long long ll;
public:
    RNG(const mt19937_64& _rng): m_rng(_rng) {}

public:
    int nextInt() { return nextInt(INT32_MAX); }
    int nextInt(int x) { return (int)nextLong(ll(x)); }
    int nextInt(int a, int b) { return (int)nextLong(ll(a), ll(b)); }
    ll nextLong() { return m_rng(); }
    ll nextLong(ll x) { return std::uniform_int_distribution<ll>(0, x - 1)(m_rng); }
    ll nextLong(ll a, ll b) { return std::uniform_int_distribution<ll>(a, b - 1)(m_rng); }
    bool nextBoolean() { return nextInt(2); }
    vector<int> nextVector(int n, int mx = 1e9) { vector<int> a(n); for (int& x : a) x = nextInt(mx + 1); return a; }
    vector<ll> nextVector(int n, ll mx = 1e18) { vector<ll> a(n); for (ll& x : a) x = nextLong(mx + 1); return a; }
private:
    mt19937_64 m_rng;
};
RNG Random(rng);

void sort(vector<int>& a) {
    int n = (int) a.size();

    vector<int> aux(n);
    auto solve = [&](auto&& self, int l, int r) -> void{
        if(r - l == 1) return;
        int m = (r + l) >> 1;
        self(self, l, m);
        self(self, m, r);

        int i = l, j = m, k = 0;
        while(i < m || j < r) {
            if(j == r || (i < m && a[i] < a[j])) {
                aux[k++] = a[i++];
            } else {
                aux[k++] = a[j++];
            }
        }
        for(i = 0; i < r - l; i++) {
            a[l + i] = aux[i];
        }
    };
    solve(solve, 0, n);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout << setprecision(12) << fixed;

    Timer t;
    for(int iter = 0; iter < 100; iter++) {
        vi a = Random.nextVector(100000, (int)1e8);
        t.start();
        sort(a);
        t.time();
        assert(is_sorted(a.begin(), a.end()));
    }
    return 0;
}