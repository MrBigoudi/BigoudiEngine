#include "be_lights.hpp"

#include "be_errorHandler.hpp"

namespace be{

void LightUboContainer::addPointLight(const Vector3& position, const Vector3& color, float intensity){
    if(_UboData._NbPointLights >= MAX_NB_POINT_LIGHTS){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::OUT_OF_RANGE_ERROR,
            "Can't add more point light in the UBO!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    PointLight newPointLight{};
    newPointLight._Color = color;
    newPointLight._Intensity = intensity;
    newPointLight._Position = position;
    _UboData._PointLights[_UboData._NbPointLights] = newPointLight;
    _UboData._NbPointLights++;
}

void LightUboContainer::addDirectionalLight(const Vector3& direction, const Vector3& color, float intensity){
    if(_UboData._NbDirectionalLights >= MAX_NB_DIRECTIONAL_LIGHTS){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::OUT_OF_RANGE_ERROR,
            "Can't add more directional light in the UBO!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    DirectionalLight newDirectionalLight{};
    newDirectionalLight._Color = color;
    newDirectionalLight._Intensity = intensity;
    newDirectionalLight._Direction = direction;
    _UboData._DirectionalLights[_UboData._NbDirectionalLights] = newDirectionalLight;
    _UboData._NbDirectionalLights++;
}

void LightUboContainer::update(uint32_t frameIndex){
    checkFrameIndex(frameIndex);
    _Ubos[frameIndex]->writeToBuffer(&_UboData);
}

void LightUboContainer::init(uint32_t size, VulkanAppPtr vulkanApp){
    UboContainer::init(size, vulkanApp);
    _UboData = LightUboData{};
    for(uint32_t i = 0; i<_Size; i++){
        _Ubos[i] = be::BufferPtr(new be::Buffer(
            vulkanApp, 
            sizeof(LightUboData),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vulkanApp->getProperties().limits.minUniformBufferOffsetAlignment
        ));
        _Ubos[i]->map();
    }
}

void LightUboContainer::reset(){
    _UboData={};
}

void LightUboContainer::addPointLight(const PointLight& pointLight){
    if(_UboData._NbPointLights >= MAX_NB_POINT_LIGHTS){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::OUT_OF_RANGE_ERROR,
            "Can't add more point light in the UBO!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    _UboData._PointLights[_UboData._NbPointLights] = pointLight;
    _UboData._NbPointLights++;
}

void LightUboContainer::addDirectionalLight(const DirectionalLight& directionalLight){
    if(_UboData._NbDirectionalLights >= MAX_NB_DIRECTIONAL_LIGHTS){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::OUT_OF_RANGE_ERROR,
            "Can't add more directional light in the UBO!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    _UboData._DirectionalLights[_UboData._NbDirectionalLights] = directionalLight;
    _UboData._NbDirectionalLights++;
}

std::vector<PointLight> LightUboContainer::getPointLights() const{
    std::vector<PointLight> lights{_UboData._NbPointLights};
    for(size_t i=0; i<_UboData._NbPointLights; i++){
        lights.push_back(_UboData._PointLights[i]);
    }
    return lights;
}

std::vector<DirectionalLight> LightUboContainer::getDirectionalLights() const{
    std::vector<DirectionalLight> lights{_UboData._NbDirectionalLights};
    for(size_t i=0; i<_UboData._NbDirectionalLights; i++){
        lights.push_back(_UboData._DirectionalLights[i]);
    }
    return lights;
}

}