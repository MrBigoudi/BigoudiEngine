#include "be_collider.hpp"

#include "be_componentCollider.hpp"
#include "be_gameCoordinator.hpp"

#include "be_vector4.hpp"

#include <algorithm>

namespace be{

std::array<std::array<bool, Collider::NB_COLLIDER_LAYERS>, Collider::NB_COLLIDER_LAYERS> Collider::_ColliderLayerMatrix = [](){
    std::array<std::array<bool, Collider::NB_COLLIDER_LAYERS>, Collider::NB_COLLIDER_LAYERS> matrix;
    std::fill(matrix.begin(), matrix.end(), std::array<bool, Collider::NB_COLLIDER_LAYERS>{true});
    return matrix;
}();

bool Collider::collidingBoxVsBox(const BoxCollider* box1, const BoxCollider* box2){
    Vector4 min{-0.5f, -0.5f, -0.5f, 1.f};
    Vector4 max{0.5f, 0.5f, 0.5f, 1.f};

    Matrix4x4 box1Model = box1->_Transform.getModel();
    Vector4 box1Min = box1Model * min;
    Vector4 box1Max = box1Model * max;
    float b1Xmin = box1Min.x();
    float b1Xmax = box1Max.x();
    float b1Ymin = box1Min.y();
    float b1Ymax = box1Max.y();
    float b1Zmin = box1Min.z();
    float b1Zmax = box1Max.z();

    Matrix4x4 box2Model = box2->_Transform.getModel();
    Vector4 box2Min = box2Model * min;
    Vector4 box2Max = box2Model * max;
    float b2Xmin = box2Min.x();
    float b2Xmax = box2Max.x();
    float b2Ymin = box2Min.y();
    float b2Ymax = box2Max.y();
    float b2Zmin = box2Min.z();
    float b2Zmax = box2Max.z();

    return b1Xmin <= b2Xmax
        && b1Xmax >= b2Xmin
        && b1Ymin <= b2Ymax
        && b1Ymax >= b2Ymin
        && b1Zmin <= b2Zmax
        && b1Zmax >= b2Zmin;
}

bool BoxCollider::isColliding(GameObject object){
    auto objectCollider = GameCoordinator::getComponent<ComponentCollider>(object);
    if(!canCollide(objectCollider._Collider)){
        return false;
    }

    // Check if the pointer is of type BoxCollider
    switch(objectCollider._Collider->getType()){
        case BOX_COLLIDER:
            return collidingBoxVsBox(this, dynamic_cast<BoxCollider*>(objectCollider._Collider.get()));
        default:
            ErrorHandler::handle(
                ErrorCode::UNKNOWN_VALUE_ERROR,
                "The collider type "
                + std::to_string(objectCollider._Collider->getType())
                + " is unkown!\n"
            );
            return false;
    }
}

};