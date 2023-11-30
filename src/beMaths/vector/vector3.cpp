#include "vector3.hpp"

#include <beCore.hpp>
#include <cmath>

#include "errorHandler.hpp"

namespace beMaths{

/**
 * An empty constructor
*/
Vector3::Vector3()
    : Vector3(0.f,0.f,0.f){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Vector3::Vector3(float v)
    : Vector3(v,v,v){}

/**
 * A basic constructor
 * @param x The first value
 * @param y The second value
 * @param z The third value
*/
Vector3::Vector3(float x, float y, float z){
    _Values = {x,y,z};
}

/**
 * Create a matrix fill with ones
*/
Vector3 Vector3::ones(){
    return Vector3(1.f,1.f,1.f);
}

/**
 * Create a matrix fill with zeros
*/
Vector3 Vector3::zeros(){
    return Vector3(0.f,0.f,0.f);
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Vector3::toString() const {
    return "{" 
        + std::to_string(x())
        + ","
        + std::to_string(y())
        + ","
        + std::to_string(z())
        + "}"
    ; 
}

/**
 * Get the x value
 * @return x
*/
float Vector3::x() const{
    return _Values[0];
}

/**
 * Get the y value
 * @return y
*/
float Vector3::y() const{
    return _Values[1];
}

/**
 * Get the z value
 * @return z
*/
float Vector3::z() const{
    return _Values[2];
}

/**
 * Set the x value
 * @param x The new value
*/
void Vector3::x(float x){
    _Values[0] = x;
}

/**
 * Set the y value
 * @param y The new value
*/
void Vector3::y(float y){
    _Values[1] = y;
}

/**
 * Set the z value
 * @param z The new value
*/
void Vector3::z(float z){
    _Values[2] = z;
}

/**
 * Get the r value
 * @return r
*/
float Vector3::r() const{
    return x();
}

/**
 * Get the g value
 * @return g
*/
float Vector3::g() const{
    return y();
}

/**
 * Get the b value
 * @return b
*/
float Vector3::b() const{
    return z();
}

/**
 * Set the r value
 * @param r The new value
*/
void Vector3::r(float r){
    x(r);
}

/**
 * Set the g value
 * @param g The new value
*/
void Vector3::g(float g){
    y(g);
}

/**
 * Set the b value
 * @param b The new value
*/
void Vector3::b(float b){
    z(b);
}

/**
 * Getter for the matrices elements
 * @param index The index of the element to access
 * @return The wanted value
*/
float Vector3::operator[](int index) const{
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        fprintf(stderr, "Index %d out of range for vector3!\n", index);
        beCore::ErrorHandler::handle(beCore::ErrorCode::BAD_VALUE, beCore::ErrorLevel::WARNING);
    }
    return _Values[index];
}

/**
 * Setter for the matrices elements
 * @param index The index of the element to set
 * @return The wanted value
*/
float& Vector3::operator[](int index){
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        fprintf(stderr, "Index %d out of range for vector3!\n", index);
        beCore::ErrorHandler::handle(beCore::ErrorCode::BAD_VALUE, beCore::ErrorLevel::WARNING);
    }
    return _Values[index];
}

/**
 * Addition between two vectors
 * @param vector The second vertex
 * @return The sum of the two vectors
*/
Vector3 Vector3::operator+(const Vector3& vector) const{
    return Vector3(
        x() + vector.x(), 
        y() + vector.y(),
        z() + vector.z()
    );
}

/**
 * Substraction between two vectors
 * @param vector The second vertex
 * @return The substraction of the two vectors
*/
Vector3 Vector3::operator-(const Vector3& vector) const{
    return Vector3(
        x() - vector.x(), 
        y() - vector.y(),
        z() - vector.z()
    );
}

/**
 * Addition between two vectors
 * @param vector The second vector
*/
void Vector3::operator+=(const Vector3& vector){
    x(x() + vector.x());
    y(y() + vector.y());
    z(z() + vector.z());
}

/**
 * Substraction between two vectors
 * @param vector The second vector
*/
void Vector3::operator-=(const Vector3& vector){
    x(x() - vector.x());
    y(y() - vector.y());
    z(z() - vector.z());
}

/**
 * Multiplication with a scalar
 * @param val The scalar
*/
void Vector3::operator*=(float scalar){
    x(x() * scalar);
    y(y() * scalar);
    z(z() * scalar);
}

/**
 * Division with a scalar
 * @param val The scalar
*/
void Vector3::operator/=(float scalar){
    if(scalar == 0.f){
        beCore::ErrorHandler::handle(beCore::ErrorCode::ZERO_DIVIDE, beCore::ErrorLevel::WARNING);
        return;
    }
    x(x() / scalar);
    y(y() / scalar);
    z(z() / scalar);
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix3x2 Vector3::operator*(const Matrix1x2& matrix) const{
    return Matrix3x2({
        {x() * matrix[0], x() * matrix[1]},
        {y() * matrix[0], y() * matrix[1]},
        {z() * matrix[0], z() * matrix[1]}
    });
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix3x3 Vector3::operator*(const Matrix1x3& matrix) const{
    return Matrix3x3({
        {x() * matrix[0], x() * matrix[1], x() * matrix[2]},
        {y() * matrix[0], y() * matrix[1], y() * matrix[2]},
        {z() * matrix[0], z() * matrix[1], z() * matrix[2]}
    });
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix3x4 Vector3::operator*(const Matrix1x4& matrix) const{
    return Matrix3x4({
        {x() * matrix[0], x() * matrix[1], x() * matrix[2], x() * matrix[3]},
        {y() * matrix[0], y() * matrix[1], y() * matrix[2], y() * matrix[3]},
        {z() * matrix[0], z() * matrix[1], z() * matrix[2], z() * matrix[3]}
    });
}

/**
 * Dot product
 * @param v1 The first vector
 * @param v2 The second vector
 * @return The dot product
*/
float Vector3::dot(const Vector3& v1, const Vector3& v2){
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

/**
 * Dot product
 * @param vector
 * @return The dot product
*/
float Vector3::dot(const Vector3& vector) const{
    return x()*vector.x() + y()*vector.y() + z()*vector.z();
}

/**
 * Cross product
 * @param v1 The first vector
 * @param v2 The second vector
 * @return The new vector product
*/
Vector3 Vector3::cross(const Vector3& v1, const Vector3& v2){
    return Vector3(
        v1.y()*v2.z() - v1.z()*v2.y(),
        v1.z()*v2.x() - v1.x()*v2.z(),
        v1.x()*v2.y() - v1.y()*v2.x()
    )
}

/**
 * Cross product
 * @param vector
*/
void Vector3::cross(const Vector3& vector) const{
    float x = y()*v2.z() - z()*v2.y();
    float y = z()*v2.x() - x()*v2.z();
    float z = x()*v2.y() - y()*v2.x();
    x(x);
    y(y);
    z(z);
}

/**
 * Get the vector's norm
 * @return The norm
*/
float Vector3::getNorm() const{
    return std::sqrt(x()*x() + y()*y() + z()*z());
}

/**
 * Normalize the vector
 * @param vector The vector to normalize
 * @return The normalized vector
*/
Vector3 Vector3::normalize(const Vector3& vector){
    float norm = vector.getNorm();
    if(norm == 0.f){
        beCore::ErrorHandler::handle(beCore::ErrorCode::ZERO_DIVIDE);
        return Vector3();
    }
    return Vector3(x() / norm, y() / norm, z() / norm);
}

/**
* Normalize the vector
*/
void Vector3::normalize(){
    float norm = vector.getNorm();
    if(norm == 0.f){
        beCore::ErrorHandler::handle(beCore::ErrorCode::ZERO_DIVIDE);
        return Vector3();
    }
    x(x() / norm);
    y(y() / norm);
    z(z() / norm);
}

}