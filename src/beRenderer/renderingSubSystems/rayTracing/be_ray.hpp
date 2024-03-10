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

        Vector3 getOrigin() const {return _Origin;}
        Vector3 getDirection() const {return _Direction;}

        Vector3 at(float t) const {
            return _Origin + t*_Direction;
        } 

    public:
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
        static RayPtr rayAt(float x, float y, 
                        const Matrix4x4& viewInv, 
                        const Matrix4x4& projInv,
                        float width,
                        float height, 
                        const Vector3& cameraPos
        );
};

}