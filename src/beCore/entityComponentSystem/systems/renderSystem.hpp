#pragma once

#include "frameInfo.hpp"
#include "gameSystem.hpp"
#include "renderSubSystem.hpp"

#include <functional>

namespace be{

/**
 * A forward declaration of the render system class
*/
class RenderSystem;

/**
 * A smart pointer to the render system class
*/
using RenderSystemPtr = std::shared_ptr<RenderSystem>;

/**
 * The render system responsible to render game objects
*/
class RenderSystem : public GameSystem{

    private:
        /**
         * The unique static private instance for the singleton dp
        */
        static RenderSystemPtr _System;

    public:
        /**
         * A privte constructor to have a static class
        */
        RenderSystem(){};

    private:
        /**
         * The getter for the singleton instance
         * @return The static instance of the static class
        */
        static RenderSystemPtr get();


    public:
        /**
         * The function to render objects in the scene
         * @param frameInfo The current frame
         * @param renderSubSystem The current render sub system
         * @see FrameInfo
         * @see IRenderSubSystem
        */
        static void renderGameObjects(
            FrameInfo frameInfo, 
            IRenderSubSystem* renderSubSystem
        );
        
        /**
         * Init the render system
        */
        static void init();
};
};