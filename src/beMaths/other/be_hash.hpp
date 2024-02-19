#pragma once

#include <functional>

#include "be_vector2.hpp"
#include "be_vector3.hpp"
#include "be_vector4.hpp"

namespace std{

/**
 * A structure to hash a Vector4
*/
template <>
struct hash<be::Vector4>{
    std::size_t operator()(const be::Vector4& min) const{
        std::hash<float> hasher;
        auto x = hasher(min.x());
        auto y = hasher(min.y());
        auto z = hasher(min.z());
        auto w = hasher(min.w());
        return x ^ (((y >> 3) ^ (z << 5)) ^ (w << 8));
    }
};

/**
 * A structure to hash a Vector3
*/
template <>
struct hash<be::Vector3>{
    std::size_t operator()(const be::Vector3& min) const{
        std::hash<float> hasher;
        auto x = hasher(min.x());
        auto y = hasher(min.y());
        auto z = hasher(min.z());
        return x ^ ((y >> 3) ^ (z << 5));
    }
};

/**
 * A structure to hash a Vector2
*/
template <>
struct hash<be::Vector2>{
    std::size_t operator()(const be::Vector2& min) const{
        std::hash<float> hasher;
        auto x = hasher(min.x());
        auto y = hasher(min.y());
        return x ^ (y >> 3);
    }
};

};