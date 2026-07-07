#include <bits/stdc++.h>
using namespace std;

template <typename Node, typename Tag> class LazySegtreeImpl {
  public:
    explicit LazySegtreeImpl(int n) { init(n); }
    explicit LazySegtreeImpl(const vector<Node> &info) : LazySegtreeImpl((int)info.size()) { build(1, 0, m_size, info); }
    template <typename T>
    explicit LazySegtreeImpl(const vector<T> &info) : LazySegtreeImpl((int)info.size()) { build(1, 0, m_size, info); }

    int size() const { return m_origSize; }

    void set(int i, const Node &v) { setImpl(1, 0, m_size, i, v); }
    Node get(int i) { return getImpl(i); }
    Node get() const { return m_tree[1]; }
    Node calc(int l, int r) {
        assert(0 <= l && l <= r && r <= m_origSize);
        if(l == r) return Node::id();
        return calcImpl(1, 0, m_size, l, r);
    }
    void apply(int l, int r, const Tag &tag) {
        assert(0 <= l && l <= r && r <= m_origSize);
        if(l == r) return;
        applyImpl(1, 0, m_size, l, r, tag);
    }

    template <typename F> int maxRight(int l, F pred, Node acc = Node::id()) {
        assert(pred(acc));
        return std::min(maxRightImpl(1, 0, m_size, l, pred, acc), m_origSize);
    };
    template <typename F> int minLeft(int r, F pred, Node acc = Node::id()) {
        assert(pred(acc));
        return std::max(0, minLeftImpl(1, 0, m_size, r, pred, acc));
    };

  private:
    void setImpl(int node, int lx, int rx, int i, const Node &v) {
        if(rx - lx == 1) {
            m_tree[node] = v;
            m_lazy[node] = Tag::id();
            return;
        }
        propagate(node, lx, rx);
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
            propagate(node, lx, rx);
            int m = (lx + rx) >> 1;
            if(i < m) {
                rx = m;
                node = node << 1;
            } else {
                lx = m;
                node = node << 1 | 1;
            }
        }
        return m_tree[node];
    }

    Node calcImpl(int node, int lx, int rx, int l, int r) {
        if(rx <= l || r <= lx) return Node::id();
        if(l <= lx && rx <= r) return m_tree[node];

        propagate(node, lx, rx);

        int m = (lx + rx) >> 1;
        auto s1 = calcImpl(node << 1, lx, m, l, r);
        auto s2 = calcImpl(node << 1 | 1, m, rx, l, r);
        return Node::merge(s1, s2);
    }

    void applyImpl(int node, int lx, int rx, int l, int r, const Tag &tag) {
        if(rx <= l || r <= lx) { return; }
        if(l <= lx && rx <= r) {
            applyTag(node, lx, rx, tag);
            return;
        }

        propagate(node, lx, rx);

        int m = (lx + rx) >> 1;
        applyImpl(node << 1, lx, m, l, r, tag);
        applyImpl(node << 1 | 1, m, rx, l, r, tag);

        recalc(node);
    };

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
        propagate(node, lx, rx);

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

        propagate(node, lx, rx);
        int m = (lx + rx) >> 1;

        int l1 = minLeftImpl(node << 1 | 1, m, rx, r, pred, acc);
        if(l1 > m) return l1;
        return minLeftImpl(node << 1, lx, m, r, pred, acc);
    }

  private:
    void propagate(int node, int lx, int rx) {
        if(rx - lx == 1 || m_lazy[node] == Tag::id()) { return; }
        int m = (lx + rx) >> 1;
        applyTag(node << 1, lx, m, m_lazy[node]);
        applyTag(node << 1 | 1, m, rx, m_lazy[node]);
        m_lazy[node] = Tag::id();
    }

    inline void applyTag(int node, int lx, int rx, const Tag &tag) {
        if(!(tag == Tag::id())) {
            m_tree[node].apply(tag, rx - lx);
            m_lazy[node] = Tag::combine(tag, m_lazy[node]);
        }
    }

    void recalc(int node) { m_tree[node] = Node::merge(m_tree[node << 1], m_tree[node << 1 | 1]); }

    void init(int n) {
        m_origSize = n;
        m_size = 1;
        while(m_size < n) m_size <<= 1;
        m_tree.resize(2 * m_size, Node::id());
        m_lazy.resize(2 * m_size, Tag::id());
    }

    template <typename T>
    void build(int node, int lx, int rx, const vector<T> &arr) {
        if(rx - lx == 1) {
            if(lx < (int)arr.size()) m_tree[node] = arr[lx], m_lazy[node] = Tag::id();
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
    vector<Tag> m_lazy;
};
