#include "be_raytracer.hpp"
#include "be_color.hpp"
#include "be_components.hpp" // IWYU pragma: keep
#include "be_gameCoordinator.hpp"
#include "be_timer.hpp"
#include "be_utilityFunctions.hpp"

namespace be{

RayHitOpt RayTracer::rayTriangleIntersection(const Ray& ray, const Triangle& trianglePrimitive){
    return rayTriangleIntersection(ray, trianglePrimitive.p0, trianglePrimitive.p1, trianglePrimitive.p2);
}

RayHitOpt RayTracer::rayTriangleIntersection(const Ray& ray, const Vector3& p0, const Vector3& p1, const Vector3& p2){
    Vector3 e0 = p1 - p0;
    Vector3 e1 = p2 - p0;

    // fprintf(stdout,
    //     "e0: %s\ne1: %s\n",
    //     e0.toString().c_str(),
    //     e1.toString().c_str()
    // );

    Vector3 w = ray.getDirection();
    Vector3 o = ray.getOrigin();

    // fprintf(stdout,
    //     "w: %s\no: %s\n",
    //     w.toString().c_str(),
    //     o.toString().c_str()
    // );

    Vector3 n = Vector3::normalize(Vector3::cross(e0, e1));
    Vector3 q = Vector3::cross(w, e1);
    float a = Vector3::dot(e0, q);

    // fprintf(stdout,
    //     "n: %s\nq: %s\na: %f\n",
    //     n.toString().c_str(),
    //     q.toString().c_str(),
    //     a
    // );
    
    // counter clock wise order
    if(Vector3::dot(n, w) <= 0){
        return RayHit::NO_HIT;
    }

    if(isZero(a)){
        return RayHit::NO_HIT;
    }

    Vector3 s = (o-p0) / a;
    Vector3 r = Vector3::cross(s, e0);

    // fprintf(stdout,
    //     "s: %s\nr: %s\n",
    //     s.toString().c_str(),
    //     r.toString().c_str()
    // );


    float b0 = Vector3::dot(s, q);
    if(b0 < 0){
        return RayHit::NO_HIT;
    }
    float b1 = Vector3::dot(r, w);
    if(b1 < 0){
        return RayHit::NO_HIT;
    }
    float b2 = 1 - b0 - b1;
    if(b2 < 0){
        return RayHit::NO_HIT;
    }

    float t = Vector3::dot(e1, r);
    if(t < 0){
        return RayHit::NO_HIT;
    }

    Vector4 res = {b0,b1,b2,t};
    // fprintf(stdout, "res: %s\n", res.toString().c_str());
    return RayHit(res);
}


std::vector<Triangle> RayTracer::getTriangles() const{
    std::vector<Triangle> allTriangles = {};
    #ifndef NDEBUG
    fprintf(stdout, "There are %zu objects in the scene!\n", _Scene->getObjects().size());
    #endif

    for(auto obj : _Scene->getObjects()){
        auto model = GameCoordinator::getComponent<ComponentModel>(obj)._Model;
        auto triangles = model->getTrianglePrimitives();

        #ifndef NDEBUG
        fprintf(stdout, "there are %zu triangles in the scene\n", triangles.size());
        fprintf(stdout, 
            "po: %s, p1: %s, p2: %s\n",
            triangles[0].p0.toString().c_str(), triangles[0].p1.toString().c_str(), triangles[0].p2.toString().c_str()
        );
        #endif

        allTriangles.insert(allTriangles.end(), triangles.begin(), triangles.end());
    }

    return allTriangles;
}



void RayTracer::run(Vector3 backgroundColor, bool verbose){
    if(!_IsRunning){
        _IsRunning = true;
        uint32_t width = _Image->getWidth();
        uint32_t height = _Image->getHeight();

        Timer timer{};
        if(verbose){
            fprintf(stdout, "Start ray tracing at `%dx%d' resolution...\n", width, height);
        }
        timer.start();
        _Image->clear(backgroundColor);

        auto primitives = getTriangles();
        
        for(uint j = 0; j<height; j++){
            for(uint32_t i = 0; i<width; i++){
                float u = (static_cast<float>(i) + 0.5f) / width;
                float v = 1.f - (static_cast<float>(j) + 0.5f) / height;
                Ray curRay = _Camera->rayAt(u,v);
                // if(j%100 == 0 && i%100 == 0){
                //     fprintf(stdout, "j: %d, i:%d\n\tray pos: %s\n\tray dir: %s\n", 
                //             j, i, curRay.getOrigin().toString().c_str(), 
                //             curRay.getDirection().toString().c_str()
                //     );
                // }

                for(auto& triangle : primitives){
                    RayHitOpt hit = rayTriangleIntersection(curRay, triangle);
                    if(hit.has_value()){
                        _Image->set(i, j, Color::RED); // red tmp
                        break; // for now
                    }
                }

                // // gradient color for now
                // float a = 0.5f*(curRay.getDirection().y() + 1.f);
                // Vector3 color = (1.f-a)*Color::WHITE + a*Vector3(0.5f, 0.7f, 1.0f);
                // // color = (curRay.getDirection() + Vector3(1.f,1.f,1.f)) * 0.5f;
                // _Image->set(i, j, color);
            }
        }

        if(verbose){
            fprintf(stdout, "Ray tracing executed in `%d ms'\n", timer.getTicks());
        }
        _IsRunning = false;
    }
}
    
}