#include "be_renderSubSystem.hpp"

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

std::vector<VkDescriptorSet> IRenderSubSystem::getDescriptorSets() const {
    return _DescriptorSets;
}

void IRenderSubSystem::cmdBindDescriptoSets(FrameInfo& frameInfo, VkPipelineBindPoint bindPoint) const {
    // maybe rebind only one set
    vkCmdBindDescriptorSets(
        frameInfo._CommandBuffer,
        bindPoint,
        getPipelineLayout(),
        0,
        static_cast<uint32_t>(getDescriptorSets().size()),
        getDescriptorSets().data(),
        0,
        nullptr
    );
}

bool IRenderSubSystem::needPerObjectBind() const {
    return _NeedPerObjectBind;
}

void IRenderSubSystem::updateDescriptorSets(GameObject object[[maybe_unused]], FrameInfo& frameInfo[[maybe_unused]]){

}

};