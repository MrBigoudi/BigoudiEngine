#include "be_raytracer.hpp"
#include "be_color.hpp"
#include "be_components.hpp" // IWYU pragma: keep
#include "be_gameCoordinator.hpp"
#include "be_timer.hpp"
#include "be_trigonometry.hpp"
#include "be_utilityFunctions.hpp"

#include <omp.h>

namespace be{

RayPtr RayTracer::sampleNewRay(const RayHit& rayHit) const {
    switch(_SamplingDistribution){
        case HEMISPHERE_SAMPLING:
            return Ray::generateRandomRayInHemiSphere(rayHit);
        case LAMBERTIAN_SAMPLING:
            return Ray::generateRandomRayLambertianDistribution(rayHit); 
    }
    ErrorHandler::handle(
        __FILE__, __LINE__,
        ErrorCode::UNKNOWN_VALUE_ERROR,
        "The given sample distribution method is unkown!\n"
    );
    return nullptr;
}

Vector3 RayTracer::colorBRDF(const RayHit& rayHit) const{
    return rayHit.getCol().xyz();
}

Vector3 RayTracer::normalBRDF(const RayHit& rayHit) const{
    Vector3 normals = rayHit.getWorldNorm();
    Vector3 color = Vector3::zeros();
    color.r(std::clamp(normals.r(), 0.f, 1.f));
    color.g(std::clamp(normals.g(), 0.f, 1.f));
    color.b(std::clamp(normals.b(), 0.f, 1.f));
    return color;
}

bool RayTracer::isInShadow(RayPtr shadowRay, float distToLight) const {
    RayHits shadowHits = getHits(shadowRay);
    if(shadowHits.getNbHits() == 0){
        return false;
    }
    // check if the shadow ray hits any object before reaching the light source
    bool isInShadow = false;
    for(auto hit: shadowHits._Hits){
        RayHit shadowHit = shadowHits.getClosestHit();
        float hitDist = (shadowHit.getWorldPos() - shadowRay->getOrigin()).getNorm();
        if(hitDist < distToLight) {
            isInShadow = true;
        }
    }
    return isInShadow;
}

Vector3 RayTracer::lambertBRDF(const RayHit& rayHit, PointLightPtr light) const{
    Vector3 lightDir = Vector3::normalize((light->_Position.xyz() - rayHit.getWorldPos()));
    float diffuseFactor = std::max(0.f, Vector3::dot(rayHit.getWorldNorm(), lightDir)) / PI;
    // cast shadow ray
    RayPtr shadowRay = RayPtr(new Ray(rayHit.getWorldPos(), lightDir));
    float distToLight = (light->_Position.xyz() - rayHit.getWorldPos()).getNorm();
    if (!isInShadow(shadowRay, distToLight)){
        return (diffuseFactor * light->_Intensity) * (rayHit.getCol().xyz() * light->_Color.xyz());
    }
    return Color::BLACK;
}

Vector3 RayTracer::lambertBRDF(const RayHit& rayHit, DirectionalLightPtr light) const{
    Vector3 lightDir = Vector3::normalize(light->_Direction.xyz());
    float diffuseFactor = std::max(0.f, Vector3::dot(rayHit.getWorldNorm(), lightDir));
    // cast shadow ray
    RayPtr shadowRay = RayPtr(new Ray(rayHit.getWorldPos(), -lightDir));
    if (!isInShadow(shadowRay)){
        return (diffuseFactor * light->_Intensity) * (rayHit.getCol().xyz() * light->_Color.xyz());
    }
    return Color::BLACK;
}

Vector3 RayTracer::lambertBRDF(const RayHit& rayHit, 
        const std::vector<PointLightPtr>& pointLights,
        const std::vector<DirectionalLightPtr>& directionalLights 
    ) const{
    Vector3 color = Color::BLACK;
    // point lights
    for(const auto& pointLight : pointLights){
        color += lambertBRDF(rayHit, pointLight);
    }
    // directional lights
    for(const auto& directionalLight : directionalLights){
        color += lambertBRDF(rayHit, directionalLight);
    }
    return color;
}


Vector3 RayTracer::lambertBRDF(const RayHit& rayHit) const{
    return lambertBRDF(rayHit, _Scene->getPointLights(), _Scene->getDirectionalLights());
}



Vector3 RayTracer::shade(RayHits& hits, uint32_t depth) const {
    if(hits.getNbHits() == 0){
        return _BackgroundColor;
    }

    if(depth > _MaxBounces){
        return Color::WHITE;
    }
    
    RayHit closestHit = hits.getClosestHit();
    MaterialPtr closestHitMaterial = closestHit.getTriangle()._Material;

    Vector3 color = Vector3::zeros();
    switch(_BRDF){
        case COLOR_BRDF:
            color += colorBRDF(closestHit);
            break;
        case NORMAL_BRDF:
            color += normalBRDF(closestHit);
            break;
        case LAMBERT_BRDF:
            color += lambertBRDF(closestHit);
            break;
    }

    if(depth == _MaxBounces){
        return color;
    }

    // path tracing
    Vector3 bounceColor = Vector3::zeros();
    for(uint32_t curSubSample=0; curSubSample<_SamplesPerBounces; curSubSample++){
        RayPtr newRay = sampleNewRay(closestHit);    
        RayHits bouncedHits = getHits(newRay);

        if(bouncedHits.getNbHits() > 0){
            bounceColor += _ShadingFactor * shade(bouncedHits, depth+1);
        } else {
            bounceColor += _BackgroundColor;
        }
    }
    color += bounceColor / _SamplesPerBounces;

    return color;
}

std::vector<Triangle> RayTracer::getTriangles(){
    std::vector<Triangle> allTriangles = {};
    #ifndef NDEBUG
    fprintf(stdout, "There are %zu objects in the scene!\n", _Scene->getObjects().size());
    #endif

    _BSH.clear();
    _BVH.clear();

    for(auto obj : _Scene->getObjects()){
        
        auto model = GameCoordinator::getComponent<ComponentModel>(obj)._Model;
        auto triangles = model->getTrianglePrimitives();
        #ifndef NDEBUG
        fprintf(stdout, "there are %zu triangles in the object `%d'\n", triangles.size(), obj);
        #endif

        auto material = GameCoordinator::getComponent<ComponentMaterial>(obj)._Material;
        auto transform = GameCoordinator::getComponent<ComponentTransform>(obj)._Transform;
        Matrix4x4 modelMatrix = transform->getModelTransposed();
        Matrix4x4 viewMatrix = Matrix4x4::transpose(_Frame._Camera->getView());
        Matrix4x4 normalMat = Matrix4x4::transpose(Matrix4x4::inverse(viewMatrix*transform->getModel()));

        for(size_t k = 0; k<triangles.size(); k++){
            auto& triangle = triangles[k];

            triangle._WorldPos0 = (modelMatrix * Vector4(triangle._Pos0, 1.f)).xyz();
            triangle._WorldPos1 = (modelMatrix * Vector4(triangle._Pos1, 1.f)).xyz();
            triangle._WorldPos2 = (modelMatrix * Vector4(triangle._Pos2, 1.f)).xyz();

            triangle._ViewPos0 = (viewMatrix * Vector4(triangle._Pos0, 1.f)).xyz();
            triangle._ViewPos1 = (viewMatrix * Vector4(triangle._Pos1, 1.f)).xyz();
            triangle._ViewPos2 = (viewMatrix * Vector4(triangle._Pos2, 1.f)).xyz();

            triangle._ViewNorm0 = (normalMat * Vector4(triangle._Norm0, 0.f)).xyz();
            triangle._ViewNorm1 = (normalMat * Vector4(triangle._Norm1, 0.f)).xyz();
            triangle._ViewNorm2 = (normalMat * Vector4(triangle._Norm2, 0.f)).xyz();

            triangle._Material = material;
            triangle._Model = modelMatrix;
            triangle._NormalMat = normalMat;
        }

        addObjectToAccelerationStructures(triangles);
        allTriangles.insert(allTriangles.end(), triangles.begin(), triangles.end());
    }
    return allTriangles;
}


RayHits RayTracer::getHitsBSH(RayPtr curRay) const{
    RayHits hits{};
    for(auto& bsh: _BSH){
        bsh->getIntersections(curRay, _Frame._Camera->getPosition(), hits);
    }
    return hits;
}

RayHits RayTracer::getHitsBVH(RayPtr curRay) const{
    RayHits hits{};
    for(auto& bvh: _BVH){
        bvh->getIntersections(curRay, _Frame._Camera->getPosition(), hits);
    }
    return hits;
}

RayHits RayTracer::getHitsNaive(RayPtr curRay) const {
    RayHits hits{};
    for(auto& triangle : _Primitives){
        RayHitOpt hit = curRay->rayTriangleIntersection(triangle);
        if(hit.has_value()){
            hits.addHit(hit.value());
        }
    }
    return hits;
}    

RayHits RayTracer::getHits(RayPtr curRay) const {
    switch(_BoundingVolumeMethod){
        case NAIVE_METHOD:
            return getHitsNaive(curRay);
        case BVH_METHOD:
            return getHitsBVH(curRay);
        case BSH_METHOD:
            return getHitsBSH(curRay);
    }
    ErrorHandler::handle(
        __FILE__, __LINE__,
        ErrorCode::UNKNOWN_VALUE_ERROR,
        "The given bounding volume method is unkown!\n"
    );
    return {};
}

void RayTracer::addObjectToAccelerationStructures(const std::vector<Triangle>& triangles){
    _BSH.push_back(BSHPtr(new BSH(triangles)));
    _BVH.push_back(BVHPtr(new BVH(triangles)));
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
            fprintf(stdout, "Using OpenMP, max_threads = %d\n", omp_get_max_threads());
        }
        timer.start();
        _BackgroundColor = backgroundColor;
        _Image->clear(_BackgroundColor);

        _Primitives = getTriangles();

        float step = 1.f;
        if(_SamplesPerPixels > 1){
            step = 1.f / (_SamplesPerPixels >> 1); // more samples to make weird effects diseaper
        }

        # pragma omp parallel for
        for(uint32_t j = 0.f; j<height; j++){
            #ifndef NDEBUG
            #ifndef _OPENMP
            float progress = j / (height+1.f);
            displayProgressBar(progress);
            #else
            displayProgressBarOpenMP((height+1.f));
            #endif
            #endif

            # pragma omp parallel for
            for(uint32_t i = 0.f; i<width; i++){
                Vector3 color = Vector3::zeros();
                int nbHits = 0;

                // subpixel sampling
                for(float deltaI=0; deltaI<1; deltaI+=step){
                    for(float deltaJ=0; deltaJ<1; deltaJ+=step){
                        float u = (i+deltaI);
                        float v = height - (j+deltaJ);

                        RayPtr curRay = Ray::rayAt(
                            u, v, 
                            viewInv, projInv, 
                            camera->getWidth(), camera->getHeight(), 
                            camera->getPosition()
                        );

                        RayHits hits = getHits(curRay);
                        nbHits += hits.getNbHits();
                        color += shade(hits);
                    }
                }
                
                if(nbHits > 0){
                    color /= _SamplesPerPixels;
                    _Image->set(i, j, color, Color::SRGB);
                }
            }
        }

        if(verbose){
            fprintf(stdout, "\nRay tracing executed in `%s'\n", Timer::format(timer.getTicks()).c_str());
        }
        _IsRunning = false;
    }
}
    

Vector3 RayTracer::getClusterEstimate(const RayHit& rayHit, LightCutsTree::LightNodePtr cluster){
    switch(cluster->_Type){
        case POINT_LIGHT:{
            PointLightPtr light = PointLightPtr(static_cast<PointLight*>(cluster->_Representative.get()));
            light->_Intensity = cluster->_TotalIntensity;
            switch(_BRDF){
                case LAMBERT_BRDF:
                    return lambertBRDF(rayHit, light);
                // TODO:
                default:
                    return Vector3::zeros();
            }
        }
        case DIRECTIONAL_LIGHT:{
            DirectionalLightPtr light = DirectionalLightPtr(static_cast<DirectionalLight*>(cluster->_Representative.get()));
            light->_Intensity = cluster->_TotalIntensity;
            switch(_BRDF){
                case LAMBERT_BRDF:
                    return lambertBRDF(rayHit, light);
                // TODO:
                default:
                    return Vector3::zeros();
            }
        }
        // TODO:
        case ORIENTED_LIGHT:{
            return Vector3::zeros();
        }
    }

    return Vector3::zeros();
}

}