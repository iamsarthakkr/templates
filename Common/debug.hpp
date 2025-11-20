#pragma once

#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <array>
#include <type_traits>

namespace dbg {
inline std::string to_string(const bool &b);                                              // boolean
inline std::string to_string(const char *s);                                              // string literal
inline std::string to_string(const char &c);                                              // char
inline std::string to_string(const std::string &str);                                     // strings
template <typename A, typename B> inline std::string to_string(const std::pair<A, B> &p); // pair
template <typename T> inline std::string to_string(const std::vector<T> &a);              // vector
template <size_t t> inline std::string to_string(const std::bitset<t> &a);                // bitset
template <typename A,
          typename std::enable_if<
              !std::is_arithmetic<A>::value &&
                  !std::is_same<typename std::decay<A>::type, std::string>::value &&
                  !std::is_same<typename std::decay<A>::type, const char *>::value,
              int>::type = 0>
inline std::string to_string(const A &a);                                                  // generic container
template <typename T, size_t sz> inline std::string to_string(const std::array<T, sz> &a); // array

template <typename T,
          typename std::enable_if<std::is_arithmetic<T>::value &&
                                      !std::is_same<T, bool>::value,
                                  int>::type = 0>
inline std::string to_string(T x) {
    return std::to_string(x);
}

template <typename T, size_t sz>
inline std::string to_string(const std::array<T, sz> &a) {
    std::string res = "{";
    bool first = true;
    for(int i = 0; i < (int)sz; i++) {
        if(!first) res += ", ";
        first = false;
        res += to_string(a[i]);
    }
    res += "}";
    return res;
}

inline std::string to_string(const bool &b) {
    return (b ? "true" : "false");
}

inline std::string to_string(const std::string &str) {
    return '"' + str + '"';
}

inline std::string to_string(const char &c) {
    std::string res = "'";
    res.push_back(c);
    res += "'";
    return res;
}

inline std::string to_string(const char *s) {
    return to_string((std::string)s);
}

template <size_t t>
inline std::string to_string(const std::bitset<t> &a) {
    std::string res = "";
    for(int i = (int)t - 1; i >= 0; i--) {
        res += a[i] ? "1" : "0";
    }
    return '"' + res + '"';
}

template <typename T>
inline std::string to_string(const std::vector<T> &a) {
    std::string res = "{";
    bool first = true;
    for(int i = 0; i < (int)a.size(); i++) {
        if(!first) res += ", ";
        first = false;
        res += to_string(a[i]);
    }
    res += "}";
    return res;
}

template <typename A,
          typename std::enable_if<
              !std::is_arithmetic<A>::value &&
                  !std::is_same<typename std::decay<A>::type, std::string>::value &&
                  !std::is_same<typename std::decay<A>::type, const char *>::value,
              int>::type>
inline std::string to_string(const A &a) {
    std::string res = "{";
    bool first = true;
    for(const auto &x : a) {
        if(!first) res += ", ";
        first = false;
        res += to_string(x);
    }
    res += "}";
    return res;
}

template <typename A, typename B>
inline std::string to_string(const std::pair<A, B> &p) {
    return '(' + to_string(p.first) + ", " + to_string(p.second) + ")";
}

inline std::vector<std::string> vec_splitter(const std::string &s) {
    std::vector<std::string> res;
    std::string cur;
    int par = 0; // ()
    int brc = 0; // {}
    int brk = 0; // []

    auto trim = [](const std::string &t) -> std::string {
        size_t l = t.find_first_not_of(" \t");
        if(l == std::string::npos) return "";
        size_t r = t.find_last_not_of(" \t");
        return t.substr(l, r - l + 1);
    };

    for(char c : s) {
        if(c == ',' && par == 0 && brc == 0 && brk == 0) {
            res.push_back(trim(cur));
            cur.clear();
        } else {
            if(c == '(')
                par++;
            else if(c == ')')
                par--;
            else if(c == '{')
                brc++;
            else if(c == '}')
                brc--;
            else if(c == '[')
                brk++;
            else if(c == ']')
                brk--;
            cur += c;
        }
    }

    if(!cur.empty())
        res.push_back(trim(cur));

    return res;
}

inline void debug_out(const std::vector<std::string> &, int, int) { std::cerr << std::endl; }
template <typename Head, typename... Tail>
inline void debug_out(const std::vector<std::string> &args, int idx, int LINE_NUM, Head H, Tail... T) {
    if(idx > 0)
        std::cerr << ", ";
    else
        std::cerr << "Line(" << LINE_NUM << "): ";
    std::cerr << args[idx] << " = " << to_string(H);
    debug_out(args, idx + 1, LINE_NUM, T...);
}
} // namespace dbg

#define debug(...) dbg::debug_out(dbg::vec_splitter(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
