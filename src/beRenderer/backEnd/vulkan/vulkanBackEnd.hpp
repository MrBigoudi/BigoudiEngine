#pragma once

#include <vulkan/vulkan.h>
#include "backEnd.hpp"

namespace beRenderer{

/**
 * A class representing a vulkan backend renderer
*/
class VulkanBackEnd : public BackEnd{

    private:
        /**
         * A vulkan instance
        */
        VkInstance _VkInstance;

    private:
        /**
         * Create a vulkan instance
        */
        void createInstance();

    public:
        /**
         * Initiate the backend
        */
        virtual void init();

        /**
         * Shut down the backend
        */
        virtual void shutdown();

        /**
         * Render using the packet infos
         * @param packet The packet containing info about the rendering
         * @see frontEndTypes
        */
        virtual void render(const RenderPacket& packet);

};

}