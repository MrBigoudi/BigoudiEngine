#pragma once

#include "gameSystem.hpp"
#include "frameInfo.hpp"
#include "pipeline.hpp"

namespace be{

class ECStest;
using ECStestPtr = std::shared_ptr<ECStest>;

class ECStest : public GameSystem{

    public:
        ECStest(){}

    public:
        void renderGameObjects(
            FrameInfo frameInfo, 
            PipelinePtr pipeline,
            VkPipelineLayout pipelineLayout
        );

        void cleanUpGameObjects();
};

};