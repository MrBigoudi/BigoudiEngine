#pragma once

#include "componentModel.hpp"
#include "componentTransform.hpp"
#include "componentRenderSubSystem.hpp"

namespace be{

class Components{

    private:
        Components(){};

    public:
        static void registerComponents();

};

};