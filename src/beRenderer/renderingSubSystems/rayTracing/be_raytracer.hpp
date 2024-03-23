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
            GGX_BRDF,     // ggx BRDF
            DISNEY_BRDF,  // disney BRDF
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
        BRDFModel _BRDF = DISNEY_BRDF;

    public:
        uint32_t _MaxBounces = 0;
        uint32_t _SamplesPerPixels = 4;
        uint32_t _SamplesPerBounces = 8;
        float _ShadingFactor = 0.1f;
        // bool _UseLightCuts = true;
        bool _UseLightCuts = false;
        float _LightcutsErrorThreshold = 0.02f; // 2%
        float _LightcutsMinIntensity = 1e-6;
        uint32_t _LightcutsMaxClusters = 100;


    public:
        RayTracer(ScenePtr scene, uint32_t width, uint32_t height)
            : _Scene(scene){
            setResolution(width, height);
        }
        
        void run(FrameInfo frame, Vector3 backgroundColor = {});

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
        void enableGgxBRDF(){_BRDF = GGX_BRDF;}

    
    private:
        std::vector<Triangle> getTriangles();
        Vector3 shade(RayHits& hits, uint32_t depth = 0) const;
        Vector3 shadeLightCuts(RayHits& hits, uint32_t depth = 0) const;
        
        RayHits getHits(RayPtr curRay) const;
        RayHits getHitsNaive(RayPtr curRay) const;        
        RayHits getHitsBSH(RayPtr curRay) const;
        RayHits getHitsBVH(RayPtr curRay) const;

        RayPtr sampleNewRay(const RayHit& rayHit) const;

        Vector3 colorBRDF(const RayHit& rayHit) const;
        Vector3 normalBRDF(const RayHit& rayHit) const;

        // Vector3 getErrorBoundLambertBRDF(const RayHit& rayHit, LightCutsTree::LightNodePtr curNode) const;
        Vector3 lambertBRDF(const RayHit& rayHit) const;
        Vector3 lambertBRDF(const RayHit& rayHit, PointLightPtr light) const;
        Vector3 lambertBRDF(const RayHit& rayHit, DirectionalLightPtr light) const;
        Vector3 lambertBRDF(const RayHit& rayHit, 
            const std::vector<PointLightPtr>& pointLights,
            const std::vector<DirectionalLightPtr>& directionalLights 
        ) const;

        // Vector3 getErrorBoundGgxBRDF(const RayHit& rayHit, LightCutsTree::LightNodePtr curNode) const;
        Vector3 ggxBRDF(const RayHit& rayHit) const;
        Vector3 ggxBRDF(const RayHit& rayHit, PointLightPtr light) const;
        Vector3 ggxBRDF(const RayHit& rayHit, DirectionalLightPtr light) const;
        Vector3 ggxBRDF(const RayHit& rayHit, 
            const std::vector<PointLightPtr>& pointLights,
            const std::vector<DirectionalLightPtr>& directionalLights 
        ) const;

        Vector3 disneyBRDF(const RayHit& rayHit) const;
        Vector3 disneyBRDF(const RayHit& rayHit, PointLightPtr light) const;
        Vector3 disneyBRDF(const RayHit& rayHit, DirectionalLightPtr light) const;
        Vector3 disneyBRDF(const RayHit& rayHit, 
            const std::vector<PointLightPtr>& pointLights,
            const std::vector<DirectionalLightPtr>& directionalLights 
        ) const;

        void addObjectToAccelerationStructures(const std::vector<Triangle>& triangles);
        bool isInShadow(RayPtr shadowRay, float distToLight = INFINITY) const;

        Vector3 getClusterEstimate(const RayHit& rayHit, const LightCutsTree::LightNodePtr cluster) const;
        Vector3 getClusterError(const RayHit& rayHit, const LightCutsTree::LightNodePtr cluster) const;

        struct CutHeap{
            LightCutsTree::LightNodePtr _Cluster = nullptr;
            Vector3 _ClusterError{};
            Vector3 _ClusterEstimate{};
        };

        static float getCutHeapError(const CutHeap& cut) {
            if(cut._Cluster->isLeaf()){
                return 0.f;
            }

            // return _Error;
            auto err = cut._ClusterError - cut._ClusterEstimate;
            // max of err.r, err.g, err.b
            float errR = std::fabs(err.r());
            float errG = std::fabs(err.g());
            float errB = std::fabs(err.b());

            float realErrR = cut._ClusterError.r() == 0.f ? cut._ClusterEstimate.r() : errR / cut._ClusterError.r();
            float realErrG = cut._ClusterError.g() == 0.f ? cut._ClusterEstimate.g() : errG / cut._ClusterError.g();
            float realErrB = cut._ClusterError.b() == 0.f ? cut._ClusterEstimate.b() : errB / cut._ClusterError.b();

            return std::max(
                    realErrR,
                    std::max(
                        realErrG, 
                        realErrB
                    )
                );
            // return 1.f;
        }
        static bool cutHeapComparator(const CutHeap& c1, const CutHeap& c2){
            return getCutHeapError(c1) < getCutHeapError(c2);
        }

        void setError(const RayHit& rayHit, const LightCutsTree::LightNodePtr cluster) const {
            // from Stochastic Lightcuts
            float dlen2 = cluster->_AABB.getDistance(rayHit.getWorldPos());
            dlen2 *= dlen2;
            if (dlen2 < 1) dlen2 = 1; // bound the distance
            // float atten = 1 / dlen2;

            float colorIntens = (cluster->_TotalColor.r() + cluster->_TotalColor.g() + cluster->_TotalColor.b()) / 3.0f;
            if (colorIntens < _LightcutsMinIntensity){
                colorIntens = _LightcutsMinIntensity;
            }
            // cluster->_Error = atten * colorIntens;
        }
};

}