#include "be_ray.hpp"
#include "be_matrix4x4.hpp"
#include "be_vector4.hpp"
#include "be_rayHit.hpp"

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

}