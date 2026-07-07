#include <vector>
#include <utility>

template <typename T_Iterable>
auto run_length_encoding(const T_Iterable &a) {
    using T = typename T_Iterable::value_type;

    std::vector<std::pair<T, int>> res;
    int n = (int)a.size();
    if(n == 0) return res;

    int i = 0;
    while(i < n) {
        int j = i + 1;
        while(j < n && a[j] == a[i]) ++j;
        res.emplace_back(a[i], j - i);
        i = j;
    }
    return res;
}
