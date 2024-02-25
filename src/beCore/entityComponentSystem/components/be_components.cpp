#include "be_components.hpp"

#include "be_gameCoordinator.hpp"

namespace be{

void Components::registerComponents(){
    GameCoordinator::registerComponent<ComponentModel>();
    GameCoordinator::registerComponent<ComponentTransform>();
    GameCoordinator::registerComponent<ComponentRenderSubSystem>();
    GameCoordinator::registerComponent<ComponentMaterial>();
    GameCoordinator::registerComponent<ComponentCollider>();
    GameCoordinator::registerComponent<ComponentRigidBody>();
}

};