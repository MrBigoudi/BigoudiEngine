#include "be_raytracer.hpp"
#include "be_color.hpp"
#include "be_components.hpp" // IWYU pragma: keep
#include "be_gameCoordinator.hpp"
#include "be_timer.hpp"
#include "be_utilityFunctions.hpp"

namespace be{

RayHitOpt RayTracer::rayTriangleIntersection(RayPtr ray, const Triangle& trianglePrimitive){
    Vector3 p0 = trianglePrimitive._WorldPos0;
    Vector3 p1 = trianglePrimitive._WorldPos1;
    Vector3 p2 = trianglePrimitive._WorldPos2;

    Vector3 e0 = p1 - p0;
    Vector3 e1 = p2 - p0;

    Vector3 w = ray->getDirection();
    Vector3 o = ray->getOrigin();

    Vector3 tmp = Vector3::cross(e0, e1);
    if(tmp.getNorm() == 0.f){
        return RayHit::NO_HIT;
    }

    Vector3 n = Vector3::normalize(tmp);
    Vector3 q = Vector3::cross(w, e1);
    float a = Vector3::dot(e0, q);
    
    // counter clock wise order
    if(Vector3::dot(n, w) >= 0){
        return RayHit::NO_HIT;
    }

    if(isZero(a)){
        return RayHit::NO_HIT;
    }

    Vector3 s = (o-p0) / a;
    Vector3 r = Vector3::cross(s, e0);

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
    return RayHit(res, trianglePrimitive, ray);
}

Vector3 RayTracer::shade(RayHits& hits) const {
    // display normals
    RayHit closestHit = hits.getClosestHit();

    Vector3 color = closestHit.getNorm(_Frame._Camera->getView()); // norm tmp
    // Vector3 color = Color::RED;
    // Vector3 color = closestHit.getPos(); // local pos
    // Vector3 color = closestHit.getTriangle()._Norm0;
    // Vector3 color = closestHit.getBarycentricCoords();

    color.r(std::max(color.r(), 0.f));
    color.g(std::max(color.g(), 0.f));
    color.b(std::max(color.b(), 0.f));

    return Color::toSRGB(color);
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
        fprintf(stdout, "there are %zu triangles in the object `%d'\n", triangles.size(), obj);
        #endif

        auto material = GameCoordinator::getComponent<ComponentMaterial>(obj)._Material;
        auto transform = GameCoordinator::getComponent<ComponentTransform>(obj)._Transform;
        Matrix4x4 modelMatrix = transform->getModelTransposed();
        // fprintf(stdout, "model:\n%s\n", modelMatrix.toString().c_str());

        for(size_t k = 0; k<triangles.size(); k++){
            auto& triangle = triangles[k];

            triangle._WorldPos0 = (modelMatrix * Vector4(triangle._Pos0, 1.f)).xyz();
            triangle._WorldPos1 = (modelMatrix * Vector4(triangle._Pos1, 1.f)).xyz();
            triangle._WorldPos2 = (modelMatrix * Vector4(triangle._Pos2, 1.f)).xyz();

            triangle._Material = material;
            triangle._Model = modelMatrix;

            // fprintf(stdout, "triangle[%zu]:%s\n", k, triangle.toString().c_str());
        }


        allTriangles.insert(allTriangles.end(), triangles.begin(), triangles.end());
    }

    return allTriangles;
}


Vector3 RayTracer::getHitWorldPosition(const RayHit& hit, const Ray& curRay){
    float t = hit.getParametricT();
    return curRay.at(t);
}



void RayTracer::run(FrameInfo frame, Vector3 backgroundColor, bool verbose){
    if(!_IsRunning){
        _IsRunning = true;
        uint32_t width = _Image->getWidth();
        uint32_t height = _Image->getHeight();

        _Frame = frame;

        Timer timer{};
        if(verbose){
            fprintf(stdout, "Start ray tracing at `%dx%d' resolution...\n", width, height);
        }
        timer.start();
        _Image->clear(backgroundColor);

        auto primitives = getTriangles();

        const uint32_t step = 1;

        std::set<size_t> triangleIds{};

        for(uint j = 0; j<height; j+=step){
            #ifndef NDEBUG
            float progress = j / (height+1.f);
            displayProgressBar(progress);
            #endif

            for(uint32_t i = 0; i<width; i+=step){
                float u = static_cast<float>(i);
                float v = height - static_cast<float>(j);

                // u = (u + 0.5f) / width;
                // v = 1.f - (v + 0.5f) / height;
                RayPtr curRay = RayPtr(new Ray(frame._Camera->rayAt(u,v)));

                RayHits hits{};

                for(size_t k=0; k<primitives.size(); k++){
                    auto& triangle = primitives[k];
                // for(auto& triangle : primitives){
                    RayHitOpt hit = rayTriangleIntersection(curRay, triangle);
                    if(hit.has_value()){
                        triangleIds.insert(k);
                        hit->setDistanceToPov(frame._Camera->getPosition());
                        hits.addHit(hit.value());
                    }
                }

                if(hits.getNbHits() > 0){
                    _Image->set(i, j, shade(hits));
                }
            }
        }

        // fprintf(stdout, "%zu triangles hit: ", triangleIds.size());
        // for(auto k : triangleIds){
        //     fprintf(stdout, "%zu, ", k);
        // }
        // fprintf(stdout, "\n");

        if(verbose){
            fprintf(stdout, "\nRay tracing executed in `%d ms'\n", timer.getTicks());
        }
        _IsRunning = false;
    }
}
    
}