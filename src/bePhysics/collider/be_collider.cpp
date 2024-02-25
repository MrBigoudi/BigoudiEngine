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
    // Define the eight corners of each box in local space
    std::vector<Vector4> box1Corners{
        Vector4(-0.5f, -0.5f, -0.5f, 1.f),
        Vector4(0.5f, -0.5f, -0.5f, 1.f),
        Vector4(-0.5f, 0.5f, -0.5f, 1.f),
        Vector4(0.5f, 0.5f, -0.5f, 1.f),
        Vector4(-0.5f, -0.5f, 0.5f, 1.f),
        Vector4(0.5f, -0.5f, 0.5f, 1.f),
        Vector4(-0.5f, 0.5f, 0.5f, 1.f),
        Vector4(0.5f, 0.5f, 0.5f, 1.f)
    };

    std::vector<Vector4> box2Corners{
        Vector4(-0.5f, -0.5f, -0.5f, 1.f),
        Vector4(0.5f, -0.5f, -0.5f, 1.f),
        Vector4(-0.5f, 0.5f, -0.5f, 1.f),
        Vector4(0.5f, 0.5f, -0.5f, 1.f),
        Vector4(-0.5f, -0.5f, 0.5f, 1.f),
        Vector4(0.5f, -0.5f, 0.5f, 1.f),
        Vector4(-0.5f, 0.5f, 0.5f, 1.f),
        Vector4(0.5f, 0.5f, 0.5f, 1.f)
    };

    // Apply the model matrix to each corner of box1
    Matrix4x4 box1Model = box1->_Transform.getModel();
    box1Model.transpose();
    for (size_t i = 0; i < box1Corners.size(); i++) {
        box1Corners[i] = box1Model * box1Corners[i];
    }

    // Apply the model matrix to each corner of box2
    Matrix4x4 box2Model = box2->_Transform.getModel();
    box2Model.transpose();
    for (size_t i = 0; i < box2Corners.size(); i++) {
        box2Corners[i] = box2Model * box2Corners[i];
    }

    // Calculate the actual minimum and maximum values in all directions
    float b1Xmin = box1Corners[0].x();
    float b1Xmax = box1Corners[0].x();
    float b1Ymin = box1Corners[0].y();
    float b1Ymax = box1Corners[0].y();
    float b1Zmin = box1Corners[0].z();
    float b1Zmax = box1Corners[0].z();

    float b2Xmin = box2Corners[0].x();
    float b2Xmax = box2Corners[0].x();
    float b2Ymin = box2Corners[0].y();
    float b2Ymax = box2Corners[0].y();
    float b2Zmin = box2Corners[0].z();
    float b2Zmax = box2Corners[0].z();

    for (size_t i = 1; i < box1Corners.size(); i++) {
        b1Xmin = std::min(b1Xmin, box1Corners[i].x());
        b1Xmax = std::max(b1Xmax, box1Corners[i].x());
        b1Ymin = std::min(b1Ymin, box1Corners[i].y());
        b1Ymax = std::max(b1Ymax, box1Corners[i].y());
        b1Zmin = std::min(b1Zmin, box1Corners[i].z());
        b1Zmax = std::max(b1Zmax, box1Corners[i].z());
    }

    for (size_t i = 1; i < box2Corners.size(); i++) {
        b2Xmin = std::min(b2Xmin, box2Corners[i].x());
        b2Xmax = std::max(b2Xmax, box2Corners[i].x());
        b2Ymin = std::min(b2Ymin, box2Corners[i].y());
        b2Ymax = std::max(b2Ymax, box2Corners[i].y());
        b2Zmin = std::min(b2Zmin, box2Corners[i].z());
        b2Zmax = std::max(b2Zmax, box2Corners[i].z());
    }

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