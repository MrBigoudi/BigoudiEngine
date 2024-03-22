#pragma once

#include "be_componentModel.hpp" // IWYU pragma: keep
#include "be_componentTransform.hpp" // IWYU pragma: keep
#include "be_componentRenderSubSystem.hpp" // IWYU pragma: keep
#include "be_componentMaterial.hpp" // IWYU pragma: keep
#include "be_componentCollider.hpp" // IWYU pragma: keep
#include "be_componentRigidBody.hpp" // IWYU pragma: keep
#include "be_componentLight.hpp" // IWYU pragma: keep

namespace be{

/**
 * A static class to instantiate all the components of the ECS system
*/
class Components{

    private:
        /**
         * A private constructor to make the class static
        */
        Components(){};

    public:
        /**
         * Register all the ECS components
        */
        static void registerComponents();

};

};