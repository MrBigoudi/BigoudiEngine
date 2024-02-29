#include "be_physicsSystem.hpp"

#include "be_components.hpp" // IWYU pragma: keep
#include "be_gameCoordinator.hpp"
#include <chrono>
#include <thread>

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
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "The physics system has already been initialized!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    _System = GameCoordinator::registerSystem<be::PhysicsSystem>();
    if(_System == nullptr){
        ErrorHandler::handle(__FILE__, __LINE__, 
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

    // init the timer
    get()->_Timer = TimerPtr(new Timer());
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
    auto rigidBody = GameCoordinator::getComponent<ComponentRigidBody>(object)._RigidBody;
    if(!rigidBody->isMovable()) return;
    fprintf(stdout, "cur obj: %d\n", object);

    auto instance = get();
    for(auto const& curObject : instance->_Objects){
        if(object == curObject){
            // self collision
            continue;
        }        
        if(collider->isColliding(curObject)){
            auto curObjectRigidBody = GameCoordinator::getComponent<ComponentRigidBody>(curObject)._RigidBody;
            
            // TODO: change normal, change contact point
            Vector3 contactPoint = {};
            Vector3 contactNormal = {0.f, 1.f, 0.f};
            auto J = rigidBody->getImpulse(curObjectRigidBody, contactNormal, contactPoint);
            rigidBody->updateForceImpulse(J);
            // rigidBody->updateTorqueImpulse(J, contactPoint);
        }
    }
}

void PhysicsSystem::fixedUpdate(uint32_t fps){
    uint32_t ticksPerFrame = 1000 / fps;
    float dt = ticksPerFrame / 1000.f;
    get()->_Timer->start();
    update(dt);
    uint32_t frameTicks = get()->_Timer->getTicks();
    if(frameTicks < ticksPerFrame){
        Timer::sleep(ticksPerFrame-frameTicks);
    }
    get()->_Timer->stop();
}

void PhysicsSystem::update(float dt){
    // fprintf(stdout, "Variable update fps: %f\n", 1.f / dt);
    auto instance = get();

    for(auto const& object : instance->_Objects){
        semiImplicitEuler(dt, object);
        handleCollisions(dt, object);
    }

    RigidBody::updateStepCounter();
}

};