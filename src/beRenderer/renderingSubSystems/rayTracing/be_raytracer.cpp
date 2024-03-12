#include "be_raytracer.hpp"
#include "be_color.hpp"
#include "be_components.hpp" // IWYU pragma: keep
#include "be_gameCoordinator.hpp"
#include "be_timer.hpp"
#include "be_utilityFunctions.hpp"

namespace be{


Vector3 RayTracer::shade(RayHits& hits, uint32_t depth) const {
    if(depth > _MaxDetph){
        return Vector3::zeros();
    }
    
    RayHit closestHit = hits.getClosestHit();
    // RayPtr newRay = Ray::generateRandomRayInHemiSphere(closestHit);    
    RayPtr newRay = Ray::generateRandomRayLambertianDistribution(closestHit);    
    // RayHits bouncedHits = getHits(newRay);
    // RayHits bouncedHits = getHitsBSH(newRay);
    RayHits bouncedHits = getHitsBVH(newRay);

    if(bouncedHits.getNbHits() > 0){
        return 0.5f * shade(bouncedHits, depth+1);
    }

    float alpha = 0.5f*(newRay->getDirection().y() + 1.f);
    return (1.f - alpha)*Color::WHITE + alpha*(Vector3(0.5f, 0.7f, 1.f));
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


RayHits RayTracer::getHitsBSH(RayPtr curRay) const{
    return _BSH->getIntersections(curRay, _Frame._Camera->getPosition());
}

RayHits RayTracer::getHitsBVH(RayPtr curRay) const{
    return _BVH->getIntersections(curRay, _Frame._Camera->getPosition());
}

RayHits RayTracer::getHits(RayPtr curRay) const {
    RayHits hits{};
    for(auto& triangle : _Primitives){
        RayHitOpt hit = curRay->rayTriangleIntersection(triangle);
        if(hit.has_value()){
            hit->setDistanceToPov(_Frame._Camera->getPosition());
            hits.addHit(hit.value());
        }
    }
    return hits;
}    


void RayTracer::run(FrameInfo frame, Vector3 backgroundColor, bool verbose){
    if(!_IsRunning){
        _IsRunning = true;
        uint32_t width = _Image->getWidth();
        uint32_t height = _Image->getHeight();

        _Frame = frame;
        auto camera = frame._Camera;
        Matrix4x4 viewInv = camera->getViewInverse();
        Matrix4x4 projInv = camera->getPerspectiveInverse();

        Timer timer{};
        if(verbose){
            fprintf(stdout, "Start ray tracing at `%dx%d' resolution...\n", width, height);
        }
        timer.start();
        _Image->clear(backgroundColor);

        _Primitives = getTriangles();
        _BSH = BSHPtr(new BSH(_Primitives));
        _BVH = BVHPtr(new BVH(_Primitives));

        const uint32_t step = 1;

        for(uint j = 0; j<height; j+=step){
            #ifndef NDEBUG
            float progress = j / (height+1.f);
            displayProgressBar(progress);
            #endif

            for(uint32_t i = 0; i<width; i+=step){
                float u = static_cast<float>(i);
                float v = height - static_cast<float>(j);

                RayPtr curRay = Ray::rayAt(
                    u, v, 
                    viewInv, projInv, 
                    camera->getWidth(), camera->getHeight(), 
                    camera->getPosition()
                );

                // RayHits hits = getHits(curRay);
                // RayHits hits = getHitsBSH(curRay);
                RayHits hits = getHitsBVH(curRay);
                // RayHits hits = {};

                if(hits.getNbHits() > 0){
                    _Image->set(i, j, shade(hits), Color::SRGB);
                }
            }
        }

        if(verbose){
            fprintf(stdout, "\nRay tracing executed in `%d ms'\n", timer.getTicks());
        }
        _IsRunning = false;
    }
}
    
}