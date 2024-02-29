#include "be_projections.hpp"
#include "be_errorHandler.hpp"

#include "be_vector3.hpp"

#include <cmath>

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
){
    Vector3 baseBack = Vector3::normalize(position - at);
    Vector3 baseRight = Vector3::normalize(Vector3::cross(up, baseBack));
    Vector3 baseUp = Vector3::cross(baseBack, baseRight);

    Matrix4x4 newMat = Matrix4x4::identity();
    newMat[0][0] = -baseRight.x();
    newMat[1][0] = baseRight.y();
    newMat[2][0] = baseRight.z();

    newMat[0][1] = -baseUp.x();
    newMat[1][1] = baseUp.y();
    newMat[2][1] = baseUp.z();

    newMat[0][2] = -baseBack.x();
    newMat[1][2] = baseBack.y();
    newMat[2][2] = baseBack.z();

    newMat[3][0] = Vector3::dot(-baseRight, position);
    newMat[3][1] = Vector3::dot(-baseUp, position);
    newMat[3][2] = Vector3::dot(-baseBack, position);

    return newMat;
}

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
){
    if(aspectRatio == 0.f){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Can't have a 0 aspect ratio when building a perspective matrix!\n"
        );
    }

    if(fovy == 0.f){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Can't have a 0 fov when building a perspective matrix!\n"
        );
    }

    if(near == far){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Can't have equal near and far planes when building a perspective matrix!\n"
        );
    }

    float cot = 1.f / tan(fovy / 2.f);
    Matrix4x4 newMat = Matrix4x4::zeros();
    newMat[0][0] = cot / aspectRatio;
    newMat[1][1] = cot;
    newMat[2][2] = -(far+near) / (far - near);
    newMat[2][3] = -1.f;
    newMat[3][2] = -2.f * (far * near) / (far - near);

    return newMat;
}

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
){
    if(left == right){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Can't have equal left and right planes when building a perspective matrix!\n"
        );
    }

    if(bottom == top){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Can't have equal bottom and top planes when building a perspective matrix!\n"
        );
    }

    if(near == far){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Can't have equal near and far planes when building a perspective matrix!\n"
        );
    }

    return Matrix4x4({{
        {2.f * near / (right-left), 0.f, (right+left)/(right-left), 0.f},
        {0.f, 2.f * near / (top-bottom), (top+bottom)/(top-bottom), 0.f},
        {0.f, 0.f, -(far + near)/(far-near), -2.f * far * near / (far-near)},
        {0.f, 0.f, -1.f, 0.f}
    }});
}

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
){
    if(left == right){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Can't have equal left and right planes when building an ortho matrix!\n"
        );
    }

    if(bottom == top){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Can't have equal bottom and top planes when building an ortho matrix!\n"
        );
    }

    if(near == far){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Can't have equal near and far planes when building an ortho matrix!\n"
        );
    }

    Matrix4x4 newMat = Matrix4x4::identity();
    newMat[0][0] = 2.f / (right-left);
    newMat[1][1] = 2.f / (top - bottom);
    newMat[2][2] = -1.f;
    newMat[3][0] = - (right + left) / (right - left);
	newMat[3][1] = - (top + bottom) / (top - bottom);

    return newMat;
}

};