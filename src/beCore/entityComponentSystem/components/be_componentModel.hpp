#pragma once

#include "be_model.hpp"

namespace be{

/**
 * A component structure to store game objects models (=mesh)
 * @see Components
 * @see Model
*/
struct ComponentModel{
    /**
     * A smart pointer to the model
    */
    ModelPtr _Model = nullptr;
};

};