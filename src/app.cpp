#include "app.hpp"
#include "errorHandler.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstring>
#include <vulkan/vulkan_core.h>

/**
 * Check if all validation layers are available
 * @return true if they all exist
*/
bool App::checkValidationLayerSupport(){
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : BE_VALIDATION_LAYERS) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

/**
 * Init the window
*/
void App::initWindow(){
    int error = glfwInit();
    if(error != GLFW_TRUE){
        fprintf(stderr, "Failed to init GLFW!\n");
        ErrorHandler::handleGLFW(ErrorCodes::INIT_FAILURE);
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _Window = WindowPtr(new Window(800, 600, "Vulkan"));
}

/**
 * Create the vulkan instance
*/
void App::createVulkanInstance(){
    if(BE_ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()){
        fprintf(stderr, "Validation layers unavailable!\n");
        ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
    }

    // init appinfo
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = PROJECT_NAME;
    appInfo.applicationVersion = VK_MAKE_VERSION(PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // init createinfo
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;
    if(BE_ENABLE_VALIDATION_LAYERS){
        createInfo.enabledLayerCount = static_cast<uint32_t>(BE_VALIDATION_LAYERS.size());
    }

    // init vulkan instance
    VkResult result = vkCreateInstance(&createInfo, nullptr, &_VulkanInstance);
    if(result != VK_SUCCESS){
        fprintf(stderr, "Failed to create vulkan instance!\n");
        ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
    }
}

/**
 * init vulkan
*/
void App::initVulkan(){
    createVulkanInstance();
}

/**
 * main loop
*/
void App::mainLoop(){
    while(!glfwWindowShouldClose(_Window->getWindow())){
        glfwPollEvents();
    }
}

/**
 * cleaning
*/
void App::cleanup(){
    vkDestroyInstance(_VulkanInstance, nullptr);
    glfwTerminate();
}

/**
 * Run the app
*/
void App::run(){
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}