#pragma once


#include "matrix4x4.hpp"
#include "projections.hpp"
#include "trigonometry.hpp"
#include "vector3.hpp"
#include <memory>

#include <cmath>

namespace be{

class Camera;
using CameraPtr = std::shared_ptr<Camera>;

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera{
    private:
        // camera Attributes
        Vector3 _Eye{};
        Vector3 _At{};
        Vector3 _WorldUp{};

        Vector3 _Up{};
        Vector3 _Right{};

        float _Fov = 0.f;
        float _AspectRatio = 0.f;
        float _Near = 0.f;
        float _Far = 0.f;

        float _MovementSpeed = 3.f;
        float _MouseSensitivity = 0.1f;

        float _Yaw = -90.f;
        float _Pitch = 0.f;

        float _Dt = 0.f;
    
    public:
        bool _Accelerate = false;

    public:
        // constructor with vectors
        Camera(
            const Vector3& position,
            float aspectRatio = 1.f,
            float fov = 50.f,
            float near = 0.1f,
            float far = 10.f,
            const Vector3& worldUp = {0.f, -1.f, 0.f}
        ){
            _AspectRatio = aspectRatio;
            _Fov = fov;
            _Near = near;
            _Far = far;
            _WorldUp = worldUp;
            _Eye = position;
            updateCameraVectors();
        }

        void setDt(float dt){_Dt = dt;}

        Vector3 getAt() const {
            return _At;
        }

        Vector3 getPosition() const {
            return _Eye;
        }

        Matrix4x4 getView() const {
            return  lookAt(_Eye, _Eye + _At, _Up);
        }
        
        Matrix4x4 getPerspective() const {
            return perspective(radians(_Fov), _AspectRatio, _Near, _Far);
        }

        virtual void processKeyboard(CameraMovement direction){
            float velocity = _MovementSpeed * _Dt;
            if(_Accelerate) velocity *= 5.f;

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

        virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true){
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

        void setAspectRatio(float aspectRatio){
            _AspectRatio = aspectRatio;
        }

    private:
        void updateCameraVectors(){
            // calculate the new at vector
            Vector3 front{};
            front.x(cos(radians(_Yaw)) * cos(radians(_Pitch)));
            front.y(sin(radians(_Pitch)));
            front.z(sin(radians(_Yaw)) * cos(radians(_Pitch)));
            _At = Vector3::normalize(front);
            // also re-calculate the Right and Up vector
            _Right = Vector3::normalize(Vector3::cross(_At, _WorldUp));
            _Up    = Vector3::normalize(Vector3::cross(_Right, _At));
        }
};

};