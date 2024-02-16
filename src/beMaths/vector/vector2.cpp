#include "vector2.hpp"

#include "matrix.hpp" // IWYU pragma: keep
#include "vector.hpp" // IWYU pragma: keep

#include "beCore.hpp"
#include <cmath>

namespace beMaths{

/**
 * An empty constructor
*/
Vector2::Vector2()
    : Vector2(0.f,0.f){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Vector2::Vector2(float v)
    : Vector2(v,v){}

/**
 * A basic constructor
 * @param x The first value
 * @param y The second value
*/
Vector2::Vector2(float x, float y){
    _Values = {x,y};
}

/**
 * Create a matrix fill with ones
*/
Vector2 Vector2::ones(){
    return Vector2(1.f,1.f);
}

/**
 * Create a matrix fill with zeros
*/
Vector2 Vector2::zeros(){
    return Vector2(0.f,0.f);
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Vector2::toString() const {
    return "{" 
        + std::to_string(x())
        + ","
        + std::to_string(y())
        + "}"
    ; 
}

/**
 * Get the x value
 * @return x
*/
float Vector2::x() const{
    return _Values[0];
}

/**
 * Get the y value
 * @return y
*/
float Vector2::y() const{
    return _Values[1];
}

/**
 * Set the x value
 * @param x The new value
*/
void Vector2::x(float x){
    _Values[0] = x;
}

/**
 * Set the y value
 * @param y The new value
*/
void Vector2::y(float y){
    _Values[1] = y;
}

/**
 * Get the r value
 * @return r
*/
float Vector2::r() const{
    return x();
}

/**
 * Get the g value
 * @return g
*/
float Vector2::g() const{
    return y();
}

/**
 * Set the r value
 * @param r The new value
*/
void Vector2::r(float r){
    x(r);
}

/**
 * Set the g value
 * @param g The new value
*/
void Vector2::g(float g){
    y(g);
}

/**
 * Get the u value
 * @return u
*/
float Vector2::u() const{
    return x();
}

/**
 * Get the v value
 * @return v
*/
float Vector2::v() const{
    return y();
}

/**
 * Set the u value
 * @param u The new value
*/
void Vector2::u(float u){
    x(u);
}

/**
 * Set the v value
 * @param v The new value
*/
void Vector2::v(float v){
    y(v);
}

/**
 * Getter for the matrices elements
 * @param index The index of the element to access
 * @return The wanted value
*/
float Vector2::operator[](int index) const{
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        fprintf(stderr, "Index %d out of range for Vector2!\n", index);
        beCore::ErrorHandler::handle(
            beCore::ErrorCode::BAD_VALUE_ERROR, 
            "Index " + std::to_string(index) + " out of range for Vector2!\n",
            beCore::ErrorLevel::WARNING
        );
    }
    return _Values[index];
}

/**
 * Setter for the matrices elements
 * @param index The index of the element to set
 * @return The wanted value
*/
float& Vector2::operator[](int index){
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        fprintf(stderr, "Index %d out of range for Vector2!\n", index);
        beCore::ErrorHandler::handle(
            beCore::ErrorCode::BAD_VALUE_ERROR, 
            "Index " + std::to_string(index) + " out of range for Vector2!\n",
            beCore::ErrorLevel::WARNING
        );
    }
    return _Values[index];
}

/**
 * Addition between two vectors
 * @param vector The second vertex
 * @return The sum of the two vectors
*/
Vector2 Vector2::operator+(const Vector2& vector) const{
    return Vector2(x() + vector.x(), y() + vector.y());
}

/**
 * Substraction between two vectors
 * @param vector The second vertex
 * @return The substraction of the two vectors
*/
Vector2 Vector2::operator-(const Vector2& vector) const{
    return Vector2(x() - vector.x(), y() - vector.y());
}

/**
 * Addition between two vectors
 * @param vector The second vector
*/
void Vector2::operator+=(const Vector2& vector){
    x(x() + vector.x());
    y(y() + vector.y());
}

/**
 * Substraction between two vectors
 * @param vector The second vector
*/
void Vector2::operator-=(const Vector2& vector){
    x(x() - vector.x());
    y(y() - vector.y());
}

/**
 * Multiplication with a scalar
 * @param val The scalar
*/
void Vector2::operator*=(float scalar){
    x(x() * scalar);
    y(y() * scalar);
}

/**
 * Division with a scalar
 * @param val The scalar
*/
void Vector2::operator/=(float scalar){
    if(scalar == 0.f){
        beCore::ErrorHandler::handle(
            beCore::ErrorCode::ZERO_DIVIDE_ERROR, 
            "Cannot divide by 0!\n",
            beCore::ErrorLevel::WARNING
        );
        return;
    }
    x(x() / scalar);
    y(y() / scalar);
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix2x2 Vector2::operator*(const Matrix1x2& matrix) const{
    return Matrix2x2({
        std::array<float, 2>({x() * matrix[0], x() * matrix[1]}),
        std::array<float, 2>({y() * matrix[0], y() * matrix[1]})
    });
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix2x3 Vector2::operator*(const Matrix1x3& matrix) const{
    return Matrix2x3({
        std::array<float, 3>({x() * matrix[0], x() * matrix[1], x() * matrix[2]}),
        std::array<float, 3>({y() * matrix[0], y() * matrix[1], y() * matrix[2]})
    });
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix2x4 Vector2::operator*(const Matrix1x4& matrix) const{
    return Matrix2x4({
        std::array<float, 4>({x() * matrix[0], x() * matrix[1], x() * matrix[2], x() * matrix[3]}),
        std::array<float, 4>({y() * matrix[0], y() * matrix[1], y() * matrix[2], y() * matrix[3]})
    });
}

/**
 * Dot product
 * @param v1 The first vector
 * @param v2 The second vector
 * @return The dot product
*/
float Vector2::dot(const Vector2& v1, const Vector2& v2){
    return v1.x()*v2.x() + v1.y()*v2.y();
}

/**
 * Dot product
 * @param vector
 * @return The dot product
*/
float Vector2::dot(const Vector2& vector) const{
    return x()*vector.x() + y()*vector.y();
}

/**
 * Get the vector's norm
 * @return The norm
*/
float Vector2::getNorm() const{
    return std::sqrt(x()*x() + y()*y());
}

/**
 * Normalize the vector
 * @param vector The vector to normalize
 * @return The normalized vector
*/
Vector2 Vector2::normalize(const Vector2& vector){
    float norm = vector.getNorm();
    if(norm == 0.f){
        beCore::ErrorHandler::handle(beCore::ErrorCode::ZERO_DIVIDE_ERROR);
        return Vector2();
    }
    return Vector2(vector.x() / norm, vector.y() / norm);
}

/**
 * Normalize the vector
*/
void Vector2::normalize(){
    float norm = getNorm();
    if(norm == 0.f){
        beCore::ErrorHandler::handle(beCore::ErrorCode::ZERO_DIVIDE_ERROR);
        return;
    }
    x(x() / norm);
    y(y() / norm);
}

}