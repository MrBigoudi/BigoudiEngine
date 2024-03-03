#include "be_rayHit.hpp"

namespace be{

const RayHitOpt RayHit::NO_HIT = std::nullopt;

Vector3 RayHit::getBarycentricCoords() const{
    return Vector3(_Representation.x(), _Representation.y(), _Representation.z());
}

float RayHit::getParametricT() const{
    return _Representation.z();
}

}