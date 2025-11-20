#include <vector>
#include <numeric>

class dsu {
  public:
    explicit dsu(int n) : m_parent(n), m_size(n, 1) {
        std::iota(m_parent.begin(), m_parent.end(), 0);
    }

    int size(int x) {
        int root = find(x);
        return m_size[root];
    }

    int find(int x) {
        int y = x;
        while(m_parent[y] != y) y = m_parent[y];
        while(m_parent[x] != x) {
            int z = m_parent[x];
            m_parent[x] = y;
            x = z;
        }
        return y;
    }

    bool unite(int x, int y) {
        x = find(x), y = find(y);
        if(x == y) return false;
        if(m_size[x] > m_size[y]) std::swap(x, y);
        m_parent[x] = y;
        m_size[y] += m_size[x];
        return true;
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }

  private:
    std::vector<int> m_parent;
    std::vector<int> m_size;
};
