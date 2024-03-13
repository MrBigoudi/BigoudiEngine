#include "be_ray.hpp"
#include "be_matrix4x4.hpp"
#include "be_vector4.hpp"
#include "be_rayHit.hpp"
#include "be_mathsFcts.hpp"

namespace be{

/**
 * Create a ray from the camera
 * @param x The x as a float for sub-pixel sampling
 * @param y The y as a float for sub-pixel sampling
 * @param viewInv The inverse of the view matrix
 * @param projInv The inverse of the projection matrix
 * @param width The viewport width
 * @param height The viewport height
 * @param cameraPos The camera world position
 * @return A ray in world space for raytracing
 * @see RayTracing
*/
RayPtr Ray::rayAt(float x, float y, 
            const Matrix4x4& viewInv, 
            const Matrix4x4& projInv,
            float width,
            float height, 
            const Vector3& cameraPos
    ){
        
    Vector3 direction{0.f,0.f,-1.f};
    float ndc_x = (2.f * x) / width - 1.f;
    float ndc_y = 1.f - (2.f * y) / height;

    Vector4 dirNDCSpace = Vector4(ndc_x, ndc_y, -1.f, 1.f);
    Vector4 dirCameraSpace = projInv * dirNDCSpace;

    Vector4 dirWorldSpace = viewInv * dirCameraSpace;
    dirWorldSpace /= dirWorldSpace.w();
    direction = dirWorldSpace.xyz() - cameraPos;

    direction.normalize();
    return RayPtr(new Ray(cameraPos, direction));
}

/**
 * Generate a ray in the unit sphere
 * @return A ray in world space
*/
RayPtr Ray::generateRandomRayInUnitSphere(){
    Vector3 origin = Vector3();
    Vector3 direction = Vector3::normalize(Vector3::random(-1, 1));
    return RayPtr(new Ray(origin, direction));
}

/**
 * Generate a random ray in the hemisphere
 * @param sphereCenter The position of the center of the sphere
 * @param planeNormal The normal of the hemisphere plane
 * @return A ray in world space
*/
RayPtr Ray::generateRandomRayInHemiSphere(const Vector3& sphereCenter, const Vector3& planeNormal){
    RayPtr ray = Ray::generateRandomRayInUnitSphere();
    ray->_Origin = sphereCenter;
    if(Vector3::dot(ray->_Direction, planeNormal) > 0.f){ // same hemisphere
        return ray;
    }
    ray->_Direction *= -1.f;
    return ray;
}

/**
 * Generate a random ray in the hemisphere
 * @param rayHit The last hit
 * @return A ray in world space
*/
RayPtr Ray::generateRandomRayInHemiSphere(const RayHit& hit){
    return generateRandomRayInHemiSphere(hit.getWorldPos(), hit.getWorldNorm());
}

/**
 * Generate a random ray according to Lambertian distribution
 * @param sphereCenter The position of the center of the sphere
 * @param planeNormal The normal of the hemisphere plane
 * @return A ray in world space
*/
RayPtr Ray::generateRandomRayLambertianDistribution(const Vector3& sphereCenter, const Vector3& planeNormal){
    Vector3 origin = sphereCenter;
    Vector3 direction = generateRandomRayInUnitSphere()->_Direction + planeNormal;

    if(direction.isZero()){
        direction = planeNormal;
    }

    direction.normalize();
    return RayPtr(new Ray(origin, direction));
}

/**
 * Generate a random ray according to Lambertian distribution
 * @param rayHit The last hit
 * @return A ray in world space
*/
RayPtr Ray::generateRandomRayLambertianDistribution(const RayHit& hit){
    return generateRandomRayLambertianDistribution(hit.getWorldPos(), hit.getWorldNorm());
}

/**
 * Check if the current ray intersects the given triangle
 * @param trianglePrimitive The triangle to check intersection with
 * @param minDist The minimum distance to consider a Hit (to avoid acnea)
 * @param maxDist The maximum distance to consider a Hit
 * @return An optional Ray hit
*/
RayHitOpt Ray::rayTriangleIntersection(const Triangle& trianglePrimitive, float minDist, float maxDist){
    Vector3 p0 = trianglePrimitive._WorldPos0;
    Vector3 p1 = trianglePrimitive._WorldPos1;
    Vector3 p2 = trianglePrimitive._WorldPos2;

    Vector3 e0 = p1 - p0;
    Vector3 e1 = p2 - p0;

    Vector3 w = getDirection();
    Vector3 o = getOrigin();

    Vector3 tmp = Vector3::cross(e0, e1);
    if(tmp.getNorm() == 0.f){
        return RayHit::NO_HIT;
    }

    Vector3 n = Vector3::normalize(tmp);
    Vector3 q = Vector3::cross(w, e1);
    float a = Vector3::dot(e0, q);
    
    // counter clock wise order
    if(Vector3::dot(n, w) >= 0){
        return RayHit::NO_HIT;
    }

    if(Maths::isZero(a)){
        return RayHit::NO_HIT;
    }

    Vector3 s = (o-p0) / a;
    Vector3 r = Vector3::cross(s, e0);

    float b0 = Vector3::dot(s, q);
    if(b0 < 0){
        return RayHit::NO_HIT;
    }
    float b1 = Vector3::dot(r, w);
    if(b1 < 0){
        return RayHit::NO_HIT;
    }
    float b2 = 1 - b0 - b1;
    if(b2 < 0){
        return RayHit::NO_HIT;
    }

    float t = Vector3::dot(e1, r);
    if(t < minDist || t > maxDist){
        return RayHit::NO_HIT;
    }

    Vector4 res = {b2,b0,b1,t};
    return RayHit(res, trianglePrimitive);
}

/**
 * Check if the current ray intersects a sphere
 * @param sphereCenter The sphere center
 * @param sphereRadius The sphere radius
 * @return true if they intersect
*/
bool Ray::raySphereIntersection(const Vector3& sphereCenter, float sphereRadius){
    Vector3 sphereToRay = getOrigin() - sphereCenter;

    auto a = getDirection().getSquaredNorm();
    auto half_b = Vector3::dot(sphereToRay, getDirection());
    auto c = sphereToRay.getNorm() - sphereRadius*sphereRadius;
    auto discriminant = half_b*half_b - a*c;

    return discriminant >= 0;
}

/**
 * Check if the current ray intersects a box
 * @param minX The minimum x-coordinate of the box
 * @param maxX The maximum x-coordinate of the box
 * @param minY The minimum y-coordinate of the box
 * @param maxY The maximum y-coordinate of the box
 * @param minZ The minimum z-coordinate of the box
 * @param maxZ The maximum z-coordinate of the box
 * @return true if they intersect
*/
bool Ray::rayBoxIntersection(float minX, float maxX, float minY, float maxY, float minZ, float maxZ){
    float tMin = 0.0f;
    float tMax = INFINITY;

    // Check intersection with X-slabs
    float inverseRayDirX = 1.0f / getDirection().x();
    float tx1 = (minX - getOrigin().x()) * inverseRayDirX;
    float tx2 = (maxX - getOrigin().x()) * inverseRayDirX;

    tMin = std::max(tMin, std::min(tx1, tx2));
    tMax = std::min(tMax, std::max(tx1, tx2));

    // Check for early exit
    if (tMax < 0.0f || tMin > tMax) {
        return false;
    }

    // Check intersection with Y-slabs
    float inverseRayDirY = 1.0f / getDirection().y();
    float ty1 = (minY - getOrigin().y()) * inverseRayDirY;
    float ty2 = (maxY - getOrigin().y()) * inverseRayDirY;

    tMin = std::max(tMin, std::min(ty1, ty2));
    tMax = std::min(tMax, std::max(ty1, ty2));

    // Check for early exit
    if (tMax < 0.0f || tMin > tMax) {
        return false;
    }

    // Check intersection with Z-slabs
    float inverseRayDirZ = 1.0f / getDirection().z();
    float tz1 = (minZ - getOrigin().z()) * inverseRayDirZ;
    float tz2 = (maxZ - getOrigin().z()) * inverseRayDirZ;

    tMin = std::max(tMin, std::min(tz1, tz2));
    tMax = std::min(tMax, std::max(tz1, tz2));

    // Check for intersection
    return tMax >= 0.0f && tMin <= tMax;

}

}