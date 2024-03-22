#include "be_raytracer.hpp"
#include "be_GGX.hpp"
#include "be_disney.hpp"
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
    for(auto hit: shadowHits._Hits){
        if(hit.getTriangle()._IsLight){
            continue;
        }
        float hitDist = (hit.getWorldPos() - shadowRay->getOrigin()).getNorm();
        if(hitDist < distToLight) {
            return true;
        }
    }
    return false;
}


Vector3 RayTracer::disneyBRDF(const RayHit& rayHit) const{
    return disneyBRDF(rayHit, _Scene->getPointLights(), _Scene->getDirectionalLights());
}

Vector3 RayTracer::disneyBRDF(const RayHit& rayHit, 
            const std::vector<PointLightPtr>& pointLights,
            const std::vector<DirectionalLightPtr>& directionalLights 
        ) const{
    Vector3 color = Color::BLACK;
    // point lights
    for(const auto& pointLight : pointLights){
        color += disneyBRDF(rayHit, pointLight);
    }
    // directional lights
    for(const auto& directionalLight : directionalLights){
        color += disneyBRDF(rayHit, directionalLight);
    }
    return color;
}

Vector3 RayTracer::disneyBRDF(const RayHit& rayHit, PointLightPtr light) const{
    Vector3 hitWorldPos = rayHit.getWorldPos();

    Vector3 wo = Vector3::normalize(light->_Position.xyz() - hitWorldPos);
    Vector3 wi = Vector3::normalize(-rayHit.getDirection());
    MaterialPtr material = rayHit.getTriangle()._Material;

    Vector3 hitNormal = rayHit.getWorldNorm();
    Vector3 surfaceColor = rayHit.getCol().xyz();

    Vector3 materialReflectance = Disney::BRDF(
        wi, wo,
        hitNormal, 
        surfaceColor, 
        material,
        light
    );

    Vector3 lightRadiance = Disney::getAttenuation(light, hitWorldPos);
    float wiDotN = std::max(0.f, Vector3::dot(wo, hitNormal));

    // cast shadow ray
    Vector3 lightDir = Vector3::normalize((light->_Position.xyz() - rayHit.getWorldPos()));
    RayPtr shadowRay = RayPtr(new Ray(rayHit.getWorldPos(), lightDir));
    float distToLight = (light->_Position.xyz() - rayHit.getWorldPos()).getNorm();
    if (!isInShadow(shadowRay, distToLight)){
        return lightRadiance * materialReflectance * wiDotN;
    }
    return Color::BLACK;
}


Vector3 RayTracer::disneyBRDF(const RayHit& rayHit, DirectionalLightPtr light) const{
    Vector3 wo = -light->_Direction.xyz();
    Vector3 wi = -rayHit.getDirection();
    MaterialPtr material = rayHit.getTriangle()._Material;

    Vector3 hitNormal = rayHit.getWorldNorm();
    Vector3 surfaceColor = rayHit.getCol().xyz();

   Vector3 materialReflectance = Disney::BRDF(
        wi, wo,
        hitNormal, 
        surfaceColor, 
        material,
        light
    );

    Vector3 lightRadiance = Disney::getAttenuation(light);
    float wiDotN = std::max(0.f, Vector3::dot(wo, hitNormal));

    // cast shadow ray
    Vector3 lightDir = Vector3::normalize(light->_Direction.xyz());
    RayPtr shadowRay = RayPtr(new Ray(rayHit.getWorldPos(), -lightDir));
    if (!isInShadow(shadowRay)){
        return lightRadiance * materialReflectance * wiDotN;
    }
    return Color::BLACK;
}


Vector3 RayTracer::ggxBRDF(const RayHit& rayHit) const{
    return ggxBRDF(rayHit, _Scene->getPointLights(), _Scene->getDirectionalLights());
}

Vector3 RayTracer::ggxBRDF(const RayHit& rayHit, 
            const std::vector<PointLightPtr>& pointLights,
            const std::vector<DirectionalLightPtr>& directionalLights 
        ) const{
    Vector3 color = Color::BLACK;
    // point lights
    for(const auto& pointLight : pointLights){
        color += ggxBRDF(rayHit, pointLight);
    }
    // directional lights
    for(const auto& directionalLight : directionalLights){
        color += ggxBRDF(rayHit, directionalLight);
    }
    return color;
}

Vector3 RayTracer::ggxBRDF(const RayHit& rayHit, PointLightPtr light) const{
    Vector3 hitWorldPos = rayHit.getWorldPos();

    Vector3 wi = Vector3::normalize(light->_Position.xyz() - hitWorldPos);
    Vector3 wo = Vector3::normalize(-rayHit.getDirection());
    MaterialPtr material = rayHit.getTriangle()._Material;

    Vector3 hitNormal = rayHit.getWorldNorm();
    Vector3 surfaceColor = rayHit.getCol().xyz();

    Vector3 materialReflectance = GGX::BRDF(
        wi, wo,
        hitNormal, 
        surfaceColor, 
        material->_Roughness, 
        material->_Metallic
    );

    Vector3 lightRadiance = GGX::getAttenuation(light, hitWorldPos);
    float wiDotN = std::max(0.f, Vector3::dot(wi, hitNormal));

    // cast shadow ray
    Vector3 lightDir = Vector3::normalize((light->_Position.xyz() - rayHit.getWorldPos()));
    RayPtr shadowRay = RayPtr(new Ray(rayHit.getWorldPos(), lightDir));
    float distToLight = (light->_Position.xyz() - rayHit.getWorldPos()).getNorm();
    if (!isInShadow(shadowRay, distToLight)){
        return lightRadiance * materialReflectance * wiDotN;
    }
    return Color::BLACK;
}


Vector3 RayTracer::ggxBRDF(const RayHit& rayHit, DirectionalLightPtr light) const{
    Vector3 wi = -light->_Direction.xyz();
    Vector3 wo = -rayHit.getDirection();
    MaterialPtr material = rayHit.getTriangle()._Material;

    Vector3 hitNormal = rayHit.getWorldNorm();
    Vector3 surfaceColor = rayHit.getCol().xyz();

    Vector3 materialReflectance = GGX::BRDF(
        wi, wo,
        hitNormal, 
        surfaceColor, 
        material->_Roughness, 
        material->_Metallic
    );

    Vector3 lightRadiance = GGX::getAttenuation(light);
    float wiDotN = std::max(0.f, Vector3::dot(wi, hitNormal));

    // cast shadow ray
    Vector3 lightDir = Vector3::normalize(light->_Direction.xyz());
    RayPtr shadowRay = RayPtr(new Ray(rayHit.getWorldPos(), -lightDir));
    if (!isInShadow(shadowRay)){
        return lightRadiance * materialReflectance * wiDotN;
    }
    return Color::BLACK;
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

// Vector3 RayTracer::getErrorBoundLambertBRDF(const RayHit& rayHit, LightCutsTree::LightNodePtr curNode) const{
//     Vector3 worlPosHit = rayHit.getWorldPos();
//     // distance between the hit point and the center of the AABB
//     float distToLight = curNode->_AABB.getDistance( worlPosHit);
//     // direction from the center of the AABB to the hit
//     Vector3 lightDir = (curNode->_AABB.getClosestPoint(worlPosHit) - worlPosHit);
//     if(lightDir.getSquaredNorm() < EPSILON){
//         return curNode->_Representative->getColor() * rayHit.getCol().xyz();
//     }
//     float diffuseFactor = std::max(0.f, Vector3::dot(rayHit.getWorldNorm(), lightDir)) / PI;
//     // cast shadow ray
//     RayPtr shadowRay = RayPtr(new Ray(worlPosHit, lightDir));
//     if (!isInShadow(shadowRay, distToLight)){
//         return diffuseFactor * (rayHit.getCol().xyz() * curNode->_Representative->getColor());
//     }
//     return Color::BLACK;
// }

// Vector3 RayTracer::getClusterEstimate(const RayHit& rayHit, LightCutsTree::LightNodePtr cluster) const {
//     switch(cluster->_Type){
//         case POINT_LIGHT:{
//             PointLightPtr pointLight = std::dynamic_pointer_cast<PointLight>(cluster->_Representative);
//             PointLightPtr light = PointLightPtr(new PointLight());
//             light->_Color = pointLight->_Color;
//             light->_Position = pointLight->_Position;
//             light->_Intensity = 1.f;
//             switch(_BRDF){
//                 case LAMBERT_BRDF:
//                     return lambertBRDF(rayHit, light) * cluster->_TotalIntensity;
//                 // TODO:
//                 default:
//                     return Vector3::zeros();
//             }
//         }
//         case DIRECTIONAL_LIGHT:{
//             DirectionalLightPtr directionalLight = std::dynamic_pointer_cast<DirectionalLight>(cluster->_Representative);
//             DirectionalLightPtr light = DirectionalLightPtr(new DirectionalLight());
//             light->_Color = directionalLight->_Color;
//             light->_Direction = directionalLight->_Direction;
//             light->_Intensity = 1.f;
//             switch(_BRDF){
//                 case LAMBERT_BRDF:
//                     return lambertBRDF(rayHit, light) * cluster->_TotalIntensity;
//                 // TODO:
//                 default:
//                     return Vector3::zeros();
//             }
//         }
//         // TODO:
//         case ORIENTED_LIGHT:{
//             return Vector3::zeros();
//         }
//     }
// 
//     return Vector3::zeros();
// }


Vector3 RayTracer::getClusterEstimate(const RayHit& rayHit, LightCutsTree::LightNodePtr cluster) const {
    if(rayHit.getTriangle()._IsLight){
        return colorBRDF(rayHit);
    }
    switch(cluster->_Type){
        case POINT_LIGHT:{
            PointLightPtr pointLight = std::dynamic_pointer_cast<PointLight>(cluster->_Representative);
            float visibility = 1.f; // already in Lambert shadow ray
            float geometric = 1.f; 
            // float geometric = cluster->getGeometric(rayHit.getWorldPos());
            float intensity = cluster->_TotalIntensity;
            Vector3 material{};
            switch(_BRDF){
                case LAMBERT_BRDF:
                    material = lambertBRDF(rayHit, pointLight) / pointLight->getIntensity();
                    break;
                case GGX_BRDF:
                    material = ggxBRDF(rayHit, pointLight) / pointLight->getIntensity();
                    break;
                case DISNEY_BRDF:
                    material = disneyBRDF(rayHit, pointLight) / pointLight->getIntensity();
                default:
                    break;
            } 
            // cluster->_BRDF = material * intensity;
            return material*geometric*visibility*intensity; 
        }
        case DIRECTIONAL_LIGHT:{
            DirectionalLightPtr directionalLight = std::dynamic_pointer_cast<DirectionalLight>(cluster->_Representative);
            float visibility = 1.f; // already in Lambert shadow ray
            float geometric = 1.f; 
            // float geometric = cluster->getGeometric(rayHit.getWorldPos());
            float intensity = cluster->_TotalIntensity;
            Vector3 material{};
            switch(_BRDF){
                case LAMBERT_BRDF:
                    material = lambertBRDF(rayHit, directionalLight) / directionalLight->getIntensity();
                    break;
                case GGX_BRDF:
                    material = ggxBRDF(rayHit, directionalLight) / directionalLight->getIntensity();
                    break;
                case DISNEY_BRDF:
                    material = disneyBRDF(rayHit, directionalLight) / directionalLight->getIntensity();
                    break;
                default:
                    break;
            } 
            return material*geometric*visibility*intensity; 
        }
        default:
            return Vector3{};
    }
}

Vector3 RayTracer::getClusterError(const RayHit& rayHit, LightCutsTree::LightNodePtr cluster) const {
    if(rayHit.getTriangle()._IsLight){
        return colorBRDF(rayHit);
    }
    
    float visibility = cluster->getVisibility();
    float geometric = cluster->getGeometricBound(rayHit.getWorldPos());
    float intensity = cluster->_TotalIntensity;
    // material upper bound
    Vector3 material{};
    // diffuse
    Vector3 diffuseBound = cluster->_TotalColor * rayHit.getCol().xyz();
    // cosine upper bound
    float theta = std::acos(
        be::Vector3::dot(rayHit.getWorldNorm(), {0.f, 0.f, 1.f})
    );
    AxisAlignedBoundingBox rotatedAABB = cluster->_AABB.rotate(theta);
    float cosBound = 1.f;
    float maxZ = rotatedAABB._MaxZ;
    float maxZ2 = maxZ*maxZ;
    if(maxZ >= 0){
        float minX2 = std::min(rotatedAABB._MinX*rotatedAABB._MinX, rotatedAABB._MaxX*rotatedAABB._MaxX);
        float minY2 = std::min(rotatedAABB._MinY*rotatedAABB._MinY, rotatedAABB._MaxY*rotatedAABB._MaxY);
        cosBound = maxZ / std::sqrt(minX2 + minY2 + maxZ2);
    } else {
        float maxX2 = std::max(rotatedAABB._MinX*rotatedAABB._MinX, rotatedAABB._MaxX*rotatedAABB._MaxX);
        float maxY2 = std::max(rotatedAABB._MinY*rotatedAABB._MinY, rotatedAABB._MaxY*rotatedAABB._MaxY);
        cosBound = maxZ / std::sqrt(maxX2 + maxY2 + maxZ2);
    }

    switch(_BRDF){
        case LAMBERT_BRDF:
            material = diffuseBound*std::max(0.f, cosBound);
            break;
        case GGX_BRDF:
            material = diffuseBound*std::max(0.f, cosBound);
            break;
        // TODO: better bound
        case DISNEY_BRDF:
            material = diffuseBound*std::max(0.f, cosBound);
            break;
        default:
            break;
    }

    return material*geometric*visibility*intensity; 
}

Vector3 RayTracer::shadeLightCuts(RayHits& hits, uint32_t depth) const {
    if(hits.getNbHits() == 0){
        return _BackgroundColor;
    }

    if(depth > _MaxBounces){
        return Color::WHITE;
    }
    
    RayHit closestHit = hits.getClosestHit();

    Vector3 color = Vector3::zeros();
            
    // init cut == root
    CutHeap rootCut = {};
    LightCutsTree::LightNodePtr root = _Scene->getLightTreeRoot();
    rootCut._Cluster = root;
    rootCut._ClusterEstimate = getClusterEstimate(closestHit, root);
    rootCut._ClusterError = getClusterError(closestHit, root);
    // setError(closestHit, root);

    std::vector<CutHeap> cutHeap = {};
    std::make_heap(cutHeap.begin(), cutHeap.end(), cutHeapComparator);
    cutHeap.push_back(rootCut);
    std::push_heap(cutHeap.begin(), cutHeap.end(), cutHeapComparator);
    
    while(getCutHeapError(cutHeap.front()) > _LightcutsErrorThreshold 
            && cutHeap.size() < _LightcutsMaxClusters
        ){
        // get worst cluster
        auto worstCut = cutHeap.front();
        std::pop_heap(cutHeap.begin(), cutHeap.end(), cutHeapComparator);
        cutHeap.pop_back();

        auto worstCluster = worstCut._Cluster;
        CutHeap leftCut{};
        CutHeap rightCut{};
        // get its two children
        leftCut._Cluster = worstCluster->_LeftChild;
        rightCut._Cluster = worstCluster->_RightChild;
        // estimate their clusters
        if(worstCluster->_IsLeftChildSame){
            leftCut._ClusterEstimate = worstCut._ClusterEstimate * (leftCut._Cluster->_TotalIntensity / worstCluster->_TotalIntensity);
            rightCut._ClusterEstimate = getClusterEstimate(closestHit, rightCut._Cluster);
        } else {
            leftCut._ClusterEstimate = getClusterEstimate(closestHit, leftCut._Cluster);
            rightCut._ClusterEstimate = worstCut._ClusterEstimate * (rightCut._Cluster->_TotalIntensity / worstCluster->_TotalIntensity);
        }
        // get their error
        leftCut._ClusterError = getClusterError(closestHit, leftCut._Cluster);
        rightCut._ClusterError = getClusterError(closestHit, rightCut._Cluster);

        // setError(closestHit, leftChild);
        // setError(closestHit, rightChild);

        // add them to heap
        cutHeap.push_back(leftCut);
        cutHeap.push_back(rightCut);
        std::push_heap(cutHeap.begin(), cutHeap.end(), cutHeapComparator);
    }
    // return sum of estimates
    // fprintf(stdout, "cut size: %zu / %zu\n", cutHeap.size(), _Scene->getPointLights().size());
    for(auto cluster : cutHeap){
        // fprintf(stdout, "cur cluster: %s\n", cluster->toString().c_str());
        color += cluster._ClusterEstimate;
        // color += cluster->_BRDF;
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
            bounceColor += _ShadingFactor * shadeLightCuts(bouncedHits, depth+1);
        } else {
            bounceColor += _BackgroundColor;
        }
    }
    color += bounceColor / _SamplesPerBounces;

    return color;
}




Vector3 RayTracer::shade(RayHits& hits, uint32_t depth) const {
    if(hits.getNbHits() == 0){
        return _BackgroundColor;
    }

    if(depth > _MaxBounces){
        return Color::WHITE;
    }
    
    RayHit closestHit = hits.getClosestHit();

    Vector3 color = Vector3::zeros();
    if(closestHit.getTriangle()._IsLight){
        color += colorBRDF(closestHit);
    } else {
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
            case GGX_BRDF:
                color += ggxBRDF(closestHit);
                break;
            case DISNEY_BRDF:
                color += disneyBRDF(closestHit);
                break;
        }
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
        fprintf(stdout, "\tThere are %zu triangles in the object `%d'\n", triangles.size(), obj);
        #endif

        auto material = GameCoordinator::getComponent<ComponentMaterial>(obj)._Material;
        auto transform = GameCoordinator::getComponent<ComponentTransform>(obj)._Transform;
        bool isLight = GameCoordinator::getComponent<ComponentLight>(obj)._IsLight;
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

            triangle._IsLight = isLight;
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



void RayTracer::run(FrameInfo frame, Vector3 backgroundColor){
    if(!_IsRunning){
        _IsRunning = true;
        uint32_t width = _Image->getWidth();
        uint32_t height = _Image->getHeight();

        _Frame = frame;
        auto camera = frame._Camera;
        Matrix4x4 viewInv = camera->getViewInverse();
        Matrix4x4 projInv = camera->getPerspectiveInverse();

        Timer timer{};
        fprintf(stdout, "Start ray tracing at `%dx%d' resolution...\n", width, height);
        fprintf(stdout, "Using OpenMP, max_threads = %d\n", omp_get_max_threads());
        timer.start();
        _BackgroundColor = backgroundColor;
        _Image->clear(_BackgroundColor);

        fprintf(stdout, "Start building BVH...\n");
        _Primitives = getTriangles();
        fprintf(stdout, "Done\n");

        fprintf(stdout, "There are %zu lights in the scene\n", 
            _Scene->getDirectionalLights().size()
            + _Scene->getPointLights().size()
        );

        if(_UseLightCuts){
            fprintf(stdout, "Start building LightTree...\n");
            _Scene->buildTree();
            fprintf(stdout, "Done\n");
        }

        float step = 1.f;
        if(_SamplesPerPixels > 1){
            step = 1.f / (_SamplesPerPixels >> 1); // more samples to make weird effects diseaper
        }


        # pragma omp parallel for
        for(uint32_t j = 0.f; j<height; j++){
            #ifndef _OPENMP
            float progress = j / (height+1.f);
            displayProgressBar(progress);
            #else
            displayProgressBarOpenMP((height+1.f));
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
                        if(_UseLightCuts){
                            color += shadeLightCuts(hits);
                        } else {
                            color += shade(hits);
                        }
                    }
                }
                
                if(nbHits > 0){
                    color /= _SamplesPerPixels;
                    _Image->set(i, j, color, Color::SRGB);
                }
            }
        }

        fprintf(stdout, "\nRay tracing executed in `%s'\n", Timer::format(timer.getTicks()).c_str());
        _IsRunning = false;
    }
}
    

}