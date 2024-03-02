#pragma once

#include <memory>
#include "be_vector3.hpp"

namespace be{

class Ray;
using RayPtr = std::shared_ptr<Ray>;

class Ray{

    private:
        Vector3 _Origin = {};
        Vector3 _Direction = {};

    public:
        Ray(const Vector3& origin, const Vector3& direction):
            _Origin(origin), _Direction(direction){}
    
};

}