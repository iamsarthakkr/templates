#include <bits/stdc++.h>
using namespace std;

template <typename Info>
struct Segtree {
    Info node;
    int leftmost, rightmost;
    Segtree *left, *right;

    Segtree() {}

    Segtree(vector<Info> &info) {
        int size = (int)info.size();
        this->build(0, size, info);
    }

    Info calc(int l, int r) {
        // disjoint
        if(r <= leftmost || rightmost <= l) return Info();
        // covered
        if(l <= leftmost && rightmost <= r) return node;

        auto s1 = left->calc(l, r), s2 = right->calc(l, r);
        return Info::merge(s1, s2);
    }

    void set(int i, const Info &v) {
        if(rightmost - leftmost == 1) {
            node = v;
            return;
        }

        int m = (rightmost + leftmost) >> 1;
        if(i < m)
            left->set(i, v);
        else
            right->set(i, v);
        recalc();
    }

  private:
    void build(int l, int r, vector<Info> &arr) {
        this->leftmost = l;
        this->rightmost = r;

        if(r - l == 1) {
            if(l < (int)arr.size()) {
                node = arr[l];
            }
            return;
        }

        int m = (r + l) >> 1;
        left = new Segtree();
        right = new Segtree();
        left->build(l, m, arr);
        right->build(m, r, arr);
        recalc();
    }

    void recalc() {
        if(rightmost - leftmost == 1) return;
        Info::unite(node, left->node, right->node);
    }
};

struct Info {
    long long v;

    Info() { // empty element neutral
        v = 0;
    }
    Info(int v) { // single
        v = v;
    }

    static Info merge(const Info &a, const Info &b) {
        Info res;
        unite(res, a, b);
        return res;
    };

    static void unite(Info &node, const Info &a, const Info &b) {
        node.v = a.v + b.v;
    }
};
