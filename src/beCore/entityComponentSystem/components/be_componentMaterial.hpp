#pragma once

#include "be_material.hpp"

namespace be{

/**
 * A component structure to store game objects material
 * @see Material
 * @see Components
*/
struct ComponentMaterial{
    MaterialPtr _Material = nullptr;
};

}