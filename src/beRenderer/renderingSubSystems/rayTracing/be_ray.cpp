#include "be_ray.hpp"
#include "be_matrix4x4.hpp"
#include "be_vector4.hpp"

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

}