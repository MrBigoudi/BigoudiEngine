#pragma once

#include "be_model.hpp"
#include "be_ray.hpp"
#include "be_vector3.hpp"
#include "be_vector4.hpp"
#include <cmath>
#include <memory>
#include <optional>
#include <vector>

namespace be{

class RayHits;
class RayHit;
using RayHitOpt = std::optional<RayHit>;

class RayHit{
    friend RayHits;

    public:
        const static RayHitOpt NO_HIT;

    private:
        /**
         * _Representation = [b0, b1, b2, t]
        */
        Vector4 _Representation{};
        float _DistanceToPov = INFINITY;
        Triangle _Triangle = {}; 
        RayPtr _Ray = nullptr;

    public:

        RayHit(const Vector4& representation, const Triangle& triangle, RayPtr ray)
            : _Representation(representation), _Triangle(triangle), _Ray(ray){
        }

        Vector3 getBarycentricCoords() const{
            return Vector3(_Representation.x(), _Representation.y(), _Representation.z());
        }
        float getParametricT() const{
            return _Representation.z();
        }
        void setDistanceToPov(const Vector3& pov);
        float getDistanceToPov() const { return _DistanceToPov;}

        Triangle getTriangle() const {return _Triangle;}


    public:
        Vector3 getPos() const;
        Vector3 getWorldPos() const;
        Vector4 getCol() const;
        Vector3 getNorm() const;
        Vector2 getTex() const;


    public:
        static bool compareRayHit(const RayHit& hit1, const RayHit& hit2){
            return hit1._DistanceToPov > hit2._DistanceToPov;
        }

};

class RayHits{
    public:
        std::vector<RayHit> _Hits = {};

    public:
        RayHits(){
            std::make_heap(_Hits.begin(), _Hits.end(), RayHit::compareRayHit);
        };

    public:
        void addHit(const RayHit& hit){
            _Hits.push_back(hit);
            std::push_heap(_Hits.begin(), _Hits.end(), RayHit::compareRayHit);
        }

        uint32_t getNbHits() const {
            return _Hits.size();
        }

        RayHit getClosestHit() {
            std::pop_heap(_Hits.begin(), _Hits.end(), RayHit::compareRayHit);
            return _Hits.back();
        }

};

};