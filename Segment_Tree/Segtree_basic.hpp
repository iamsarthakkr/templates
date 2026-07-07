#include <bits/stdc++.h>

using namespace std;

template <typename Node> class SegtreeImpl {
  public:
    explicit SegtreeImpl(int n) { init(n); }
    explicit SegtreeImpl(const vector<Node> &info) : SegtreeImpl((int)info.size()) { build(1, 0, m_size, info); }
    template <typename T>
    explicit SegtreeImpl(const vector<T> &info) : SegtreeImpl((int)info.size()) { build(1, 0, m_size, info); }

    int size() const { return m_origSize; }

    void set(int i, const Node &v) { setImpl(1, 0, m_size, i, v); }

    Node get(int i) { return getImpl(i); }

    Node get() const { return m_tree[1]; }

    Node calc(int l, int r) { return calcImpl(1, 0, m_size, l, r); }

    template <class F>
    void update(int i, F fn) { set(i, fn(get(i))); }

    template <typename F> int maxRight(int l, F pred, Node acc = Node::id()) {
        assert(pred(acc));
        return std::min(m_origSize, maxRightImpl(1, 0, m_size, l, pred, acc));
    };

    template <typename F> int minLeft(int r, F pred, Node acc = Node::id()) {
        assert(pred(acc));
        return std::max(0, minLeftImpl(1, 0, m_size, r, pred, acc));
    };

  private:
    void setImpl(int node, int lx, int rx, int i, const Node &v) {
        if(rx - lx == 1) {
            m_tree[node] = v;
            return;
        }
        int m = (lx + rx) >> 1;
        if(i < m)
            setImpl(node << 1, lx, m, i, v);
        else
            setImpl(node << 1 | 1, m, rx, i, v);

        recalc(node);
    }

    Node getImpl(int i) {
        int node = 1, lx = 0, rx = m_size;
        while(rx - lx > 1) {
            int m = (lx + rx) >> 1;
            if(i < m)
                rx = m, node = node << 1;
            else
                lx = m, node = node << 1 | 1;
        }
        return m_tree[node];
    }

    Node calcImpl(int node, int lx, int rx, int l, int r) {
        if(rx <= l || r <= lx) return Node::id();
        if(l <= lx && rx <= r) return m_tree[node];
        int m = (lx + rx) >> 1;
        auto s1 = calcImpl(node << 1, lx, m, l, r);
        auto s2 = calcImpl(node << 1 | 1, m, rx, l, r);
        return Node::merge(s1, s2);
    }

    template <typename F> int maxRightImpl(int node, int lx, int rx, int l, F &pred, Node &acc) {
        if(rx <= l) return l;
        if(l <= lx) {
            Node res = Node::merge(acc, m_tree[node]);
            if(pred(res)) {
                acc = std::move(res);
                return rx;
            }
            if(rx - lx == 1) return lx;
        }
        if(rx - lx == 1) return l;

        int m = (lx + rx) >> 1;

        int r1 = maxRightImpl(node << 1, lx, m, l, pred, acc);
        if(r1 < m) return r1;
        return maxRightImpl(node << 1 | 1, m, rx, l, pred, acc);
    }
    template <typename F> int minLeftImpl(int node, int lx, int rx, int r, F &pred, Node &acc) {
        if(r <= lx) return r;
        if(rx <= r) {
            Node res = Node::merge(m_tree[node], acc);
            if(pred(res)) {
                acc = std::move(res);
                return lx;
            }
            if(rx - lx == 1) return rx;
        }
        if(rx - lx == 1) return r;

        int m = (lx + rx) >> 1;

        int l1 = minLeftImpl(node << 1 | 1, m, rx, r, pred, acc);
        if(l1 > m) return l1;
        return minLeftImpl(node << 1, lx, m, r, pred, acc);
    }

  private:
    void recalc(int node) {
        m_tree[node] = Node::merge(m_tree[node << 1], m_tree[node << 1 | 1]);
    }

    void init(int n) {
        m_origSize = n;
        m_size = 1;
        while(m_size < n) m_size <<= 1;
        m_tree.resize(2 * m_size, Node::id());
    }

    template <typename T>
    void build(int node, int lx, int rx, const vector<T> &arr) {
        if(rx - lx == 1) {
            if(lx < (int)arr.size()) m_tree[node] = arr[lx];
            return;
        }
        int m = (lx + rx) >> 1;
        build(node << 1, lx, m, arr);
        build(node << 1 | 1, m, rx, arr);

        recalc(node);
    }

  private:
    int m_size;
    int m_origSize;
    vector<Node> m_tree;
};
