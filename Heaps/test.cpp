#include <bits/stdc++.h>
#include "Heap.hpp"
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
#include "/Users/sarthakkumar/Work/Cpp/Templates/CP_Templates/debug.cpp"
#else
#define debug(...) 69
#endif

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
// mt19937_64 rng(69);

class RNG {
    typedef long long ll;

  public:
    RNG(const mt19937_64 &_rng) : m_rng(_rng) {}

  public:
    int nextInt() { return nextInt(INT32_MAX); }
    int nextInt(int x) { return (int)nextLong(ll(x)); }
    int nextInt(int a, int b) { return (int)nextLong(ll(a), ll(b)); }
    ll nextLong() { return m_rng(); }
    ll nextLong(ll x) { return std::uniform_int_distribution<ll>(0, x - 1)(m_rng); }
    ll nextLong(ll a, ll b) { return std::uniform_int_distribution<ll>(a, b - 1)(m_rng); }
    bool nextBoolean() { return nextInt(2); }
    vector<int> nextVector(int n, int mx = 1e9) {
        vector<int> a(n);
        for(int &x : a) x = nextInt(mx + 1);
        return a;
    }
    vector<ll> nextVector(int n, ll mx = 1e18) {
        vector<ll> a(n);
        for(ll &x : a) x = nextLong(mx + 1);
        return a;
    }

  private:
    mt19937_64 m_rng;
};
RNG Random(rng);

class Timer {
  public:
    Timer() {
        m_start = std::chrono::high_resolution_clock::now();
    }
    ~Timer() {
        m_end = std::chrono::high_resolution_clock::now();
        m_duration = (m_end - m_start);
        float ms = m_duration.count() * 1000.0f;
        std::cerr << "Done... Timer took: " << ms << " ms.\n";
    }

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start, m_end;
    std::chrono::duration<float> m_duration;
};
// test operations
void Test1() {
    PriorityQueue<int> h;
    multiset<int> s;

    const int ops = 1e4;
    for(int iter = 0; iter < ops; iter++) {
        if(!s.empty()) {
            assert(s.size() == h.size());
            assert(*s.begin() == h.top());
        }
        if(Random.nextInt(100) <= 90 || s.empty()) {
            // insert
            int x = Random.nextInt(1000);
            s.insert(x);
            h.insert(x);
        } else {
            // remove
            int mn = *s.begin();
            s.erase(s.begin());
            int mn_h = h.remove_min();
            assert(mn_h == mn);
        }
        if(!s.empty()) {
            assert(s.size() == h.size());
            assert(*s.begin() == h.top());
        }
    }
    cout << "Passed all tests !!" << '\n';
}

void Test2() {
    Timer tt;
    const int ops = 1e4;
    vpii operations;
    int sz = 0;
    for(int iter = 0; iter < ops; iter++) {
        if(Random.nextInt(100) <= 70 || sz == 0) {
            // insert
            int x = Random.nextInt(1000);
            operations.emplace_back(0, x);
            sz++;
        } else {
            // remove
            operations.emplace_back(1, 0);
            sz--;
        }
    }
    if(1) {
        // heap
        Timer t;
        cout << "Testing using heap.." << '\n';
        PriorityQueue<int> h;
        for(auto &op : operations) {
            if(op.first == 0)
                h.insert(op.second);
            else
                h.remove_min();
        }
        cout << "Done heap..." << '\n';
    }
    if(1) {
        Timer t;
        // sets
        cout << "Testing using sets.." << '\n';
        multiset<int> h;
        for(auto &op : operations) {
            if(op.first == 0)
                h.insert(op.second);
            else
                h.erase(h.begin());
        }
        cout << "Done sets..." << '\n';
    }
    if(1) {
        Timer t;
        // priority queue
        cout << "Testing using pq.." << '\n';
        priority_queue<int> h;
        for(auto &op : operations) {
            if(op.first == 0)
                h.push(op.second);
            else
                h.pop();
        }
        cout << "Done priority queue..." << '\n';
    }
}

void Test3() {
    int tests = 1e3;
    const int n = 1e4;
    for(int iter = 0; iter < tests; iter++) {
        vector<int> a = Random.nextVector(n, n);
        PriorityQueue<int>::sort(a);
        assert(is_sorted(a.begin(), a.end()));
    }
    cout << "Passed" << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout << setprecision(12) << fixed;

    Test1();
    Test2();
    Test3();
    cout << "Done.." << '\n';
    return 0;
}
