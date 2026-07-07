#include <bits/stdc++.h>

template <typename _Tp, typename _Container = std::vector<_Tp>,
          typename _Comp = std::less_equal<typename _Container::value_type>>
class Heap {
    typedef _Tp Type;
    typedef _Comp Comparator;

  public:
    Heap() {}
    virtual void insert(Type x) = 0;
    virtual Type top() = 0;
    virtual Type remove_min() = 0;
    virtual bool empty() = 0;
    virtual size_t size() = 0;
};

template <typename _Tp, typename _Container = std::vector<_Tp>,
          typename _Comp = std::less_equal<typename _Container::value_type>>
class PriorityQueue : Heap<_Tp, _Container, _Comp> {

    typedef _Tp Type;
    typedef _Comp Comparator;

  public:
    PriorityQueue() {
        value_compare = Comparator();
    }

  public:
    void insert(Type x) {
        m_h.emplace_back(x);
        shift_up(m_h, (int)m_h.size() - 1);
    }
    Type top() {
        return m_h.front();
    }
    Type remove_min() {
        Type min = m_h.front();
        std::swap(m_h.front(), m_h.back());
        m_h.pop_back();
        shift_down(m_h, (int)m_h.size(), 0);
        return min;
    }
    bool empty() { return m_h.empty(); }
    size_t size() { return m_h.size(); }

  public:
    static void sort(std::vector<Type> &a) {
        heapify(a);
        for(int i = (int)a.size() - 1; i >= 0; --i) {
            std::swap(a[0], a[i]);
            shift_down(a, i, 0);
        }
        std::reverse(a.begin(), a.end());
    }

  private:
    static int left(int i) { return 2 * i + 1; }
    static int right(int i) { return 2 * i + 2; }
    static int parent(int i) { return (i - 1) >> 1; }

    static void shift_down(std::vector<Type> &a, int size, int i) {
        static auto compare = Comparator();
        while(left(i) < size) {
            int j = left(i);
            if(right(i) < size && a[right(i)] < a[j]) j = right(i);
            if(compare(a[i], a[j])) break;
            std::swap(a[i], a[j]);
            i = j;
        }
    }
    static void shift_up(std::vector<int> &a, int i) {
        static auto compare = Comparator();
        while(i > 0 && !compare(a[parent(i)], a[i])) {
            std::swap(a[i], a[parent(i)]);
            i = parent(i);
        }
    }
    static void heapify(std::vector<int> &a) {
        for(int i = (int)a.size() - 1; i >= 0; --i) {
            shift_down(a, (int)a.size(), i);
        }
    }

  private:
    std::vector<Type> m_h;
    Comparator value_compare;
};
