#pragma once

#include "camera.hpp"
#include <cstdint>
#include <vulkan/vulkan.hpp>

namespace be{

/**
 * All the information contained in a frame
*/
struct FrameInfo{
    /**
     * The index of the current frame
    */
    uint32_t _FrameIndex = 0;

    /**
     * The time elasped since the last frame
    */
    float _FrameTime = 0.f;

    /**
     * The command buffer bind to this frame
    */
    VkCommandBuffer _CommandBuffer;

    /**
     * The current camera to render from
    */
    CameraPtr _Camera = nullptr;
};

};