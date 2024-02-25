#include "be_physicsSystem.hpp"

#include "be_components.hpp" // IWYU pragma: keep
#include "be_gameCoordinator.hpp"

namespace be{

PhysicsSystemPtr PhysicsSystem::_System = nullptr;

PhysicsSystemPtr PhysicsSystem::get(){
    if(!_System){
        init();
    }
    return _System;
}

void PhysicsSystem::init(){
    if(_System != nullptr){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "The physics system has already been initialized!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    _System = GameCoordinator::registerSystem<be::PhysicsSystem>();
    if(_System == nullptr){
        ErrorHandler::handle(
            ErrorCode::NOT_INITIALIZED_ERROR,
            "Failed to initialize the ECS physics system!\n"
        );
    }
    GameObjectSignature signature;
    signature.set(be::GameCoordinator::getComponentType<ComponentCollider>());
    signature.set(be::GameCoordinator::getComponentType<ComponentTransform>());
    signature.set(be::GameCoordinator::getComponentType<ComponentRigidBody>());
    GameCoordinator::setSystemSignature<be::PhysicsSystem>(signature);
}

void PhysicsSystem::update(float dt){
    auto instance = get();

    for(auto const& object : instance->_Objects){
        auto& transform = GameCoordinator::getComponent<ComponentTransform>(object);    
        auto rigidBody = GameCoordinator::getComponent<ComponentRigidBody>(object)._RigidBody;    
        auto collider = GameCoordinator::getComponent<ComponentCollider>(object)._Collider;

        // semi implicit euler
        rigidBody->update(dt);
        transform._Position += rigidBody->vel() * dt;
        if(collider){
            collider->_Transform._Position += rigidBody->vel() * dt;
        }
    }
}

};