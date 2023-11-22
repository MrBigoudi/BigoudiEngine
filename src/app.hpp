#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "window.hpp"

#include <vector>

const std::vector<const char*> BE_VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool BE_ENABLE_VALIDATION_LAYERS = false;
#else
    const bool BE_ENABLE_VALIDATION_LAYERS = true;
#endif

/**
 * A class representing the main application
*/
class App{

    private:
        /**
         * The main window of the app
        */
        WindowPtr _Window = nullptr;

        /**
         * The vulkan instance
        */
        VkInstance _VulkanInstance = nullptr;

    private:
        /**
         * Check if all validation layers are available
         * @return true if they all exist
        */
        bool checkValidationLayerSupport();

        /**
         * Create the vulkan instance
        */
        void createVulkanInstance();

        /**
         * Init the window
        */
        void initWindow();

        /**
         * Init vulkan
        */
        void initVulkan();

        /**
         * Main loop
        */
        void mainLoop();

        /**
         * Cleaning
        */
        void cleanup();

    public:
        /**
         * Run the app
        */
        void run();

};