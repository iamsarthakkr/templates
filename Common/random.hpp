#include <bits/stdc++.h>
using namespace std;

namespace random_utils {
class Random {
    using Engine = std::mt19937_64;

  public:
    using result_type = Engine::result_type;

    Random() : m_rng(static_cast<result_type>(std::chrono::steady_clock::now().time_since_epoch().count())) {}

    explicit Random(result_type seed) : m_rng(seed) {}
    explicit Random(const Engine &eng) : m_rng(eng) {}

    template <typename T>
    T next(T min, T max) {
        static_assert(std::is_arithmetic_v<T>, "Random::next requires arithmetic type");

        if constexpr(std::is_same_v<T, bool>) {
            std::bernoulli_distribution dist(0.5);
            return dist(m_rng);
        } else if constexpr(std::is_integral_v<T>) {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(m_rng);
        } else {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(m_rng);
        }
    }

    // next in [0, max]
    template <typename T>
    T next(T max = std::numeric_limits<T>::max()) {
        if constexpr(std::is_same_v<T, bool>) {
            return next<bool>(false, true);
        } else {
            return next<T>(static_cast<T>(0), max);
        }
    }

    template <typename T>
    std::vector<T> next_vector(int n, T max = std::numeric_limits<T>::max()) {
        std::vector<T> ret(n);
        for(auto &v : ret) v = next<T>(max);
        return ret;
    }

    template <typename T>
    std::vector<T> next_vector(int n, T min, T max) {
        std::vector<T> ret(n);
        for(auto &v : ret) v = next<T>(min, max);
        return ret;
    }

  private:
    Engine m_rng;
};
} // namespace random_utils

static random_utils::Random Rng;
// static random_utils::Random Rng(42);
