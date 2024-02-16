#include "renderSubSystem.hpp"

namespace be{


IRenderSubSystem::IRenderSubSystem(VulkanAppPtr vulkanApp, VkRenderPass renderPass[[maybe_unused]])
    : _VulkanApp(vulkanApp){
}

void IRenderSubSystem::cleanUp(){
    cleanUpPipeline();
    cleanUpPipelineLayout();
}

PipelinePtr IRenderSubSystem::getPipeline() const {
    return _Pipeline;
}

VkPipelineLayout IRenderSubSystem::getPipelineLayout() const {
    return _PipelineLayout;
}

void IRenderSubSystem::cleanUpPipeline(){
    _Pipeline->cleanUp();
}

};