#pragma once

#include "gameObject.hpp"
#include "gameSystem.hpp"
#include "frameInfo.hpp"
#include "pipeline.hpp"

#include <functional>

namespace be{

class RenderSystem;
using RenderSystemPtr = std::shared_ptr<RenderSystem>;

class RenderSystem : public GameSystem{

    public:
        RenderSystem(){}

    public:
        void renderGameObjects(
            FrameInfo frameInfo, 
            PipelinePtr pipeline, 
            VkPipelineLayout pipelineLayout,
            std::vector<VkDescriptorSet> descriptorSets,
            std::function<void(GameObject)> renderingFunction// rendering function
        );

        void cleanUpGameObjects(
            std::function<void(GameObject)> cleanUpFunction
        );
};
};