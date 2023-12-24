#pragma once

#include "backEnd.hpp"
#include "frontEndTypes.hpp"
#include <memory>

namespace beRenderer{

/**
 * Forward declaration for the Backend class
*/
class FrontEnd;

/**
 * A type defining a smart pointer to a backend
*/
using FrontEndPtr = std::shared_ptr<FrontEnd>;

/**
 * A class representing a front end of the renderer
*/
class FrontEnd{

    private:

    public:
        /**
         * Initiate the frontend
        */
        void init();

        /**
         * Shut down the frontend
        */
        void shutdown();

        /**
         * Render using the packet infos
         * @param packet The packet containing info about the rendering
         * @param backend The backend to use
         * @see frontEndTypes
        */
        void render(const RenderPacket& packet, const BackEndPtr& backend);
};
}