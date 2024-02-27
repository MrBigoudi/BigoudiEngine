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
    signature.set(be::GameCoordinator::getComponentType<ComponentModel>());
    signature.set(be::GameCoordinator::getComponentType<ComponentCollider>());
    signature.set(be::GameCoordinator::getComponentType<ComponentTransform>());
    signature.set(be::GameCoordinator::getComponentType<ComponentRigidBody>());
    GameCoordinator::setSystemSignature<be::PhysicsSystem>(signature);
}

void PhysicsSystem::semiImplicitEuler(float dt, GameObject object){
    auto& transform = GameCoordinator::getComponent<ComponentTransform>(object);    
    auto rigidBody = GameCoordinator::getComponent<ComponentRigidBody>(object)._RigidBody;    
    auto collider = GameCoordinator::getComponent<ComponentCollider>(object)._Collider;

    rigidBody->computeForces();
    rigidBody->updateLinearMomentum(dt);
    rigidBody->updatePosition(transform._Transform->_Position, dt);
    if(collider){
        collider->_Transform._Position += rigidBody->vel() * dt;
    }
}

void PhysicsSystem::handleCollisions(float dt, GameObject object){  
    auto collider = GameCoordinator::getComponent<ComponentCollider>(object)._Collider;
    if(!collider) return;
    auto& transform = GameCoordinator::getComponent<ComponentTransform>(object);    
    auto rigidBody = GameCoordinator::getComponent<ComponentRigidBody>(object)._RigidBody;

    auto instance = get();
    for(auto const& curObject : instance->_Objects){
        if(object == curObject){
            // self collision
            continue;
        }        
        if(collider->isColliding(curObject)){
            auto curObjectRigidBody = GameCoordinator::getComponent<ComponentRigidBody>(curObject)._RigidBody;
            // TODO: change that
            float K = 1.f / rigidBody->mass()
                        + 1.f / curObjectRigidBody->mass();

            // get normal of separating planes
            Vector3 n = {0.f, 1.f, 0.f};

            float j = Vector3::dot(
                (rigidBody->vel() - curObjectRigidBody->vel()),
                n /  K
            );

            Vector3 J = n*j;

            // transform._Position += J;
            transform._Transform->_Position -= rigidBody->vel() * dt;
            collider->_Transform._Position -= rigidBody->vel() * dt;
        }
    }
}


void PhysicsSystem::update(float dt){
    auto instance = get();

    for(auto const& object : instance->_Objects){
        semiImplicitEuler(dt, object);
        handleCollisions(dt, object);
    }

    RigidBody::updateStepCounter();
}

};