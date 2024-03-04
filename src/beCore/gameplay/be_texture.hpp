#pragma once

#include "be_image.hpp"
#include "be_ubo.hpp"
#include <cstdint>

namespace be{

struct TextureImage{
    int _TexWidth = 0;
    int _TexHeight = 0;
    int _TexChannels = 0;

    VkFormat _Format{};
    VkImage _Image = nullptr;
    VkDeviceMemory _ImageMemory = nullptr;
    VkImageView _ImageView = nullptr;
    VkSampler _ImageSampler = nullptr;

    VulkanAppPtr _VulkanApp = nullptr;

    // load and init
    static TextureImage load(VulkanAppPtr vulkanApp, const std::string& path);
    static TextureImage load(VulkanAppPtr vulkanApp, ImagePtr image);

    void cleanUp();
    VkDescriptorImageInfo getDescriptorInfo()const;

    private:
        void createTextureSampler();
        void createImageView();
        void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
        void init(VkFormat format, 
                VkImageTiling tiling, VkImageUsageFlags usage, 
                VkMemoryPropertyFlags properties
        );
};

struct TextureUboData: public UboData{
};

class TextureUboContainer: public UboContainer{
    private:
        TextureUboData _UboData{};

    public:
        virtual void init(uint32_t size, VulkanAppPtr vulkanApp) override;
        virtual void update(uint32_t frameIndex) override;
};

};