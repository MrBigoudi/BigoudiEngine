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
        ImagePtr _Image = nullptr;
        ScenePtr _Scene = nullptr;
        bool _IsRunning = false;
        FrameInfo _Frame;
        std::vector<Triangle> _Primitives = {};

        BSHPtr _BSH = nullptr;
        BVHPtr _BVH = nullptr;

        uint32_t _MaxDetph = 2;


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
        RayHits getHitsBSH(RayPtr curRay) const;
        RayHits getHitsBVH(RayPtr curRay) const;
};

}