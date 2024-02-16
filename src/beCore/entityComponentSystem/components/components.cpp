#include "components.hpp"

#include "gameCoordinator.hpp"

namespace beCore{

void Components::registerComponents(){
    GameCoordinator::registerComponent<ComponentModel>();
    GameCoordinator::registerComponent<ComponentTransform>();
    GameCoordinator::registerComponent<ComponentRenderSubSystem>();
}

};