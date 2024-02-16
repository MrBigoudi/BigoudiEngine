#include "vector4.hpp"

#include "matrix.hpp" // IWYU pragma: keep
#include "vector.hpp" // IWYU pragma: keep

#include "errorHandler.hpp"
#include <cmath>

namespace be{

/**
 * An empty constructor
*/
Vector4::Vector4()
    : Vector4(0.f,0.f,0.f,0.f){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Vector4::Vector4(float v)
    : Vector4(v,v,v,v){}

/**
 * A basic constructor
 * @param x The first value
 * @param y The second value
 * @param z The third value
 * @param w The fourth value
*/
Vector4::Vector4(float x, float y, float z, float w){
    _Values = {x,y,z,w};
}

/**
 * Create a matrix fill with ones
*/
Vector4 Vector4::ones(){
    return Vector4(1.f,1.f,1.f,1.f);
}

/**
 * Create a matrix fill with zeros
*/
Vector4 Vector4::zeros(){
    return Vector4(0.f,0.f,0.f,0.f);
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Vector4::toString() const {
    return "{" 
        + std::to_string(x())
        + ","
        + std::to_string(y())
        + ","
        + std::to_string(z())
        + ","
        + std::to_string(w())
        + "}"
    ; 
}

/**
 * Get the x value
 * @return x
*/
float Vector4::x() const{
    return _Values[0];
}

/**
 * Get the y value
 * @return y
*/
float Vector4::y() const{
    return _Values[1];
}

/**
 * Get the z value
 * @return z
*/
float Vector4::z() const{
    return _Values[2];
}

/**
 * Get the w value
 * @return w
*/
float Vector4::w() const{
    return _Values[3];
}

/**
 * Set the x value
 * @param x The new value
*/
void Vector4::x(float x){
    _Values[0] = x;
}

/**
 * Set the y value
 * @param y The new value
*/
void Vector4::y(float y){
    _Values[1] = y;
}

/**
 * Set the z value
 * @param z The new value
*/
void Vector4::z(float z){
    _Values[2] = z;
}

/**
 * Set the w value
 * @param w The new value
*/
void Vector4::w(float w){
    _Values[3] = w;
}

/**
 * Get the r value
 * @return r
*/
float Vector4::r() const{
    return x();
}

/**
 * Get the g value
 * @return g
*/
float Vector4::g() const{
    return y();
}

/**
 * Get the b value
 * @return b
*/
float Vector4::b() const{
    return z();
}

/**
 * Get the a value
 * @return a
*/
float Vector4::a() const{
    return w();
}

/**
 * Set the r value
 * @param r The new value
*/
void Vector4::r(float r){
    x(r);
}

/**
 * Set the g value
 * @param g The new value
*/
void Vector4::g(float g){
    y(g);
}

/**
 * Set the b value
 * @param b The new value
*/
void Vector4::b(float b){
    z(b);
}

/**
 * Set the a value
 * @param a The new value
*/
void Vector4::a(float a){
    w(a);
}

/**
 * Getter for the matrices elements
 * @param index The index of the element to access
 * @return The wanted value
*/
float Vector4::operator[](int index) const{
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR, 
            "Index " + std::to_string(index) + " out of range for Vector4!\n",
            ErrorLevel::WARNING
        );
    }
    return _Values[index];
}

/**
 * Setter for the matrices elements
 * @param index The index of the element to set
 * @return The wanted value
*/
float& Vector4::operator[](int index){
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR, 
            "Index " + std::to_string(index) + " out of range for Vector4!\n",
            ErrorLevel::WARNING
        );
    }
    return _Values[index];
}

/**
 * Addition between two vectors
 * @param vector The second vertex
 * @return The sum of the two vectors
*/
Vector4 Vector4::operator+(const Vector4& vector) const{
    return Vector4(
        x() + vector.x(), 
        y() + vector.y(),
        z() + vector.z(),
        w() + vector.w()
    );
}

/**
 * Substraction between two vectors
 * @param vector The second vertex
 * @return The substraction of the two vectors
*/
Vector4 Vector4::operator-(const Vector4& vector) const{
    return Vector4(
        x() - vector.x(), 
        y() - vector.y(),
        z() - vector.z(),
        w() - vector.w()
    );
}

/**
 * Addition between two vectors
 * @param vector The second vector
*/
void Vector4::operator+=(const Vector4& vector){
    x(x() + vector.x());
    y(y() + vector.y());
    z(z() + vector.z());
    w(w() + vector.w());
}

/**
 * Substraction between two vectors
 * @param vector The second vector
*/
void Vector4::operator-=(const Vector4& vector){
    x(x() - vector.x());
    y(y() - vector.y());
    z(z() - vector.z());
    w(w() - vector.w());
}

/**
 * Multiplication with a scalar
 * @param val The scalar
*/
void Vector4::operator*=(float scalar){
    x(x() * scalar);
    y(y() * scalar);
    z(z() * scalar);
    w(w() * scalar);
}

/**
 * Division with a scalar
 * @param val The scalar
*/
void Vector4::operator/=(float scalar){
    if(scalar == 0.f){
        ErrorHandler::handle(
            ErrorCode::ZERO_DIVIDE_ERROR, 
            "Cannot divide by 0!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    x(x() / scalar);
    y(y() / scalar);
    z(z() / scalar);
    w(w() / scalar);
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix4x2 Vector4::operator*(const Matrix1x2& matrix) const{
    return Matrix4x2({
        std::array<float, 2>({x() * matrix[0], x() * matrix[1]}),
        std::array<float, 2>({y() * matrix[0], y() * matrix[1]}),
        std::array<float, 2>({z() * matrix[0], z() * matrix[1]}),
        std::array<float, 2>({w() * matrix[0], w() * matrix[1]})
    });
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix4x3 Vector4::operator*(const Matrix1x3& matrix) const{
    return Matrix4x3({
        std::array<float, 3>({x() * matrix[0], x() * matrix[1], x() * matrix[2]}),
        std::array<float, 3>({y() * matrix[0], y() * matrix[1], y() * matrix[2]}),
        std::array<float, 3>({z() * matrix[0], z() * matrix[1], z() * matrix[2]}),
        std::array<float, 3>({w() * matrix[0], w() * matrix[1], w() * matrix[2]})
    });
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix4x4 Vector4::operator*(const Matrix1x4& matrix) const{
    return Matrix4x4({
        std::array<float, 4>({x() * matrix[0], x() * matrix[1], x() * matrix[2], x() * matrix[3]}),
        std::array<float, 4>({y() * matrix[0], y() * matrix[1], y() * matrix[2], y() * matrix[3]}),
        std::array<float, 4>({z() * matrix[0], z() * matrix[1], z() * matrix[2], z() * matrix[3]}),
        std::array<float, 4>({w() * matrix[0], w() * matrix[1], w() * matrix[2], w() * matrix[3]})
    });
}

/**
 * Dot product
 * @param v1 The first vector
 * @param v2 The second vector
 * @return The dot product
*/
float Vector4::dot(const Vector4& v1, const Vector4& v2){
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z() + v1.w()*v2.w();
}

/**
 * Dot product
 * @param vector
 * @return The dot product
*/
float Vector4::dot(const Vector4& vector) const{
    return x()*vector.x() + y()*vector.y() + z()*vector.z() + w()*vector.w();
}

/**
 * Get the vector's norm
 * @return The norm
*/
float Vector4::getNorm() const{
    return std::sqrt(x()*x() + y()*y() + z()*z() + w()*w());
}

/**
 * Normalize the vector
 * @param vector The vector to normalize
 * @return The normalized vector
*/
Vector4 Vector4::normalize(const Vector4& vector){
    float norm = vector.getNorm();
    if(norm == 0.f){
        ErrorHandler::handle(ErrorCode::ZERO_DIVIDE_ERROR);
        return Vector4();
    }
    return Vector4(vector.x() / norm, vector.y() / norm, vector.z() / norm, vector.w() / norm);
}

/**
 * Normalize the vector
*/
void Vector4::normalize(){
    float norm = getNorm();
    if(norm == 0.f){
        ErrorHandler::handle(ErrorCode::ZERO_DIVIDE_ERROR);
        return;
    }
    x(x() / norm);
    y(y() / norm);
    z(z() / norm);
    w(w() / norm);
}

}