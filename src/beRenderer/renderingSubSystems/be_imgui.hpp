#pragma once

#include "be_vulkanApp.hpp"
#include "be_window.hpp"
#include "be_renderer.hpp" // IWYU pragma: keep

// imgui
#include "imgui.h" // IWYU pragma: keep
#include "imgui_impl_glfw.h" // IWYU pragma: keep
#include "imgui_impl_vulkan.h" // IWYU pragma: keep

namespace be{

/**
 * A static class to handle imgui
*/
class BeImgui{
    private:
        /**
         * A private constructor to make the class static
        */
        BeImgui(){};

    private:
        /**
         * Check if imgui is initialized
        */
        static bool _IsInit;

        /**
         * The desciptor pool
        */
        static VkDescriptorPool _ImguiPool;

    public:
        /**
         * Init imgui
         * @param window The graphics window
         * @param vulkanApp The vulkan application
         * @param renderer The renderer
         * @see Window
         * @see VulkanApp
         * @see Renderer
        */
        static void init(WindowPtr window, VulkanAppPtr vulkanApp, RendererPtr renderer);

        /**
         * Cleanup imgui
         * @param vulkanApp The vulkan application
        */
        static void cleanUp(VulkanAppPtr vulkanApp);
};

};