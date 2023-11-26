#include "app.hpp"
#include "errorHandler.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <set>
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <fstream>

/**
 * Create a window surface
*/
void App::createVulkanWindowSurface(){
    auto result = glfwCreateWindowSurface(_VulkanInstance, _Window->getWindow(), nullptr, &_WindowSurface);
    if(result != VK_SUCCESS){
        fprintf(stderr, "Failed to create a vulkan window surface!\n");
        ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
    }
}

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

SwapChainSupportDetails App::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _WindowSurface, &details.capabilities);
    
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, _WindowSurface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, _WindowSurface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, _WindowSurface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, _WindowSurface, &presentModeCount, details.presentModes.data());
    }
    return details;
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
 * Rate a device's suitability
 * @param device The device to rate
 * @return The device's score
*/
int App::rateDeviceSuitability(const VkPhysicalDevice& device){
    int score = DeviceSuitabilityScore::BAD;

    // check device properties (name, type, supported vulkan version, ...)
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
        score += DeviceSuitabilityScore::DISCRETE_GPU;
    }

    // example
    // maximum size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    // check device supported features
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    // example
    // can't run without geometry shaders
    // if(!deviceFeatures.geometryShader){
    //     score = DeviceSuitabilityScore::BAD;
    // }

    // check queue family support
    QueueFamilyIndices indices = findQueueFamilies(device);
    bool extensionsSupported = checkDeviceExtensionSupport(device);
     // check swap chain
    bool swapChainAdequate = false;
    if (extensionsSupported){
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    if(!indices.isComplete() || !extensionsSupported || !swapChainAdequate){
        score = DeviceSuitabilityScore::BAD;
    }

    return score;
}

bool App::checkDeviceExtensionSupport(VkPhysicalDevice device){
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for(const auto& extension : availableExtensions){
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


/**
 * Pick the graphics card
*/
void App::pickPhysicalDevice(){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_VulkanInstance, &deviceCount, nullptr);

    if(deviceCount == 0){
        fprintf(stderr, "Failed to find GPU with Vulkan support!\n");
        ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
    }

    // hold all the gpus
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_VulkanInstance, &deviceCount, devices.data());

    // check if one is suitable
    int maxScore = DeviceSuitabilityScore::BAD;
    for(const auto& device : devices){
        int deviceScore = rateDeviceSuitability(device);
        if(deviceScore > maxScore){
            maxScore = deviceScore;
            _VulkanPhysicalDevice = device;
        }
    }

    // none are suitable
    if(_VulkanPhysicalDevice == VK_NULL_HANDLE){
        fprintf(stderr, "Failed to find a suitable GPU!\n");
        ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
    }
}

VkSurfaceFormatKHR App::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR App::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D App::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(_Window->getWindow(), &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void App::createSwapChain(){
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(_VulkanPhysicalDevice);
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _WindowSurface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(_VulkanPhysicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    auto result = vkCreateSwapchainKHR(_VulkanLogicalDevice, &createInfo, nullptr, &_VulkanSwapChain);
    if(result != VK_SUCCESS){
        fprintf(stderr, "Failed to create the swap chain!\n");
        ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
    }

    vkGetSwapchainImagesKHR(_VulkanLogicalDevice, _VulkanSwapChain, &imageCount, nullptr);
    _VulkanSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_VulkanLogicalDevice, _VulkanSwapChain, &imageCount, _VulkanSwapChainImages.data());
    _VulkanSwapChainImageFormat = surfaceFormat.format;
    _VulkanSwapChainExtent = extent;
}

/**
 * Find the needed queue families
 * @param device The device to check
 * @return The indices of the queue family
*/
QueueFamilyIndices App::findQueueFamilies(const VkPhysicalDevice& device){
    QueueFamilyIndices indices;

    // retrieve the number of queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int index = 0;
    for(const auto& queueFamily : queueFamilies){
        // find queue family supporting graphics
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily = index;
        }
        // find queue family supporting present surface
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, index, _WindowSurface, &presentSupport);
        if(presentSupport){
            indices.presentFamily = index;
        }

        // quit if found all families
        if(indices.isComplete()){
            break;
        }
        // increase index counter
        index++;
    }

    return indices;
}

/**
 * Create the vulkan logical device
*/
void App::createVulkanLogicalDevice(){
    // number of queues we want for a queue family
    QueueFamilyIndices indices = findQueueFamilies(_VulkanPhysicalDevice);
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily.value(), 
        indices.presentFamily.value()
    };
    
    // set priority to queue
    float queuePriority = 1.0f;
    for(uint32_t queueFamily : uniqueQueueFamilies){
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        // queue with graphics capabilities
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // specify the set of device features that we'll use
    VkPhysicalDeviceFeatures deviceFeatures{};

    // create logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    // enable device extensions
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    // validation layers
    if (BE_ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(BE_VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = BE_VALIDATION_LAYERS.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    // instanciate logical device
    if (vkCreateDevice(_VulkanPhysicalDevice, &createInfo, nullptr, &_VulkanLogicalDevice) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create logical device!\n");
        ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
    }

    // retrieve graphics queue from device
    vkGetDeviceQueue(_VulkanLogicalDevice, indices.graphicsFamily.value(), 0, &_VulkanGraphicsQueue);
    vkGetDeviceQueue(_VulkanLogicalDevice, indices.presentFamily.value(), 0, &_VulkanPresentQueue);
}

void App::createImageViews(){
    _VulkanSwapChainImageViews.resize(_VulkanSwapChainImages.size());
    for(size_t i = 0; i < _VulkanSwapChainImages.size(); i++){
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _VulkanSwapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = _VulkanSwapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        auto result = vkCreateImageView(_VulkanLogicalDevice, &createInfo, nullptr, &_VulkanSwapChainImageViews[i]);
        if(result != VK_SUCCESS){
            fprintf(stderr, "Failed to create swap chain views!\n");
            ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
        }
    }
}

std::vector<char> App::readFile(const std::string& filename){
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        fprintf(stderr, "Failed to open the file %s!\n", filename.c_str());
        ErrorHandler::handle(ErrorCodes::IO_ERROR);
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.close();

    return buffer;
}

VkShaderModule App::createShaderModule(const std::vector<char>& code){
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    VkShaderModule shaderModule;
    auto result = vkCreateShaderModule(_VulkanLogicalDevice, &createInfo, nullptr, &shaderModule);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Failed to create shader module!\n");
        ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
    }
    return shaderModule;
}


void App::createGraphicsPipeline(){
    auto vertShaderCode = readFile("src/shaders/shader.vert.spv");
    auto fragShaderCode = readFile("src/shaders/shader.frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {
        vertShaderStageInfo, 
        fragShaderStageInfo
    };

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    auto result = vkCreatePipelineLayout(_VulkanLogicalDevice, &pipelineLayoutInfo, nullptr, &_VulkanPipelineLayout);

    if (result != VK_SUCCESS) {
        fprintf(stderr, "Failed to create pipeline layout!\n");
        ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;

    pipelineInfo.layout = _VulkanPipelineLayout;
    pipelineInfo.renderPass = _VulkanRenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    result = vkCreateGraphicsPipelines(_VulkanLogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_VulkanGraphicsPipeline);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Failed to create graphics pipeline!\n");
        ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
    }

    vkDestroyShaderModule(_VulkanLogicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(_VulkanLogicalDevice, vertShaderModule, nullptr);
}

void App::createRenderPass(){
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = _VulkanSwapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    auto result = vkCreateRenderPass(_VulkanLogicalDevice, &renderPassInfo, nullptr, &_VulkanRenderPass); 

    if (result != VK_SUCCESS) {
        fprintf(stderr, "Failed to create render pass!\n");
        ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
    }
}

void App::createFrameBuffers(){
    _VulkanSwapChainFramebuffers.resize(_VulkanSwapChainImageViews.size());
    for (size_t i = 0; i < _VulkanSwapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            _VulkanSwapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = _VulkanRenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = _VulkanSwapChainExtent.width;
        framebufferInfo.height = _VulkanSwapChainExtent.height;
        framebufferInfo.layers = 1;

        auto result = vkCreateFramebuffer(_VulkanLogicalDevice, &framebufferInfo, nullptr, &_VulkanSwapChainFramebuffers[i]);
        if (result != VK_SUCCESS) {
            fprintf(stderr, "Failed to create framebuffer!\n");
            ErrorHandler::handleVulkan(ErrorCodes::INIT_FAILURE);
        }
    }
}


/**
 * init vulkan
*/
void App::initVulkan(){
    createVulkanInstance();
    createVulkanWindowSurface();
    pickPhysicalDevice();
    createVulkanLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFrameBuffers();
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
    for (auto framebuffer : _VulkanSwapChainFramebuffers) {
        vkDestroyFramebuffer(_VulkanLogicalDevice, framebuffer, nullptr);
    }
    vkDestroyPipeline(_VulkanLogicalDevice, _VulkanGraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(_VulkanLogicalDevice, _VulkanPipelineLayout, nullptr);
    vkDestroyRenderPass(_VulkanLogicalDevice, _VulkanRenderPass, nullptr);
    for (auto imageView : _VulkanSwapChainImageViews) {
        vkDestroyImageView(_VulkanLogicalDevice, imageView, nullptr);
    }
    vkDestroySwapchainKHR(_VulkanLogicalDevice, _VulkanSwapChain, nullptr);
    vkDestroyDevice(_VulkanLogicalDevice, nullptr);
    vkDestroySurfaceKHR(_VulkanInstance, _WindowSurface, nullptr);
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