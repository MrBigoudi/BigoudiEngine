#include "be_rayHit.hpp"

namespace be{

const RayHitOpt RayHit::NO_HIT = std::nullopt;

Vector3 RayHit::getWorldPos() const {
    Vector3 p0 = _Triangle._WorldPos0;
    Vector3 p1 = _Triangle._WorldPos1;
    Vector3 p2 = _Triangle._WorldPos2;

    Vector3 baryCoords = getBarycentricCoords();
    float b0 = baryCoords[0];
    float b1 = baryCoords[1];
    float b2 = baryCoords[2];

    return (b0 * p0) + (b1 * p1) + (b2 * p2);
}

Vector3 RayHit::getViewPos() const {
    Vector3 p0 = _Triangle._ViewPos0;
    Vector3 p1 = _Triangle._ViewPos1;
    Vector3 p2 = _Triangle._ViewPos2;

    Vector3 baryCoords = getBarycentricCoords();
    float b0 = baryCoords[0];
    float b1 = baryCoords[1];
    float b2 = baryCoords[2];

    return (b0 * p0) + (b1 * p1) + (b2 * p2);
}

Vector3 RayHit::getPos() const {
    Vector3 p0 = _Triangle._Pos0;
    Vector3 p1 = _Triangle._Pos1;
    Vector3 p2 = _Triangle._Pos2;

    Vector3 baryCoords = getBarycentricCoords();
    float b0 = baryCoords[0];
    float b1 = baryCoords[1];
    float b2 = baryCoords[2];

    return (b0 * p0) + (b1 * p1) + (b2 * p2);
}

Vector4 RayHit::getCol() const {
    Vector4 c0 = _Triangle._Col0;
    Vector4 c1 = _Triangle._Col1;
    Vector4 c2 = _Triangle._Col2;

    Vector3 baryCoords = getBarycentricCoords();
    float b0 = baryCoords[0];
    float b1 = baryCoords[1];
    float b2 = baryCoords[2];

    return b0 * c0 + b1 * c1 + b2 * c2;
}

Vector3 RayHit::getNorm() const {
    Vector3 n0 = _Triangle._Norm0;
    Vector3 n1 = _Triangle._Norm1;
    Vector3 n2 = _Triangle._Norm2;

    Vector3 baryCoords = getBarycentricCoords();
    float b0 = baryCoords[0];
    float b1 = baryCoords[1];
    float b2 = baryCoords[2];

    return (b0 * n0) + (b1 * n1) + (b2 * n2);
}

Vector3 RayHit::getWorldNorm() const {
    Vector3 n0 = Vector3::normalize((_Triangle._Model * Vector4(_Triangle._Norm0, 0.f)).xyz());
    Vector3 n1 = Vector3::normalize((_Triangle._Model * Vector4(_Triangle._Norm1, 0.f)).xyz());
    Vector3 n2 = Vector3::normalize((_Triangle._Model * Vector4(_Triangle._Norm2, 0.f)).xyz());

    Vector3 baryCoords = getBarycentricCoords();
    float b0 = baryCoords[0];
    float b1 = baryCoords[1];
    float b2 = baryCoords[2];

    return Vector3::normalize((b0 * n0) + (b1 * n1) + (b2 * n2));
}

Vector3 RayHit::getViewNorm() const {
    Vector3 n0 = _Triangle._ViewNorm0;
    Vector3 n1 = _Triangle._ViewNorm1;
    Vector3 n2 = _Triangle._ViewNorm2;

    Vector3 baryCoords = getBarycentricCoords();
    float b0 = baryCoords[0];
    float b1 = baryCoords[1];
    float b2 = baryCoords[2];

    return Vector3::normalize((b0 * n0) + (b1 * n1) + (b2 * n2));
}

Vector2 RayHit::getTex() const {
    Vector2 uv0 = _Triangle._Tex0;
    Vector2 uv1 = _Triangle._Tex1;
    Vector2 uv2 = _Triangle._Tex2;

    Vector3 baryCoords = getBarycentricCoords();
    float b0 = baryCoords[0];
    float b1 = baryCoords[1];
    float b2 = baryCoords[2];
    
    return b0 * uv0 + b1 * uv1 + b2 * uv2;
}


}