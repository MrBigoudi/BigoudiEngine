#pragma once

#include <memory>
#include "be_vector3.hpp"

namespace be{

class RigidBody;
using RigidBodyPtr = std::shared_ptr<RigidBody>;

class RigidBody{

    private:
        float _Mass = 0.f; // 0.f == unmovable objects
        Vector3 _Velocity{};
        Vector3 _Acceleration{};

    public:
        RigidBody(float mass):_Mass(mass){};
        RigidBody(float mass, const Vector3& velocity, const Vector3& acceleration = {})
            : _Mass(mass), _Velocity(velocity), _Acceleration(acceleration){}

    public:
        Vector3 vel() const;
        Vector3 acc() const;
        float mass() const;
        void update(float dt);

    private:
        Vector3 getGravityForce();
};

};