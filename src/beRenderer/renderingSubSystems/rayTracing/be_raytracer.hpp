#pragma once

#include <memory>
#include "be_boundingVolume.hpp"
#include "be_frameInfo.hpp"
#include "be_image.hpp"
#include "be_model.hpp"
#include "be_ray.hpp"
#include "be_rayHit.hpp"
#include "be_scene.hpp"

namespace be{

class RayTracer;
using RayTracerPtr = std::shared_ptr<RayTracer>;

class RayTracer{
    private:
        enum BoundingVolumeMethod{
            NAIVE_METHOD, // checking every triangles at all times
            BVH_METHOD,   // using bounding volume hierarchy with AABB
            BSH_METHOD,   // using bounding spheres hierarchy
        };

        enum SamplingDistribution{
            HEMISPHERE_SAMPLING, // sample bouncing rays on a hemisphere
            LAMBERTIAN_SAMPLING, // sample bouncing rays using lambertian sampling
        };

    private:
        Vector3 _BackgroundColor = Color::WHITE;
        ImagePtr _Image = nullptr;
        ScenePtr _Scene = nullptr;
        bool _IsRunning = false;
        FrameInfo _Frame;
        std::vector<Triangle> _Primitives = {};
        BSHPtr _BSH = nullptr;
        BVHPtr _BVH = nullptr;

    private:
        // raytracing parameters
        BoundingVolumeMethod _BoundingVolumeMethod = BVH_METHOD;
        SamplingDistribution _SamplingDistribution = LAMBERTIAN_SAMPLING;
        uint32_t _MaxBounces = 10;
        uint32_t _SamplesPerPixels = 2;
        float _ShadingFactor = 0.5f;


    public:
        RayTracer(ScenePtr scene, uint32_t width, uint32_t height)
            : _Scene(scene){
            setResolution(width, height);
        }
        
        void run(FrameInfo frame, Vector3 backgroundColor = {}, bool verbose = false);

    public:
        ImagePtr getImage() const { 
            return _Image;
        }
        
        void setScene(ScenePtr scene){
            _Scene = scene;
        }

        void setResolution(uint32_t width, uint32_t height){
            _Image = std::make_shared<Image>(width, height);
        }

    
    private:
        std::vector<Triangle> getTriangles() const;
        Vector3 shade(RayHits& hits, uint32_t depth = 0) const;
        RayHits getHits(RayPtr curRay) const;
        RayHits getHitsNaive(RayPtr curRay) const;        
        RayHits getHitsBSH(RayPtr curRay) const;
        RayHits getHitsBVH(RayPtr curRay) const;

        RayPtr sampleNewRay(const RayHit& rayHit) const;
};

}