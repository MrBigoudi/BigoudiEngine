#pragma once

#include <memory>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <vector>

#include "be_window.hpp"
#include "be_swapChain.hpp"

#include "be_errorHandler.hpp"
#include "be_vulkanApp.hpp"

namespace be{

class Renderer;
using RendererPtr = std::shared_ptr<Renderer>;

class Renderer{

    private:
        WindowPtr _Window = nullptr;
        VulkanAppPtr _VulkanApp = nullptr;
        SwapChainPtr _SwapChain = nullptr;
        std::vector<VkCommandBuffer> _CommandBuffers;
        uint32_t _CurrentImageIndex = 0;
        uint32_t _CurrentFrameIndex = 0;
        bool _IsFrameStarted = false;

    private:
        void recreateSwapChain();
        void initCommandBuffers();
        void initSwapChain();
        void cleanUpCommandBuffers();

    public:
        Renderer(WindowPtr window, VulkanAppPtr vulkanApp);
        void cleanUp();

        VkCommandBuffer beginFrame();
        void endFrame();

        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        bool isFrameInProgress() const {
            return _IsFrameStarted;
        }

        SwapChainPtr getSwapChain() const {return _SwapChain;}

        VkCommandBuffer getCurrentCommandBuffer() const {
            if(!_IsFrameStarted){
                ErrorHandler::handle(__FILE__, __LINE__, 
                    ErrorCode::NOT_INITIALIZED_ERROR, 
                    "Can't get command buffer when frame not in progress!\n"
                );
            }
            return _CommandBuffers[_CurrentFrameIndex];
        }

        VkRenderPass getSwapChainRenderPass() const {
            return _SwapChain->getRenderPass();
        }

        float getSwapChainAspectRatio() const {
            return _SwapChain->getExtentAspectRatio();
        }

        uint32_t getFrameIndex() const {
            if(!_IsFrameStarted){
                ErrorHandler::handle(__FILE__, __LINE__, 
                    ErrorCode::NOT_INITIALIZED_ERROR, 
                    "Can't get current frame index when frame not in progress!\n"
                );
            }
            return _CurrentFrameIndex;
        }

        
};

};