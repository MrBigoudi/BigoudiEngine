#pragma once

#include <cstdint>
#include <optional>
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

enum DeviceSuitabilityScore{
    BAD = 0,
    DISCRETE_GPU = 1000,
};

struct QueueFamilyIndices{
    // queue must supports graphics commands
    std::optional<uint32_t> graphicsFamily;

    // queue must supports presenting surface 
    std::optional<uint32_t> presentFamily;

    /**
     * Check if the family is complete
     * @return True if it is
    */
    bool isComplete(){
        return graphicsFamily.has_value()
            && presentFamily.has_value()
            ;
    }
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

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

        /**
         * The vulkan physical device (GPU)
        */
        VkPhysicalDevice _VulkanPhysicalDevice = VK_NULL_HANDLE;

        /**
         * The vulkan logical device
        */
        VkDevice _VulkanLogicalDevice = VK_NULL_HANDLE;

        /**
         * Handle to graphics queue
        */
        VkQueue _VulkanGraphicsQueue = VK_NULL_HANDLE;

        /**
         * The window surface
        */
        VkSurfaceKHR _WindowSurface;

        /**
         * Presentation queue
        */
        VkQueue _VulkanPresentQueue;

        VkSwapchainKHR _VulkanSwapChain;
        std::vector<VkImage> _VulkanSwapChainImages;
        VkFormat _VulkanSwapChainImageFormat;
        VkExtent2D _VulkanSwapChainExtent;
        std::vector<VkImageView> _VulkanSwapChainImageViews;
        VkPipelineLayout _VulkanPipelineLayout;
        VkRenderPass _VulkanRenderPass;
        VkPipeline _VulkanGraphicsPipeline;
        std::vector<VkFramebuffer> _VulkanSwapChainFramebuffers;

    private:
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void createSwapChain();
        void createImageViews();
        void createGraphicsPipeline();
        void createRenderPass();
        void createFrameBuffers();

        static std::vector<char> readFile(const std::string& filename);
        VkShaderModule createShaderModule(const std::vector<char>& code);

        /**
         * 
        */
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        /**
         * Create a window surface
        */
        void createVulkanWindowSurface();

        /**
         * Find the needed queue families
         * @param device The device to check
         * @return The indices of the queue family
        */
        QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device);

        /**
         * Rate a device's suitability
         * @param device The device to rate
         * @return The device's score
        */
        int rateDeviceSuitability(const VkPhysicalDevice& device);

        /**
         * Pick the graphics card
        */
        void pickPhysicalDevice();

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
         * Create the vulkan logical device
        */
        void createVulkanLogicalDevice();

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