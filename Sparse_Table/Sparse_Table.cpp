#include <bits/stdc++.h>

// For range min
template <typename T, typename F>
class sparse_table {
  public:
    sparse_table(const std::vector<T> &arr, const F &func) : m_size((int)arr.size()), m_log(1), m_func(func) {
        while((1 << m_log) < m_size) m_log++;
        m_data.assign(m_log + 1, std::vector<T>(m_size));
        m_log2.assign(m_size + 1, 0);

        init(arr);
    }

    T get(int l, int r) {
        r++;
        int sz = m_log2[r - l];
        return m_func(m_data[sz][l], m_data[sz][r - (1 << sz)]);
    }

  private:
    void init(const std::vector<T> &arr) {
        int n = arr.size();
        for(int i = 0; i < n; i++) {
            m_data[0][i] = arr[i];
        }
        for(int k = 1; k <= m_log; k++) {
            for(int i = 0; i < n; i++) {
                int m = i + (1 << (k - 1));
                m_data[k][i] = m_data[k - 1][i];
                if(m < n) {
                    m_data[k][i] = m_func(m_data[k][i], m_data[k - 1][m]);
                }
            }
        }

        int k = 0;
        for(int i = 0; i <= n; i++) {
            while((1 << (k + 1)) < i) k++;
            m_log2[i] = k;
        }
    }

  private:
    const F m_func;
    std::vector<std::vector<T>> m_data;
    std::vector<int> m_log2;
    int m_size;
    int m_log;
};
