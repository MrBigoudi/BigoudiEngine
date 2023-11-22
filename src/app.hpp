#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "window.hpp"

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