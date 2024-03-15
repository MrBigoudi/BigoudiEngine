#pragma once

#include "be_transform.hpp"
#include "be_gameCoordinator.hpp"

namespace be{

/**
 * A component structure to store game objects transforms
 * @see Components
 * @see Transform
*/
struct ComponentTransform{
    TransformPtr _Transform = TransformPtr(new Transform());

    static ComponentTransform create(
        const Vector3& position = {}, 
        const Vector3& rotation = {}, 
        const Vector3& scaling = {1.f,1.f,1.f}
    ){
        Transform transform = {};
        transform._Position = position;
        transform._Rotation = rotation;
        transform._Scale = scaling;
        return {._Transform = TransformPtr(&transform)};
    }

    static void add(GameObject object, 
        const Vector3& position = {}, 
        const Vector3& rotation = {}, 
        const Vector3& scaling = {1.f,1.f,1.f}
    ){
        GameCoordinator::addComponent(
            object, 
            create(position, rotation, scaling)
        );
    }
};

};