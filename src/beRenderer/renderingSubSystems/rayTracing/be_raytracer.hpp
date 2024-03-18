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
    public:
        enum BoundingVolumeMethod{
            NAIVE_METHOD, // checking every triangles at all times
            BVH_METHOD,   // using bounding volume hierarchy with AABB
            BSH_METHOD,   // using bounding spheres hierarchy
        };

        enum SamplingDistribution{
            HEMISPHERE_SAMPLING, // sample bouncing rays on a hemisphere
            LAMBERTIAN_SAMPLING, // sample bouncing rays using lambertian sampling
        };

        enum BRDFModel{
            COLOR_BRDF,   // simple color pass through
            NORMAL_BRDF,  // simple normal pass through
            LAMBERT_BRDF, // lambert BRDF
        };

    private:
        Vector3 _BackgroundColor = Color::WHITE;
        ImagePtr _Image = nullptr;
        ScenePtr _Scene = nullptr;
        bool _IsRunning = false;
        FrameInfo _Frame;
        std::vector<Triangle> _Primitives = {};
        std::vector<BSHPtr> _BSH = {};
        std::vector<BVHPtr> _BVH = {};

    private:
        // raytracing parameters
        BoundingVolumeMethod _BoundingVolumeMethod = BVH_METHOD;
        SamplingDistribution _SamplingDistribution = LAMBERTIAN_SAMPLING;
        BRDFModel _BRDF = COLOR_BRDF;
        uint32_t _MaxBounces = 0;
        uint32_t _SamplesPerPixels = 1;
        uint32_t _SamplesPerBounces = 2;
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
        void enableColorBRDF(){_BRDF = COLOR_BRDF;}
        void enableNormalBRDF(){_BRDF = NORMAL_BRDF;}
        void enableLambertBRDF(){_BRDF = LAMBERT_BRDF;}

    
    private:
        std::vector<Triangle> getTriangles();
        Vector3 shade(RayHits& hits, uint32_t depth = 0) const;
        RayHits getHits(RayPtr curRay) const;
        RayHits getHitsNaive(RayPtr curRay) const;        
        RayHits getHitsBSH(RayPtr curRay) const;
        RayHits getHitsBVH(RayPtr curRay) const;

        RayPtr sampleNewRay(const RayHit& rayHit) const;

        Vector3 colorBRDF(const RayHit& rayHit) const;
        Vector3 normalBRDF(const RayHit& rayHit) const;
        Vector3 lambertBRDF(const RayHit& rayHit) const;
        Vector3 lambertBRDF(const RayHit& rayHit, PointLightPtr light) const;
        Vector3 lambertBRDF(const RayHit& rayHit, DirectionalLightPtr light) const;
        Vector3 lambertBRDF(const RayHit& rayHit, 
            const std::vector<PointLightPtr>& pointLights,
            const std::vector<DirectionalLightPtr>& directionalLights 
        ) const;

        void addObjectToAccelerationStructures(const std::vector<Triangle>& triangles);
        bool isInShadow(RayPtr shadowRay, float distToLight = INFINITY) const;

        Vector3 getClusterEstimate(const RayHit& rayHit, LightCutsTree::LightNodePtr cluster);
};

}