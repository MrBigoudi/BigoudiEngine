#include "be_camera.hpp"

#include "be_projections.hpp"
#include "be_trigonometry.hpp"

#include <cmath>

namespace be{

Camera::Camera(
    const Vector3& position,
    float width,
    float height,
    float fov,
    float near,
    float far,
    const Vector3& worldUp){
    _Width = width;
    _Height = height;
    _AspectRatio = width / height;
    _Fov = fov;
    _Near = near;
    _Far = far;
    _WorldUp = worldUp;
    _Eye = position;
    updateCameraVectors();
}

void Camera::setDt(float dt){_Dt = dt;}

Vector3 Camera::getAt() const {
    return _At;
}

Vector3 Camera::getPosition() const {
    return _Eye;
}

Matrix4x4 Camera::getView() const {
    return  lookAt(_Eye, _Eye + _At, _Up);
}

Matrix4x4 Camera::getPerspective() const {
    return perspective(radians(_Fov), _AspectRatio, _Near, _Far);
}

void Camera::processKeyboard(CameraMovement direction){
    if(!_IsLocked){
        float velocity = _MovementSpeed * _Dt;

        switch(direction){
            case FORWARD:
                _Eye += _At * velocity;
                break;
            case BACKWARD:
                _Eye -= _At * velocity;
                break;
            case LEFT:
                _Eye -= _Right * velocity;
                break;
            case RIGHT:
                _Eye += _Right * velocity;
                break;
            case UP:
                _Eye -= _WorldUp * velocity;
                break;
            case DOWN:
                _Eye += _WorldUp * velocity;
                break;
        }
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch){
    if(!_IsLocked){
        xoffset *= _MouseSensitivity;
        yoffset *= _MouseSensitivity;

        _Yaw   += xoffset;
        _Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch){
            if (_Pitch > 89.0f)
                _Pitch = 89.0f;
            if (_Pitch < -89.0f)
                _Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }
}

void Camera::setAspectRatio(float width, float height){
    _Width = width;
    _Height = height;
    if(height == 0.f){
        ErrorHandler::handle(
            __FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Can't have a null height for the camera!\n"   
        );
    }
    _AspectRatio = width / height;
}

void Camera::updateCameraVectors(){
    // calculate the new at vector
    Vector3 front{};
    front.x(cos(radians(_Yaw)) * cos(radians(_Pitch)));
    front.y(sin(radians(_Pitch)));
    front.z(sin(radians(_Yaw)) * cos(radians(_Pitch)));
    _At = Vector3::normalize(front);
    // also re-calculate the Right and Up vector
    _Right = Vector3::normalize(Vector3::cross(_WorldUp, -_At));
    _Up    = Vector3::normalize(Vector3::cross(-_At, _Right));
}

void CameraUboContainer::setView(const Matrix4x4& view){
    _UboData._View = view;
}

void CameraUboContainer::setProj(const Matrix4x4& proj){
    _UboData._Proj = proj;
}

void CameraUboContainer::update(uint32_t frameIndex){
    checkFrameIndex(frameIndex);
    _Ubos[frameIndex]->writeToBuffer(&_UboData);
}

void CameraUboContainer::init(uint32_t size, VulkanAppPtr vulkanApp){
    UboContainer::init(size, vulkanApp);
    _UboData = CameraUboData{};
    for(uint32_t i = 0; i<_Size; i++){
        _Ubos[i] = be::BufferPtr(new be::Buffer(
            vulkanApp, 
            sizeof(CameraUboData),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vulkanApp->getProperties().limits.minUniformBufferOffsetAlignment
        ));
        _Ubos[i]->map();
    }
}

Matrix4x4 Camera::getProjection(CameraProjection projectionType) const {
    Matrix4x4 proj{};
    switch (projectionType) {
        case PERSPECTIVE:
            proj = getPerspective();
            break;
        case ORTHOGRAPHIC:
            ErrorHandler::handle(
                __FILE__, __LINE__,
                ErrorCode::UNIMPLEMENTED_ERROR,
                "The orthographic projection is not yet implemented!\n"
            );
            break;
    }
    return proj;
}


Ray Camera::rayAt(float x, float y, CameraProjection projectionType[[maybe_unused]]) const{
    Vector3 direction{0.f,0.f,-1.f};
    switch(projectionType) {
        case PERSPECTIVE:{
            float w = 2.f * radians(_Fov / 2.f);
            direction = _At + ((x - 0.5f) * _AspectRatio * w) * _Right + ((1.f-y) - 0.5f) * w * _Up;
            break;
        }
        case ORTHOGRAPHIC:{
            ErrorHandler::handle(
                __FILE__, __LINE__, 
                ErrorCode::UNIMPLEMENTED_ERROR,
                "The orthographic ray at function is not implemented yet!\n"
                );
            break;
        }
    }

    direction.normalize();
    Ray ray(_Eye, direction);
    return ray;
}

};
