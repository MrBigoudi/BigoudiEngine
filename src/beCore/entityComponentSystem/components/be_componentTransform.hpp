#pragma once

#include "be_transform.hpp"

namespace be{

/**
 * A component structure to store game objects transforms
 * @see Components
 * @see Transform
*/
struct ComponentTransform{
    TransformPtr _Transform = nullptr;
};

};