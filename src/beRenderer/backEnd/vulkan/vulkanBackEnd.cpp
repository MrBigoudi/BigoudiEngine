#include "vulkanBackEnd.hpp"
#include <vulkan/vulkan_core.h>

namespace beRenderer{

/**
 * Create a vulkan instance
*/
void VulkanBackEnd::createInstance(){
    VkApplicationInfo appInfo{};
    appInfo.pNext = nullptr;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello World";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
}


/**
 * Initiate the backend
*/
void VulkanBackEnd::init(){
    createInstance();
}

/**
 * Shut down the backend
*/
void VulkanBackEnd::shutdown(){

}

/**
 * Render using the packet infos
 * @param packet The packet containing info about the rendering
 * @see frontEndTypes
*/
void VulkanBackEnd::render(const RenderPacket& packet){

}

};
