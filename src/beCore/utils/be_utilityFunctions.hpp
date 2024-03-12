#pragma once

#include <functional>
#include <iostream>

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

inline void displayProgressBar(float progress, int barWidth = 70){
    fprintf(stdout, "[");
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) fprintf(stdout, "=");
        else if (i == pos) fprintf(stdout, ">");
        else fprintf(stdout, " ");
    }
    fprintf(stdout, "] %.0f %%\r", progress * 100.0);
    fflush(stdout);
}

};