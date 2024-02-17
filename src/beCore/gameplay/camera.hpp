#pragma once


#include "matrix4x4.hpp"
#include "vector3.hpp"
#include <memory>

namespace be{

/**
 * Forward declaration of the camera class
*/
class Camera;

/**
 * Smart pointer to a camera
*/
using CameraPtr = std::shared_ptr<Camera>;

/**
 * @enum All the possible camera movements
*/
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

/**
 * A class representing a camera
*/
class Camera{
    private:
        /**
         * The camera's position
        */
        Vector3 _Eye{};

        /**
         * The camera's target
        */
        Vector3 _At{};

        /**
         * The world up vector
        */
        Vector3 _WorldUp{};

        /**
         * The camera's frame up vector
        */
        Vector3 _Up{};

        /**
         * The camera's frame right vector
        */
        Vector3 _Right{};


        /**
         * The camera's field of view
        */
        float _Fov = 0.f;

        /**
         * The camera's aspect ratio
        */
        float _AspectRatio = 0.f;

        /**
         * The camera's near plane
        */
        float _Near = 0.f;

        /**
         * The camera's far plane
        */
        float _Far = 0.f;


        /**
         * The camera's movement speed
        */
        float _MovementSpeed = 3.f;

        /**
         * The camera's mouse sensitivity for panning movement
        */
        float _MouseSensitivity = 0.1f;

        /**
         * The camera's yaw angle (in degrees)
        */
        float _Yaw = -90.f;

        /**
         * The camera's pitch angle (in degrees)
        */
        float _Pitch = 0.f;

        /**
         * The ellapsed time since last frame
        */
        float _Dt = 0.f;

    public:
        /**
         * A basic constructor
         * @param position The camera's position
         * @param aspectRatio The camera's aspect ratio
         * @param fov The camera's field of view
         * @param near The camera's near plane
         * @param far The camera's far plane
         * @param worldUp The global frame up vector
        */
        Camera(
            const Vector3& position,
            float aspectRatio = 1.f,
            float fov = 50.f,
            float near = 0.1f,
            float far = 10.f,
            const Vector3& worldUp = {0.f, -1.f, 0.f});

        /**
         * Set the camera's delta time
         * @param dt The current delta time
        */
        void setDt(float dt);

        /**
         * Getter for the target position
         * @return The target position as a vector
        */
        Vector3 getAt() const;

        /**
         * Getter for the camera position
         * @return The camera position as a vector
        */
        Vector3 getPosition() const;

        /**
         * Build the camera's view matrix
         * @return The 4x4 view matrix
        */
        Matrix4x4 getView() const;
        
        /**
         * Build the camera's perspective projection matrix
         * @return The 4x4 projection matrix
        */
        Matrix4x4 getPerspective() const;

        /**
         * Handle camera's movement
         * @param direction The direction where to move the camera
        */
        virtual void processKeyboard(CameraMovement direction);

        /**
         * Handle camera's panning
         * @param xoffset The movement in the x direction
         * @param yoffset The movement in the y direction
         * @param constrainPitch Boolean to activate or deactivate the pitch rotation constrain that can leads to weird effect
        */
        virtual void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

        /**
         * A setter for the aspect ratio
         * @param aspectRatio The new aspect ratio
        */
        void setAspectRatio(float aspectRatio);

    private:
        /**
         * Update camera's frame
        */
        void updateCameraVectors();
};

};