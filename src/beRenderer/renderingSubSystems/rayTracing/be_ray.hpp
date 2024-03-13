#pragma once

#include <cmath>
#include <memory>
#include "be_model.hpp"
#include "be_vector3.hpp"
#include "be_rayHit.hpp"

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

        /**
         * Generate a random ray in the unit sphere
         * @return A ray in world space
        */
        static RayPtr generateRandomRayInUnitSphere();

        /**
         * Generate a random ray in the hemisphere
         * @param sphereCenter The position of the center of the sphere
         * @param planeNormal The normal of the hemisphere plane
         * @return A ray in world space
        */
        static RayPtr generateRandomRayInHemiSphere(const Vector3& sphereCenter, const Vector3& planeNormal);

        /**
         * Generate a random ray in the hemisphere
         * @param rayHit The last hit
         * @return A ray in world space
        */
        static RayPtr generateRandomRayInHemiSphere(const RayHit& hit);

        /**
         * Generate a random ray according to Lambertian distribution
         * @param sphereCenter The position of the center of the sphere
         * @param planeNormal The normal of the hemisphere plane
         * @return A ray in world space
        */
        static RayPtr generateRandomRayLambertianDistribution(const Vector3& sphereCenter, const Vector3& planeNormal);

        /**
         * Generate a random ray according to Lambertian distribution
         * @param rayHit The last hit
         * @return A ray in world space
        */
        static RayPtr generateRandomRayLambertianDistribution(const RayHit& hit);


        /**
         * Check if the current ray intersects the given triangle
         * @param trianglePrimitive The triangle to check intersection with
         * @param minDist The minimum distance to consider a Hit (to avoid acnea)
         * @param maxDist The maximum distance to consider a Hit
         * @return An optional Ray hit
        */
        RayHitOpt rayTriangleIntersection(const Triangle& trianglePrimitive, float minDist = 1e-3, float maxDist = INFINITY);

        /**
         * Check if the current ray intersects a sphere
         * @param sphereCenter The sphere center
         * @param sphereRadius The sphere radius
         * @return true if they intersect
        */
        bool raySphereIntersection(const Vector3& sphereCenter, float sphereRadius);

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
        bool rayBoxIntersection(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
};

}