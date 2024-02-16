#include "components.hpp"

#include "gameCoordinator.hpp"

namespace be{

void Components::registerComponents(){
    GameCoordinator::registerComponent<ComponentModel>();
    GameCoordinator::registerComponent<ComponentTransform>();
    GameCoordinator::registerComponent<ComponentRenderSubSystem>();
}

};