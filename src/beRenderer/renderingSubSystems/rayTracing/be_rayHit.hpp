#pragma once

#include "be_vector3.hpp"
#include "be_vector4.hpp"
#include <memory>
#include <optional>

namespace be{

class RayHit;
using RayHitOpt = std::optional<RayHit>;

class RayHit{
    public:
        const static RayHitOpt NO_HIT;

    private:
        /**
         * _Representation = [b0, b1, b2, t]
        */
        Vector4 _Representation{};

    public:
        RayHit(const Vector4& representation): _Representation(representation){
        }

        Vector3 getBarycentricCoords() const;
        float getParametricT() const;

};

};