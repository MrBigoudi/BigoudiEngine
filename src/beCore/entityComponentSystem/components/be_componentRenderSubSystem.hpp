#pragma once

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
};

};