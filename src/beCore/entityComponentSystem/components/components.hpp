#pragma once

#include "componentModel.hpp"
#include "componentTransform.hpp"
#include "componentRenderSubSystem.hpp"

namespace beCore{

class Components{

    private:
        Components(){};

    public:
        static void registerComponents();

};

};