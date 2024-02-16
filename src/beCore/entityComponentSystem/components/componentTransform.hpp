#pragma once

#include "vector3.hpp"
#include "matrix4x4.hpp"

#include <cmath>

namespace be{

struct ComponentTransform{
    Vector3 _Position{};
    Vector3 _Rotation{};
    Vector3 _Scale{1.f, 1.f, 1.f};

    Matrix4x4 getModel(){
        const float c3 = cos(_Rotation.z());
        const float s3 = sin(_Rotation.z());
        const float c2 = cos(_Rotation.x());
        const float s2 = sin(_Rotation.x());
        const float c1 = cos(_Rotation.y());
        const float s1 = sin(_Rotation.y());
        return 
            Matrix4x4({
                {
                    {
                        _Scale.x() * (c1 * c3 + s1 * s2 * s3),
                        _Scale.x() * (c2 * s3),
                        _Scale.x() * (c1 * s2 * s3 - c3 * s1),
                        0.0f,
                    },
                    {
                        _Scale.y() * (c3 * s1 * s2 - c1 * s3),
                        _Scale.y() * (c2 * c3),
                        _Scale.y() * (c1 * c3 * s2 + s1 * s3),
                        0.0f,
                    },
                    {
                        _Scale.z() * (c2 * s1),
                        _Scale.z() * (-s2),
                        _Scale.z() * (c1 * c2),
                        0.0f,
                    },
                    {
                        _Position.x(), 
                        _Position.y(), 
                        _Position.z(), 
                        1.0f
                    }
                }
            });
    }
};

};