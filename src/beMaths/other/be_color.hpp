#pragma once

#include "be_vector3.hpp"
#include <cmath>

namespace be{

class Color: public Vector3{
    public:
        static const Vector3 BLACK;
        static const Vector3 WHITE;

        static const Vector3 RED;
        static const Vector3 GREEN;
        static const Vector3 BLUE;

        static const Vector3 MAGENTA;
        static const Vector3 CYAN;
        static const Vector3 YELLOW;

    public:
        static float linearToSRGB(float linear){
            if (linear <= 0.0031308f) {
                return linear * 12.92f;
            } else {
                return 1.055f * std::pow(linear, 1.0f / 2.4f) - 0.055f;
            }
        }

        static Vector3 toSRGB(const Vector3& vec){
            Vector3 newVec{};
            newVec.r(linearToSRGB(vec.r()));
            newVec.g(linearToSRGB(vec.g()));
            newVec.b(linearToSRGB(vec.b()));
            return newVec;
        }
};

}