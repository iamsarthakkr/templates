#include <limits>
#include <ostream>

namespace packs {

// ---- Tags ----

// Null Tag
struct NullTag {
    static constexpr NullTag id() noexcept { return {}; }
    static constexpr NullTag combine(const NullTag &, const NullTag &) noexcept { return {}; }
    friend constexpr bool operator==(const NullTag &, const NullTag &) noexcept { return true; }
};

// Add to range
template <typename T>
struct AddTag {
    T add{};
    constexpr AddTag() noexcept = default;
    explicit constexpr AddTag(const T &val) noexcept : add(val) {}

    static constexpr AddTag id() noexcept { return AddTag(); }
    // top ∘ bottom  (apply bottom first, then top)
    static constexpr AddTag combine(const AddTag &top, const AddTag &bottom) noexcept {
        return AddTag{top.add + bottom.add};
    }
    friend constexpr bool operator==(const AddTag &a, const AddTag &b) noexcept { return a.add == b.add; }
};

// Assign value to range
template <typename T>
struct AssignTag {
    T assign{};
    bool has = false;

    constexpr AssignTag() noexcept = default;
    explicit constexpr AssignTag(const T &val) noexcept : assign(val), has(true) {}

    static constexpr AssignTag id() noexcept { return AssignTag(); }
    // top ∘ bottom
    static constexpr AssignTag combine(const AssignTag &top, const AssignTag &bottom) noexcept {
        return top.has ? top : bottom;
    }
    friend constexpr bool operator==(const AssignTag &a, const AssignTag &b) noexcept {
        return a.has == b.has && (!a.has || a.assign == b.assign);
    }

    // optional ergonomic helper
    static constexpr AssignTag set(const T &x) noexcept { return AssignTag(x); }
};

// ---- Unified Nodes ----

// Sum over range
template <typename T>
struct SumNode {
    T sum;
    constexpr SumNode() noexcept : sum(T(0)) {}
    explicit constexpr SumNode(const T &v) noexcept : sum(v) {}

    static constexpr SumNode id() noexcept { return SumNode(T(0)); }
    static constexpr SumNode merge(const SumNode &a, const SumNode &b) noexcept {
        return SumNode(a.sum + b.sum);
    }

    // Lazy hooks
    constexpr void apply(const NullTag &, int /*len*/) noexcept {}
    template <typename U> constexpr void apply(const AddTag<U> &tag, int len) noexcept {
        sum += static_cast<T>(tag.add) * static_cast<T>(len);
    }
    template <typename U> constexpr void apply(const AssignTag<U> &tag, int len) noexcept {
        if(tag.has) sum = static_cast<T>(tag.assign) * static_cast<T>(len);
    }
};

// Max over range
template <typename T>
struct MaxNode {
    T mx;
    constexpr MaxNode() noexcept : mx(std::numeric_limits<T>::lowest()) {}
    explicit constexpr MaxNode(const T &v) noexcept : mx(v) {}

    static constexpr MaxNode id() noexcept { return MaxNode(); }
    static constexpr MaxNode merge(const MaxNode &a, const MaxNode &b) noexcept {
        return MaxNode(a.mx < b.mx ? b.mx : a.mx);
    }

    constexpr void apply(const NullTag &, int) noexcept {}
    template <typename U> constexpr void apply(const AddTag<U> &tag, int) noexcept { mx += static_cast<T>(tag.add); }
    template <typename U> constexpr void apply(const AssignTag<U> &tag, int) noexcept {
        if(tag.has) mx = static_cast<T>(tag.assign);
    }
};

// Min over range
template <typename T>
struct MinNode {
    T mn;
    constexpr MinNode() noexcept : mn(std::numeric_limits<T>::max()) {}
    explicit constexpr MinNode(const T &v) noexcept : mn(v) {}

    static constexpr MinNode id() noexcept { return MinNode(); }
    static constexpr MinNode merge(const MinNode &a, const MinNode &b) noexcept {
        return MinNode(a.mn < b.mn ? a.mn : b.mn);
    }

    constexpr void apply(const NullTag &, int) noexcept {}
    template <typename U> constexpr void apply(const AddTag<U> &tag, int) noexcept { mn += static_cast<T>(tag.add); }
    template <typename U> constexpr void apply(const AssignTag<U> &tag, int) noexcept {
        if(tag.has) mn = static_cast<T>(tag.assign);
    }
};

// Min over range and count
template <typename T>
struct MinCountNode {
    T mn;
    int cnt;
    constexpr MinCountNode() noexcept : mn(std::numeric_limits<T>::max()), cnt(0) {}
    constexpr MinCountNode(T v, int c = 1) noexcept : mn(v), cnt(c) {}

    static constexpr MinCountNode id() noexcept { return MinCountNode(); }
    static constexpr MinCountNode merge(const MinCountNode &a, const MinCountNode &b) noexcept {
        if(a.mn < b.mn) return a;
        if(b.mn < a.mn) return b;
        return MinCountNode(a.mn, a.cnt + b.cnt);
    }

    constexpr void apply(const NullTag &, int) noexcept {}
    template <typename U> constexpr void apply(const AddTag<U> &tag, int) noexcept { mn += static_cast<T>(tag.add); }
    template <typename U> constexpr void apply(const AssignTag<U> &tag, int) noexcept {
        if(tag.has) mn = static_cast<T>(tag.assign);
    }
};

// ---- debugging ----
template <typename T>
inline std::ostream &operator<<(std::ostream &os, const SumNode<T> &n) { return os << "{sum=" << n.sum << "}"; }
template <typename T>
inline std::ostream &operator<<(std::ostream &os, const MaxNode<T> &n) { return os << "{mx=" << n.mx << "}"; }
template <typename T>
inline std::ostream &operator<<(std::ostream &os, const MinNode<T> &n) { return os << "{mn=" << n.mn << "}"; }
template <typename T>
inline std::ostream &operator<<(std::ostream &os, const MinCountNode<T> &n) { return os << "{mn=" << n.mn << ", cnt=" << n.cnt << "}"; }

} // namespace packs
