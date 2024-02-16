#pragma once

#include "gameSystem.hpp"
#include "frameInfo.hpp"
#include "pipeline.hpp"

namespace beCore{

class ECSSimpleRenderSystem;
using ECSSimpleRenderSystemPtr = std::shared_ptr<ECSSimpleRenderSystem>;

class ECSSimpleRenderSystem : public GameSystem{

    public:
        ECSSimpleRenderSystem(){}

    public:
        void renderGameObjects(
            FrameInfo frameInfo, 
            PipelinePtr pipeline,
            VkPipelineLayout pipelineLayout
        );

        void cleanUpGameObjects();
};
};