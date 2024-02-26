#include "be_rigidBody.hpp"
#include "be_errorHandler.hpp"

namespace be{

Vector3 RigidBody::vel() const {return _LinearVelocity;}

float RigidBody::mass() const {return _Mass;}

uint32_t RigidBody::_SimulationSteps = 0;

RigidBody::RigidBody(){

}

RigidBody::RigidBody(bool movable, float mass) : _Movable(movable), _Mass(mass){
    if(mass < 0.f){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "A mass for a rigid body can't be negative!\n"
        );
    }
};

RigidBody::RigidBody(bool movable, float mass, const Vector3& initialForce, const Vector3& initialTorque)
    : _Movable(movable), _Mass(mass), _Force(initialForce), _Torque(initialTorque){
    if(mass < 0.f){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "A mass for a rigid body can't be negative!\n"
        );
    }
};

void RigidBody::updateStepCounter(){
    _SimulationSteps++;
}

Vector3 RigidBody::getGravityForce(){
    return {0.f, -9.81f, 0.f};
}

void RigidBody::computeForces(){
    if(_SimulationSteps == 0){
        _Force += getGravityForce();
    } else {
        _Force = getGravityForce();
    }
}

void RigidBody::updateLinearMomentum(float dt){
    _LinearMomentum += _Force*dt;
}

void RigidBody::updateAngularMomentum(float dt){
    _AngularMomentum += _Torque*dt;
}

void RigidBody::updatePosition(Vector3& position, float dt){
    if(!_Movable) return;
    if(_Mass < 1e-9){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "The mass of the rigid body is too low!\n"
        );
    }
    _LinearVelocity = _LinearMomentum / _Mass;
    position += _LinearMomentum / _Mass * dt;
}

void RigidBody::updateRotation(Vector3& rotation, float dt){
    // do quaternion stuff
}


// see https://en.wikipedia.org/wiki/List_of_moments_of_inertia
Matrix3x3 RigidBody::getInertiaTensorCuboid(float height, float width, float depth, float mass){
    Matrix3x3 I = Matrix3x3::zeros();
    const float factor = mass/12.f;
    const float h2 = height*height;
    const float d2 = depth*depth;
    const float w2 = width*width;
    I[0][0] = factor*(h2+d2);
    I[1][1] = factor*(w2+d2);
    I[2][2] = factor*(w2+h2);
    return I;
}

// see https://en.wikipedia.org/wiki/List_of_moments_of_inertia
Matrix3x3 RigidBody::getInertiaTensorCylinder(float radius, float height, float mass){
    Matrix3x3 I = Matrix3x3::zeros();
    const float factor = mass/12.f;
    const float h2 = height*height;
    const float r2 = radius*radius;
    I[0][0] = factor*(3*r2+h2);
    I[1][1] = factor*(3*r2+h2);
    I[2][2] = 0.5f*mass*r2;
    return I;
}

};