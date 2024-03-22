#pragma once

#include "be_gameCoordinator.hpp"

namespace be{

/**
 * A component structure to store game objects light flag
 * @see Components
*/
struct ComponentLight{
    bool _IsLight = false;

    static void add(GameObject object, bool isLight = false){
        ComponentLight component{};
        component._IsLight = isLight;

        GameCoordinator::addComponent(
            object, 
            component
        );
    }
};

};