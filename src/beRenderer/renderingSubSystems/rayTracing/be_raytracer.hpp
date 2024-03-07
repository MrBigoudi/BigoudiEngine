#pragma once

#include <memory>
#include "be_frameInfo.hpp"
#include "be_image.hpp"
#include "be_model.hpp"
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

        static RayHitOpt rayTriangleIntersection(RayPtr ray, const Triangle& trianglePrimitive);
        static Vector3 getHitWorldPosition(const RayHit& hit, const Ray& curRay);

    
    private:
        std::vector<Triangle> getTriangles() const;
        Vector3 shade(RayHits& hits) const;


};

}