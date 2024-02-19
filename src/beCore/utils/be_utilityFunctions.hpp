#pragma once

#include <functional>

namespace be{

template <class T>
inline void hashCombine(std::size_t& seed, const T& v) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <class T, class... Rest>
inline void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
    hashCombine(seed, v);
    hashCombine(seed, rest...);
}

};