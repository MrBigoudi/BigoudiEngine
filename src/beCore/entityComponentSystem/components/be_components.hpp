#pragma once

#include "be_componentModel.hpp"
#include "be_componentTransform.hpp"
#include "be_componentRenderSubSystem.hpp"

namespace be{

class Components{

    private:
        Components(){};

    public:
        static void registerComponents();

};

};