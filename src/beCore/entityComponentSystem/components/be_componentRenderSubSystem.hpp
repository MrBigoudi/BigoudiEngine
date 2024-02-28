#pragma once

#include "be_gameCoordinator.hpp"
#include "be_renderSubSystem.hpp"

namespace be{

/**
 * A component structure to store game objects render sub systems
 * @see Components
 * @see IRenderSubSystem
*/
struct ComponentRenderSubSystem{
    /**
     * A smart pointer to the sub system
    */
    IRenderSubSystemPtr _RenderSubSystem = nullptr;

    static ComponentRenderSubSystem create(IRenderSubSystemPtr rss){
        return {._RenderSubSystem = rss};
    }

    static void add(GameObject object, IRenderSubSystemPtr rss){
        GameCoordinator::addComponent(
            object, 
            create(rss)
        );
    }
};

};