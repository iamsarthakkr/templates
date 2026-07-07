#include <type_traits>
#include <cassert>
#include <array>
#include <vector>

namespace hashing {

template <typename T_iterable, typename Mod>
class poly_hash {
  public:
    poly_hash() : m_hash(1), m_power(1), m_size(0), m_base(0) {}
    explicit poly_hash(const T_iterable &s, Mod base = Mod(31)) { build(s, base); }
    // hash [l,r) half open
    Mod operator()(int l, int r) const {
        assert(l >= 0 && l <= r && r <= (int)m_size);
        Mod r_hash = m_hash[r], l_hash = m_hash[l];
        return r_hash - l_hash * m_power[r - l];
    }
    Mod operator()() const { return m_hash.back(); }
    // hash of length len
    Mod operator()(int len) const {
        assert(0 <= len && len <= (int)m_size);
        return m_hash[len];
    }
    Mod concat(Mod hA, Mod hB, int lenB) const { return hA * m_power[lenB] + hB; }
    const std::vector<Mod> &powers() const { return m_power; }

    size_t size() const { return m_size; }
    Mod base() const { return m_base; }

  private:
    void build(const T_iterable &s, Mod base) {
        m_size = s.size();
        m_base = base;
        m_hash.assign(m_size + 1, Mod(0));
        m_power.assign(m_size + 1, Mod(1));

        for(size_t i = 1; i <= m_size; i++) m_power[i] = m_base * m_power[i - 1];

        for(size_t i = 0; i < m_size; i++) {
            Mod val = normalize(s[i]);
            m_hash[i + 1] = m_hash[i] * m_base + val;
        }
    }

  private:
    template <typename T>
    static Mod normalize(const T &x) {
        using U = std::decay_t<T>;
        if constexpr(std::is_same_v<U, Mod>) {
            return x;
        } else if constexpr(std::is_integral_v<U> && sizeof(U) == 1) {
            return Mod(static_cast<unsigned int>(static_cast<unsigned char>(x))) + Mod(1);
        } else {
            return Mod(x) + Mod(1);
        }
    }

  private:
    std::vector<Mod> m_hash;
    std::vector<Mod> m_power;
    size_t m_size;
    Mod m_base;
};

template <typename T_Iterable, typename Mod, size_t N = 2>
class multi_hash {
    static_assert(N <= 10, "Can't have more then 10 hashes");
    template <typename T> using poly = std::array<T, N>;

  public:
    multi_hash() = delete;
    explicit multi_hash(const T_Iterable &s) {
        build(s);
    }

    // hash [l,r) half open
    poly<Mod> operator()(int l, int r) const {
        assert(l >= 0 && l <= r && r <= (int)m_size);
        std::array<Mod, N> ret;
        for(size_t i = 0; i < N; i++) {
            ret[i] = m_polyHash[i](l, r);
        }
        return ret;
    }
    poly<Mod> operator()() const {
        std::array<Mod, N> ret;
        for(size_t i = 0; i < N; i++) {
            ret[i] = m_polyHash[i]();
        }
        return ret;
    }
    // hash of length len
    poly<Mod> operator()(int len) const {
        assert(0 <= len && len <= (int)m_size);
        std::array<Mod, N> ret;
        for(size_t i = 0; i < N; i++) {
            ret[i] = m_polyHash[i](len);
        }
        return ret;
    }

    poly<Mod> concat(const poly<Mod> &hA, const poly<Mod> &hB, int lenB) const {
        poly<Mod> ret;
        for(size_t i = 0; i < N; i++) {
            ret[i] = hA[i] * m_polyHash[i].powers()[lenB] + hB[i];
        }
        return ret;
    }
    size_t size() const { return m_size; }

  public:
    static int get_base(int pos) { return s_bases[pos]; }

  private:
    void build(const T_Iterable &s) {
        m_size = s.size();
        for(size_t i = 0; i < N; i++) {
            m_polyHash[i] = poly_hash<T_Iterable, Mod>(s, Mod(s_bases[i]));
        }
    }

  private:
    poly<poly_hash<T_Iterable, Mod>> m_polyHash;
    size_t m_size;

  private:
    static constexpr int s_bases[10] = {31, 37, 53, 59, 61, 73, 97, 101, 131, 137};
};
} // namespace hashing
