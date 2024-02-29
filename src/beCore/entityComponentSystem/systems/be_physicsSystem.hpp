#pragma once

#include "be_gameSystem.hpp"
#include "be_timer.hpp"


namespace be{

class PhysicsSystem;
using PhysicsSystemPtr = std::shared_ptr<PhysicsSystem>;

class PhysicsSystem : public GameSystem{
    private:
        TimerPtr _Timer = nullptr;

    private:
        static PhysicsSystemPtr _System;

    public:
        PhysicsSystem(){};

    private:
        static PhysicsSystemPtr get();

    public:
        static void init();
        static void semiImplicitEuler(float dt, GameObject object);
        static void handleCollisions(float dt, GameObject object);
        static void update(float dt);

        static void fixedUpdate(uint32_t fps);
};

};