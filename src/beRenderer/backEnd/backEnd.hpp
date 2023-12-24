#pragma once

#include "frontEndTypes.hpp"

#include <memory>

namespace beRenderer{

/**
 * Forward declaration for the Backend class
*/
class BackEnd;

/**
 * A type defining a smart pointer to a backend
*/
using BackEndPtr = std::shared_ptr<BackEnd>;

/**
 * A class representing a backend api abstraction for the renderer
*/
class BackEnd{

    private:

    public:
        /**
         * Initiate the backend
        */
        virtual void init() = 0;

        /**
         * Shut down the backend
        */
        virtual void shutdown() = 0;

        /**
         * Render using the packet infos
         * @param packet The packet containing info about the rendering
         * @see frontEndTypes
        */
        virtual void render(const RenderPacket& packet) = 0;
};

}