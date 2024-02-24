#include "be_imgui.hpp"

namespace be{

/**
 * Check if imgui is initialized
*/
bool BeImgui::_IsInit;

/**
 * The desciptor pool
*/
VkDescriptorPool BeImgui::_ImguiPool = nullptr;

/**
 * Init imgui
 * @param window The graphics window
 * @param vulkanApp The vulkan application
 * @param renderer The renderer
 * @see Window
 * @see VulkanApp
 * @see Renderer
*/
void BeImgui::init(WindowPtr window, VulkanAppPtr vulkanApp, RendererPtr renderer){
    //1: create descriptor pool for IMGUI
	// the size of the pool is very oversize, but it's copied from imgui demo itself.
	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

    VkResult result = vkCreateDescriptorPool(vulkanApp->getDevice(), &pool_info, nullptr, &_ImguiPool);
	ErrorHandler::vulkanError(result, "Failed to create imgui descriptor pool!\n");

    // 2: initialize imgui library
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(window->getWindow(), false);
    
    // ImGui_ImplGlfw_InitForVulkan(window->getWindow(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = vulkanApp->getInstance()->getInstance();
    init_info.PhysicalDevice = vulkanApp->getPhysicalDevice();
    init_info.Device = vulkanApp->getDevice();
    init_info.QueueFamily = 0;
    init_info.Queue = vulkanApp->getGraphicsQueue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = _ImguiPool;
    init_info.Allocator = nullptr;
    init_info.RenderPass = renderer->getSwapChainRenderPass();
    init_info.MinImageCount = SwapChain::VULKAN_MAX_FRAMES_IN_FLIGHT;
    init_info.ImageCount = SwapChain::VULKAN_MAX_FRAMES_IN_FLIGHT;
    ImGui_ImplVulkan_Init(&init_info);

    _IsInit = true;
}

/**
 * Cleanup imgui
*/
void BeImgui::cleanUp(VulkanAppPtr vulkanApp){
    if(_IsInit){
        // TODO: fix cleanup
        // vkDestroyDescriptorPool(vulkanApp->getDevice(), _ImguiPool, nullptr);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

};