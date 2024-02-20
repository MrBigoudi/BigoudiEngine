#pragma once

#include "be_components.hpp" // IWYU pragma: keep

#include "be_frameInfo.hpp"
#include "be_gameSystem.hpp"
#include "be_renderSubSystem.hpp"

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

        /**
         * Create a renderable object
         * @param model The model of the new object
         * @param transform The transform component of the new object
         * @param material The material component of the new object
         * @param renderSubSystem The render sub system of the new object
         * @return The id representing the newly created object
         * @see GameObject
        */
        static GameObject createRenderableObject(
            ComponentModel model,
            ComponentTransform transform,
            ComponentMaterial material,
            ComponentRenderSubSystem renderSubSystem
        );
};
};