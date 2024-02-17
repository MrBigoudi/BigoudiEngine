#include "renderSystem.hpp"

namespace be{

void RenderSystem::renderGameObjects(
    FrameInfo frameInfo,
    PipelinePtr pipeline, 
    VkPipelineLayout pipelineLayout,
    std::vector<VkDescriptorSet> descriptorSets,
    std::function<void(GameObject)> renderingFunction
    ){
    pipeline->bind(frameInfo._CommandBuffer);

    vkCmdBindDescriptorSets(
        frameInfo._CommandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipelineLayout,
        0,
        static_cast<uint32_t>(descriptorSets.size()),
        descriptorSets.data(),
        0,
        nullptr
    );

    for(auto const& object : _Objects){
        renderingFunction(object);
    }
        
}

void RenderSystem::cleanUpGameObjects(
    std::function<void(GameObject)> cleanUpFunction
    ){
    for(auto object : _Objects){
        cleanUpFunction(object);
    }
}

};