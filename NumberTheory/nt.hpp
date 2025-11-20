#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace nt {
class Sieve {
  public:
    // Seive upto N
    static void init(int N = 50000000) {
        if(N <= s_maxN) return;
        s_maxN = N;

        s_isPrime.assign(N + 1, true);
        s_primeList.clear();

        if(N >= 0) s_isPrime[0] = false;
        if(N >= 1) s_isPrime[1] = false;

        for(int p = 2; p * p <= N; ++p) {
            if(!s_isPrime[p]) continue;
            for(int x = p * p; x <= N; x += p) s_isPrime[x] = false;
        }
        for(int p = 2; p <= N; ++p) {
            if(!s_isPrime[p]) continue;
            s_primeList.emplace_back(p);
        }
    }

    static void resize(int N) {
        init(N);
    }

    static const std::vector<int> &primes() { return s_primeList; }

    static bool is_prime(int x) {
        if(x <= 1) return false;
        resize(x);
        return s_isPrime[x];
    }

    // factorization of n using primes up to sqrt(n)
    // returns vector of {prime, exponent}
    static std::vector<std::pair<int64_t, int64_t>> factors(int64_t n) {
        std::vector<std::pair<int64_t, int64_t>> facts;
        if(n <= 1) return facts;

        int64_t limit = (int64_t)std::sqrt((long double)n) + 1;
        resize((int)limit);

        for(int p : primes()) {
            if(1LL * p * p > n) break;

            if(n % p == 0) {
                int64_t cnt = 0;
                while(n % p == 0) n /= p, cnt++;
                facts.emplace_back(p, cnt);
            }
        }
        if(n > 1) facts.emplace_back(n, 1);
        return facts;
    }

    // divisor of a number in sorted order
    static std::vector<int64_t> divisors(int64_t n) {
        std::vector<int64_t> divs;
        for(int64_t x = 1; x * x <= n; x++) {
            if(n % x == 0) {
                divs.push_back(x);
                if(x * x != n) divs.push_back(n / x);
            }
        }
        std::sort(divs.begin(), divs.end());
        return divs;
    }

  public:
    static inline std::vector<bool> s_isPrime;
    static inline std::vector<int> s_primeList;
    static inline int s_maxN = 0;
};

struct MillerRabin {
    static int64_t mul_mod(int64_t a, int64_t b, int64_t m) {
        return (long long)((__int128)a * b % m);
    }

    static int64_t pow_mod(int64_t base, int64_t exp, int64_t mod) {
        int64_t res = 1 % mod;
        base %= mod;
        while(exp > 0) {
            if(exp & 1) res = mul_mod(res, base, mod);
            base = mul_mod(base, base, mod);
            exp >>= 1;
        }
        return res;
    }

    static bool check_composite(int64_t n, int64_t a, int64_t d, int s) {
        int64_t x = pow_mod(a, d, n);
        if(x == 1 || x == n - 1) return false;
        for(int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if(x == n - 1) return false;
        }
        return true;
    }

    static bool is_prime(int64_t n) {
        if(n < 2) return false;

        // small primes trial
        for(int64_t p : {2, 3, 5, 7, 11, 13, 17, 19, 23}) {
            if(n == p) return true;
            if(n % p == 0) return false;
        }

        int64_t d = n - 1;
        int s = 0;
        while((d & 1) == 0) {
            d >>= 1;
            ++s;
        }

        // deterministic bases for 64-bit integers
        for(int64_t a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
            if(a % n == 0) continue;
            if(check_composite(n, a, d, s)) return false;
        }
        return true;
    }
};
} // namespace nt
using namespace nt;
