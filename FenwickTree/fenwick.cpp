#pragma once
#include <vector>

template <typename T>
class fenwick {
  public:
    fenwick() : m_size(0) {}

    explicit fenwick(int n, T v = T{}) : m_size(n), m_fenwick(n, T{}) {
        if(v != T{}) {
            for(int i = 0; i < m_size; ++i) add(i, v);
        }
    }

    template <typename U>
    explicit fenwick(const std::vector<U> &a) : m_size(static_cast<int>(a.size())), m_fenwick(m_size, T{}) {
        for(int i = 0; i < m_size; ++i) {
            m_fenwick[i] += static_cast<T>(a[i]);
            int j = i | (i + 1);
            if(j < m_size) m_fenwick[j] += m_fenwick[i];
        }
    }

    // add v at index i (0-based)
    void update(int i, T v) {
        for(int j = i; j < m_size; j = j | (j + 1))
            m_fenwick[j] += v;
    }

    // prefix sum [0..r]
    T pref(int r) const {
        if(r < 0) return T{};
        T res{};
        for(int i = r; i >= 0; i = ((i & (i + 1)) - 1))
            res += m_fenwick[i];
        return res;
    }

    // range sum [l..r]
    T get(int l, int r) const {
        if(l > r) return T{};
        return pref(r) - pref(l - 1);
    }

    int size() const { return m_size; }

  private:
    int m_size;
    std::vector<T> m_fenwick;
};
