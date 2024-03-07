#include "be_rigidBody.hpp"
#include "be_errorHandler.hpp"
#include "be_physicsConstants.hpp"
#include "be_transform.hpp"

namespace be{

Vector3 RigidBody::vel() const {return _LinearVelocity;}

float RigidBody::mass() const {return _Mass;}

uint32_t RigidBody::_SimulationSteps = 0;

RigidBody::RigidBody(TransformPtr transform, ModelPtr model)
    : _Transform(transform), _Model(model){
    _Rotation = transform->getRotationMatrix();
    initInertiaTensors();
}

bool RigidBody::isMovable() const { return _Movable;}

RigidBody::RigidBody(TransformPtr transform, ModelPtr model, bool movable, float mass) {
    _Movable = movable; 
    _Mass = mass; 
    if(mass < EPSILON){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "A mass for a rigid body can't be too low!\n"
        );
    }
    RigidBody(transform, model);
};

RigidBody::RigidBody(TransformPtr transform, ModelPtr model, bool movable, float mass, const Vector3& initialForce, const Vector3& initialTorque){
    _Movable = movable; 
    _Mass = mass; 
    _Force = initialForce;
    _Torque = initialTorque;
    if(mass < EPSILON){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "A mass for a rigid body can't be too low!\n"
        );
    }
    RigidBody(transform, model);
};

void RigidBody::initInertiaTensors(){
    // TODO: different for different shapes
    bool isCube = true;
    if(isCube){
        // TODO: need to update models
        _InertiaBodySpace = getInertiaTensorCuboid(1.f, 1.f, 1.f, _Mass);
        _InertiaInverseBodySpace = Matrix3x3::inverse(_InertiaBodySpace);
        _InertiaInverseWorldSpace = _Rotation * _InertiaInverseBodySpace * Matrix3x3::transpose(_Rotation);
    }
}


void RigidBody::updateStepCounter(){
    _SimulationSteps++;
}

Vector3 RigidBody::getGravityForce(){
    return {0.f, -GRAVITY, 0.f};
}

void RigidBody::computeTorques(){
    if(_SimulationSteps != 0){
        _Torque = {};
    }
}

void RigidBody::computeForces(){
    if(_SimulationSteps == 0){
        _Force += getGravityForce();
    } else {
        _Force = getGravityForce();
    }
    // fprintf(stdout, "step %d, force = %s\n", _SimulationSteps, _Force.toString().c_str());
}

void RigidBody::updateLinearMomentum(float dt){
    _LinearMomentum += _Force*dt;
    // fprintf(stdout, "step %d, linear momentum = %s\n", _SimulationSteps, _LinearMomentum.toString().c_str());
}

void RigidBody::updateAngularMomentum(float dt){
    _AngularMomentum += _Torque*dt;
}

void RigidBody::updatePosition(Vector3& position, float dt){
    if(!_Movable) return;
    _LinearVelocity = _LinearMomentum / _Mass;
    position += _LinearMomentum / _Mass * dt;
}

void RigidBody::updateRotation(Vector3& rotation[[maybe_unused]], float dt[[maybe_unused]]){
    // do quaternion stuff
    if(!_Movable) return;
    // Vector3 omega = _InertiaInverseWorldSpace * _AngularMomentum;
    // _Rotation += dt * Matrix3x3::getCrossProductMatrix(omega) * _Rotation; // TODO: implement that
}

Vector3 RigidBody::getImpulse(const RigidBodyPtr r, const Vector3& n, const Vector3& contactPoint[[maybe_unused]]) const {
    float K = 1.f / _Mass + 1.f / r->_Mass;

    // TODO: velocity at the contact point
    float j = Vector3::dot((r->_LinearVelocity - _LinearVelocity), n / K);

    Vector3 J = n * j;

    return J;
}

void RigidBody::updateForceImpulse(const Vector3& J){
    // fprintf(stdout, "linear momentum before: %s\n", _LinearMomentum.toString().c_str());
    _LinearMomentum += J;
    // fprintf(stdout, "linear momentum after: %s\n", _LinearMomentum.toString().c_str());
}

void RigidBody::updateTorqueImpulse(const Vector3& J, const Vector3& contactPoint){
    Vector3 diff = contactPoint - _CenterOfMass;
    _AngularMomentum += Vector3::cross(diff, J);
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