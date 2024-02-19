#pragma once

#include "be_gameSystem.hpp"


namespace be{

class PhysicsSystem;
using PhysicsSystemPtr = std::shared_ptr<PhysicsSystem>;

class PhysicsSystem : public GameSystem{

    public:
        PhysicsSystem(){};

    public:
        void update(float dt);

};

};