#pragma once

#include "be_vector3.hpp"

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
};

}