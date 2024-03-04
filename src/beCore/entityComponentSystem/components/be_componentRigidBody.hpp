#pragma once

#include "be_rigidBody.hpp"
#include "be_gameCoordinator.hpp"

namespace be{

struct ComponentRigidBody{

    RigidBodyPtr _RigidBody = nullptr;

    static ComponentRigidBody create(RigidBodyPtr rigidBody){
        return {._RigidBody = be::RigidBodyPtr(rigidBody)};
    }

    static ComponentRigidBody create(TransformPtr transform, ModelPtr model){
        return {._RigidBody = be::RigidBodyPtr(
            new be::RigidBody(transform, model)
        )};
    }

    static ComponentRigidBody create(TransformPtr transform, ModelPtr model,
        bool movable, float mass){
        return {._RigidBody = be::RigidBodyPtr(
            new be::RigidBody(transform, model, movable, mass)
        )};
    }

    static ComponentRigidBody create(TransformPtr transform, ModelPtr model,
        bool movable, float mass, const Vector3& initialForce, const Vector3& initialTorque){
        return {._RigidBody = be::RigidBodyPtr(
            new be::RigidBody(transform, model, movable, mass, initialForce, initialTorque)
        )};
    }

    static void add(GameObject object, be::RigidBodyPtr rigidBody){
        GameCoordinator::addComponent(
            object, 
            create(rigidBody)
        );
    }

    static void add(GameObject object, TransformPtr transform, ModelPtr model){
        GameCoordinator::addComponent(
            object, 
            create(transform, model)
        );
    }

    static void add(GameObject object, TransformPtr transform, ModelPtr model,
        bool movable, float mass){
        GameCoordinator::addComponent(
            object, 
            create(transform, model, movable, mass)
        );
    }

    static void add(GameObject object, TransformPtr transform, ModelPtr model,
        bool movable, float mass, const Vector3& initialForce, const Vector3& initialTorque){
        GameCoordinator::addComponent(
            object, 
            create(transform, model, movable, mass, initialForce, initialTorque)
        );
    }

};

}