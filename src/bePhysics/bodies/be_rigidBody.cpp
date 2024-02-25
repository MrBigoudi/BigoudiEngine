#include "be_rigidBody.hpp"

namespace be{

Vector3 RigidBody::vel() const {return _Velocity;}

Vector3 RigidBody::acc() const {return _Acceleration;}

Vector3 RigidBody::getGravityForce(){
    return {0.f, -9.81f, 0.f};
}

void RigidBody::update(float dt){
    if(_Mass == 0.f) return; // mass == 0.f => static object
    _Acceleration = getGravityForce() / _Mass;
    _Velocity += _Acceleration * dt;
}

};