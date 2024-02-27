#pragma once

#include <memory>
#include "be_matrix3x3.hpp"
#include "be_vector3.hpp"

namespace be{

class RigidBody;
using RigidBodyPtr = std::shared_ptr<RigidBody>;

class RigidBody{
    private:
        static uint32_t _SimulationSteps;

    private:
        bool _Movable = false;

        float _Mass = 0.f; // 0.f == unmovable objects
        
        Vector3 _LinearVelocity{};
        Vector3 _AngularVelocity{};

        Vector3 _Force{};
        Vector3 _Torque{};

        Vector3 _LinearMomentum{};
        Vector3 _AngularMomentum{};

        Matrix3x3 _InertiaBodySpace{};
        Matrix3x3 _InertiaInverseBodySpace{};
        Matrix3x3 _InertiaInverseWorldSpace{};

    public:
        RigidBody();
        RigidBody(bool movable, float mass);
        RigidBody(bool movable, float mass, const Vector3& initialForce, const Vector3& initialTorque);

    public:
        static void updateStepCounter();

    public:
        Vector3 vel() const;
        float mass() const;
        void updateLinearMomentum(float dt);
        void updateAngularMomentum(float dt);
        void updatePosition(Vector3& position, float dt);
        void updateRotation(Vector3& rotation, float dt);

        void computeForces();
        void computeTorques();

    private:
        static Vector3 getGravityForce();

        static Matrix3x3 getInertiaTensorCuboid(float height, float width, float depth, float mass);
        static Matrix3x3 getInertiaTensorCylinder(float radius, float height, float mass);
};

};