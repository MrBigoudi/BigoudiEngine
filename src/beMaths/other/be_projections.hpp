#pragma once

#include "be_matrix4x4.hpp"
#include "be_vector3.hpp"

namespace be{

/**
 * Build a look at view matrix
 * @param position The camera's position
 * @param at The positions that the camera is looking at
 * @param up The camera's frame up vector
 * @note Be careful, "up" is not necessarily the world up vector
*/
Matrix4x4 lookAt(
    const Vector3& position, 
    const Vector3& at, 
    const Vector3& up
);

/**
 * Build a perspective projection matrix
 * @param fovy The camera's field of view in the y direction specified in radians
 * @param aspectRatio The camera's aspect ratio (width over height)
 * @param near The camera's near plane
 * @param far The camera's far plane
*/
Matrix4x4 perspective(
    float fovy,
    float aspectRatio,
    float near,
    float far
);

/**
 * Build a perpective projection matrix
 * @param left The camera's left plane
 * @param right The camera's right plane
 * @param bottom The camera's bottom plane
 * @param top The camera's top plane
 * @param near The camera's near plane
 * @param far The camera's far plane
*/
Matrix4x4 perspective(
    float left,
    float right,
    float bottom,
    float top,
    float near,
    float far
);

/**
 * Build an orthographic projection matrix
 * @param left The camera's left plane
 * @param right The camera's right plane
 * @param bottom The camera's bottom plane
 * @param top The camera's top plane
 * @param near The camera's near plane
 * @param far The camera's far plane
*/
Matrix4x4 ortho(
    float left,
    float right,
    float bottom,
    float top,
    float near,
    float far
);

};