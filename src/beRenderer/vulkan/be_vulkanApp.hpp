# pragma once

#include <memory>
#include <vulkan/vulkan.h>
#include <cstdint>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

#include "be_vulkanInstance.hpp"
#include "be_window.hpp"

namespace be{

/**
 * Forward declaration of the vulkan app class
 * @see VulkanApp
*/
class VulkanApp;

/**
 * A smart pointer to a vulkan app
 * @see VulkanApp
*/
using VulkanAppPtr = std::shared_ptr<VulkanApp>;

/**
 * A structure containing all the graphics queue families
*/
struct VulkanQueueFamilyIndices {
    /**
     * The graphics family
    */
    std::optional<uint32_t> graphicsFamily;

    /**
     * The present family
    */
    std::optional<uint32_t> presentFamily;

    /**
     * Check if the queue family is complete
     * @return True if all the families have value
    */
    bool isComplete() {
        return 
            graphicsFamily.has_value() 
            && presentFamily.has_value();
    }
};

/**
 * A structure representing the swap chain support
*/
struct VulkanSwapChainSupportDetails {
    /**
     * The surface capabilities
    */
    VkSurfaceCapabilitiesKHR capabilities;

    /**
     * The surface format
    */
    std::vector<VkSurfaceFormatKHR> formats;

    /**
     * The surface present modes
    */
    std::vector<VkPresentModeKHR> presentModes;
};


/**
 * A vulkan application
 * @note The class wraps up instantiation of a vulkan application
*/
class VulkanApp{

    public:
        #ifdef NDEBUG
            static const bool _VulkanEnableValidationLayers = false;
        #else
            static const bool _VulkanEnableValidationLayers = true;
        #endif

        /**
         * The validation layers
        */
        static const std::vector<const char*> _VulkanValidationLayers;

        /**
         * The device extensions
        */
        static const std::vector<const char*> _VulkanDeviceExtensions;

    private:
        /**
         * A pointer to the window
         * @see Window
        */
        WindowPtr _Window = nullptr;

        /**
         * A pointer to the vulkan instance
         * @see VulkanInstance
        */
        VulkanInstancePtr _Instance = nullptr;

        /**
         * The physical device
        */
        VkPhysicalDevice _VulkanPhysicalDevice = VK_NULL_HANDLE;

        /**
         * The command pool
        */
        VkCommandPool _VulkanCommandPool;

        /**
         * The logical device
        */
        VkDevice _VulkanLogicalDevice;

        /**
         * The rendering surface
        */
        VkSurfaceKHR _VulkanSurface;

        /**
         * The graphics queue
        */
        VkQueue _VulkanGraphicsQueue;

        /**
         * The present queue
        */
        VkQueue _VulkanPresentQueue;

        /**
         * The GPU properties
        */
        VkPhysicalDeviceProperties _Properties;

        /**
         * The queue family indices
        */
        VulkanQueueFamilyIndices _QueueFamilyIndices;

        /**
         * Tells if imgui was loaded
        */
        bool _IsImguiInit = false;

    public:
        /**
         * A basic constructor
        */
        VulkanApp(){};
        
        /**
         * Init the vulkan application
         * @param window The game window
        */
        void init(WindowPtr window);

        /**
         * Clean up the vulkan application
        */
        void cleanUp();

    public:
        /**
         * A getter to the gpu properties
         * @return The properties
        */
        VkPhysicalDeviceProperties getProperties() const {return _Properties;}

        /**
         * A getter to the command pool
         * @return The command pool
        */
        VkCommandPool getCommandPool() const {return _VulkanCommandPool;}

        /**
         * A getter to the logical device
         * @return The logical device
        */
        VkDevice getDevice() const {return _VulkanLogicalDevice;}

        /**
         * A getter to the physical device
         * @return The physical device
        */
        VkPhysicalDevice getPhysicalDevice() const {return _VulkanPhysicalDevice;}

        /**
         * A getter to the surface
         * @return The surface
        */
        VkSurfaceKHR getSurface() const {return _VulkanSurface;}

        /**
         * A getter to the instance
         * @return The instance
        */
        VulkanInstancePtr getInstance() const {return _Instance;}

        /**
         * A getter to the graphics queue
         * @return The graphics queue
        */
        VkQueue getGraphicsQueue() const {return _VulkanGraphicsQueue;}

        /**
         * A getter to the present queue
         * @return The present queue
        */
        VkQueue getPresentQueue() const {return _VulkanPresentQueue;}

        /**
         * A getter to the swap chain support
         * @return The swap chain support
        */
        VulkanSwapChainSupportDetails getSwapChainSupport(){ 
            return querySwapChainSupport(_VulkanPhysicalDevice); 
        }

        VulkanQueueFamilyIndices getQueueFamilyIndices() const {
            return _QueueFamilyIndices;
        }

        /**
         * Get the memory type
         * @param typeFilter
         * @param properties
         * @return The memory type
        */
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        /**
         * Get the queue families of the GPU
         * @return The queue families indices
        */
        VulkanQueueFamilyIndices findPhysicalQueueFamilies(){ 
            return findQueueFamilies(_VulkanPhysicalDevice); 
        }

        /**
         * Get the supported formats
         * @param candidates
         * @param tiling
         * @param features
         * @return The best format
        */
        VkFormat findSupportedFormat(
            const std::vector<VkFormat> &candidates, 
            VkImageTiling tiling, 
            VkFormatFeatureFlags features
        );


        /**
         * Create a buffer
         * @param size The buffer's size
         * @param usage The buffer's usage
         * @param properties The buffer's properties
         * @param buffer A pointer to the buffer
         * @param bufferMmeory A pointer to the buffer memory
        */
        void createBuffer(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer &buffer,
            VkDeviceMemory &bufferMemory
        );

        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void copyBufferToImage(
            VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount
        );

        void createImageWithInfo(
            const VkImageCreateInfo &imageInfo,
            VkMemoryPropertyFlags properties,
            VkImage &image,
            VkDeviceMemory &imageMemory
        );

    private:
        void createInstance();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();

        int rateDevice(VkPhysicalDevice device);

        bool isDeviceSuitable(VkPhysicalDevice device);
        VulkanQueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool checkDeviceExtensionsSupport(VkPhysicalDevice device);
        VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

};

};