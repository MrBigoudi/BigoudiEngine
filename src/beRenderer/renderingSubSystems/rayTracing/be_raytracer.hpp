#pragma once

#include <memory>
#include "be_color.hpp"
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
        CameraPtr _Camera = nullptr;

    public:
        RayTracer(ScenePtr scene, CameraPtr camera, uint32_t width, uint32_t height)
            : _Scene(scene), _Camera(camera){
            setResolution(width, height);
        }
        
        void run(Vector3 backgroundColor = {}, bool verbose = false);

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

        static RayHitOpt rayTriangleIntersection(const Ray& ray, const Vector3& p0, const Vector3& p1, const Vector3& p2);
        static RayHitOpt rayTriangleIntersection(const Ray& ray, const Triangle& trianglePrimitive);

    
    private:
        std::vector<Triangle> getTriangles() const;
        Color shade(RayHit hit, Triangle triangle) const;

};

}