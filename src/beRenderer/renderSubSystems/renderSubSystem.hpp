#pragma once

#include "frameInfo.hpp"
#include "pipeline.hpp"
#include "vulkanApp.hpp"

namespace beCore{

class IRenderSubSystem;
using IRenderSubSystemPtr = std::shared_ptr<IRenderSubSystem>;

class IRenderSubSystem{

    protected:
        VulkanAppPtr _VulkanApp = nullptr;
        PipelinePtr _Pipeline = nullptr;
        VkPipelineLayout _PipelineLayout;

    public:
        IRenderSubSystem(VulkanAppPtr vulkanApp, VkRenderPass renderPass[[maybe_unused]]);

        virtual ~IRenderSubSystem() = default;
        virtual void renderGameObjects(FrameInfo& frameInfo) = 0;

        virtual void cleanUp();

    public:
        virtual PipelinePtr getPipeline() const;

        virtual VkPipelineLayout getPipelineLayout() const;

    protected:
        virtual void initPipelineLayout() = 0;
        virtual void initPipeline(VkRenderPass renderPass) = 0;
        virtual void cleanUpPipelineLayout() = 0;
        virtual void cleanUpPipeline();

};

};