#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_ONE
#include <glm/glm.hpp>

#include <memory>
#include "renderSubSystem.hpp"
#include "buffer.hpp"
#include "descriptors.hpp"
#include "ecsSimpleRenderSystem.hpp"
#include "swapChain.hpp"

namespace beCore{

struct SimplePushConstantData{
    alignas(4) float _Random;
    alignas(16) glm::mat4 _Model;
};

struct CameraUbo{
    glm::mat4 _View{1.f};
    glm::mat4 _Proj{1.f};
};

struct LightUbo{
    glm::vec4 _LightDir = glm::vec4(
        glm::normalize(
            glm::vec3(1.f, -3.f, -1.f)
        ), 
        1.f
    );
};

class SimpleRenderSubSystem;
using SimpleRenderSubSystemPtr = std::shared_ptr<SimpleRenderSubSystem>;

class SimpleRenderSubSystem : public IRenderSubSystem {
    private:
        ECSSimpleRenderSystemPtr _ECSRenderSystem = nullptr;

        std::vector<BufferPtr> _CameraUBO{SwapChain::VULKAN_MAX_FRAMES_IN_FLIGHT};
        std::vector<BufferPtr> _LightUBO{SwapChain::VULKAN_MAX_FRAMES_IN_FLIGHT};

        DescriptorPoolPtr _GlobalPool = nullptr;

        std::vector<VkDescriptorSet> _GlobalDescriptorSets{SwapChain::VULKAN_MAX_FRAMES_IN_FLIGHT};
        std::vector<VkDescriptorSet> _LightDescriptorSets{SwapChain::VULKAN_MAX_FRAMES_IN_FLIGHT};
        DescriptorSetLayoutPtr _GlobalSetLayout = nullptr;
        DescriptorSetLayoutPtr _LightSetLayout = nullptr;

    public:
        SimpleRenderSubSystem(VulkanAppPtr vulkanApp, VkRenderPass renderPass, DescriptorPoolPtr globalPool);

        void renderGameObjects(FrameInfo& frameInfo) override;

        void cleanUp() override;


    private:
        void initECSRender();
        void initPipelineLayout() override;
        void initPipeline(VkRenderPass renderPass) override;
        void cleanUpPipelineLayout() override;
        void initUBOs();
        void initDescriptors();
};

};