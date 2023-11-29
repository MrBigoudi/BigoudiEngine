#include "vector2.hpp"

#include <beCore.hpp>

#include "matrix1x2.hpp"

namespace beMaths{

/**
 * An empty constructor
*/
Vector2f::Vector2f()
    : Vector2f(0.f,0.f){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Vector2f::Vector2f(float v)
    : Vector2f(v,v){}

/**
 * A basic constructor
 * @param x The first value
 * @param y The second value
*/
Vector2f::Vector2f(float x, float y){
    _Values = {x,y};
}

/**
 * Create a matrix fill with ones
*/
Vector2f Vector2f::ones(){
    return Vector2f(1.f,1.f);
}

/**
 * Create a matrix fill with zeros
*/
Vector2f Vector2f::zeros(){
    return Vector2f(0.f,0.f);
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Vector2f::toString() const {
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
float Vector2f::x() const{
    return _Values[0];
}

/**
 * Get the y value
 * @return y
*/
float Vector2f::y() const{
    return _Values[1];
}

/**
 * Set the x value
 * @param x The new value
*/
void Vector2f::x(float x){
    _Values[0] = x;
}

/**
 * Set the y value
 * @param y The new value
*/
void Vector2f::y(float y){
    _Values[1] = y;
}

/**
 * Get the r value
 * @return r
*/
float Vector2f::r() const{
    return x();
}

/**
 * Get the g value
 * @return g
*/
float Vector2f::g() const{
    return y();
}

/**
 * Set the r value
 * @param r The new value
*/
void Vector2f::r(float r){
    x(r);
}

/**
 * Set the g value
 * @param g The new value
*/
void Vector2f::g(float g){
    y(g);
}

/**
 * Get the u value
 * @return u
*/
float Vector2f::u() const{
    return x();
}

/**
 * Get the v value
 * @return v
*/
float Vector2f::v() const{
    return y();
}

/**
 * Set the u value
 * @param u The new value
*/
void Vector2f::u(float u){
    x(u);
}

/**
 * Set the v value
 * @param v The new value
*/
void Vector2f::v(float v){
    y(v);
}

/**
 * Getter for the matrices elements
 * @param index The index of the element to access
 * @return The wanted value
*/
float Vector2f::operator[](int index) const{
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        fprintf(stderr, "Index %d out of range for matrix1x2!\n", index);
        beCore::ErrorHandler::handle(beCore::ErrorCode::BAD_VALUE, beCore::ErrorLevel::WARNING);
    }
    return _Values[index];
}

/**
 * Setter for the matrices elements
 * @param index The index of the element to set
 * @return The wanted value
*/
float& Vector2f::operator[](int index){
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        fprintf(stderr, "Index %d out of range for matrix1x2!\n", index);
        beCore::ErrorHandler::handle(beCore::ErrorCode::BAD_VALUE, beCore::ErrorLevel::WARNING);
    }
    return _Values[index];
}

/**
 * Addition between two vectors
 * @param vector The second vertex
 * @return The sum of the two vectors
*/
Vector2f Vector2f::operator+(const Vector2f& vector) const{
    return Vector2f(x() + vector.x(), y() + vector.y());
}

/**
 * Substraction between two vectors
 * @param vector The second vertex
 * @return The substraction of the two vectors
*/
Vector2f Vector2f::operator-(const Vector2f& vector) const{
    return Vector2f(x() - vector.x(), y() - vector.y());
}

/**
 * Addition between two vectors
 * @param vector The second vector
*/
void Vector2f::operator+=(const Vector2f& vector){
    x(x() + vector.x());
    y(y() + vector.y());
}

/**
 * Substraction between two vectors
 * @param vector The second vector
*/
void Vector2f::operator-=(const Vector2f& vector){
    x(x() - vector.x());
    y(y() - vector.y());
}

/**
 * Multiplication with a scalar
 * @param val The scalar
*/
void Vector2f::operator*=(float scalar){
    x(x() * scalar);
    y(y() * scalar);
}

/**
 * Division with a scalar
 * @param val The scalar
*/
void Vector2f::operator/=(float scalar){
    if(scalar == 0.f){
        beCore::ErrorHandler::handle(beCore::ErrorCode::ZERO_DIVIDE, beCore::ErrorLevel::WARNING);
        return;
    }
    x(x() / scalar);
    y(y() / scalar);
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix2x2f Vector2f::operator*(const Matrix1x2f& matrix) const{
    return Matrix2x2f({
        {x() * matrix[0], x() * matrix[1]},
        {y() * matrix[0], y() * matrix[1]}
    });
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix2x3f Vector2f::operator*(const Matrix1x3f& matrix) const{
    return Matrix2x3f({
        {x() * matrix[0], x() * matrix[1], x() * matrix[2]},
        {y() * matrix[0], y() * matrix[1], y() * matrix[2]}
    });
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix2x4f Vector2f::operator*(const Matrix1x4f& matrix) const{
    return Matrix2x4f({
        {x() * matrix[0], x() * matrix[1], x() * matrix[2], x() * matrix[3]},
        {y() * matrix[0], y() * matrix[1], y() * matrix[2], y() * matrix[3]}
    });
}

}

namespace beMaths{

/**
 * An empty constructor
*/
Vector2i::Vector2i()
    : Vector2i(0.f,0.f){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Vector2i::Vector2i(int v)
    : Vector2i(v,v){}

/**
 * A basic constructor
 * @param x The first value
 * @param y The second value
*/
Vector2i::Vector2i(int x, int y){
    _Values = {x,y};
}

/**
 * Create a matrix fill with ones
*/
Vector2i Vector2i::ones(){
    return Vector2i(1.f,1.f);
}

/**
 * Create a matrix fill with zeros
*/
Vector2i Vector2i::zeros(){
    return Vector2i(0.f,0.f);
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Vector2i::toString() const {
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
int Vector2i::x() const{
    return _Values[0];
}

/**
 * Get the y value
 * @return y
*/
int Vector2i::y() const{
    return _Values[1];
}

/**
 * Set the x value
 * @param x The new value
*/
void Vector2i::x(int x){
    _Values[0] = x;
}

/**
 * Set the y value
 * @param y The new value
*/
void Vector2i::y(int y){
    _Values[1] = y;
}

/**
 * Get the r value
 * @return r
*/
int Vector2i::r() const{
    return x();
}

/**
 * Get the g value
 * @return g
*/
int Vector2i::g() const{
    return y();
}

/**
 * Set the r value
 * @param r The new value
*/
void Vector2i::r(int r){
    x(r);
}

/**
 * Set the g value
 * @param g The new value
*/
void Vector2i::g(int g){
    y(g);
}

/**
 * Get the u value
 * @return u
*/
int Vector2i::u() const{
    return x();
}

/**
 * Get the v value
 * @return v
*/
int Vector2i::v() const{
    return y();
}

/**
 * Set the u value
 * @param u The new value
*/
void Vector2i::u(int u){
    x(u);
}

/**
 * Set the v value
 * @param v The new value
*/
void Vector2i::v(int v){
    y(v);
}

/**
 * Getter for the matrices elements
 * @param index The index of the element to access
 * @return The wanted value
*/
int Vector2i::operator[](int index) const{
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        fprintf(stderr, "Index %d out of range for matrix1x2!\n", index);
        beCore::ErrorHandler::handle(beCore::ErrorCode::BAD_VALUE, beCore::ErrorLevel::WARNING);
    }
    return _Values[index];
}

/**
 * Setter for the matrices elements
 * @param index The index of the element to set
 * @return The wanted value
*/
int& Vector2i::operator[](int index){
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        fprintf(stderr, "Index %d out of range for matrix1x2!\n", index);
        beCore::ErrorHandler::handle(beCore::ErrorCode::BAD_VALUE, beCore::ErrorLevel::WARNING);
    }
    return _Values[index];
}

/**
 * Addition between two vectors
 * @param vector The second vertex
 * @return The sum of the two vectors
*/
Vector2i Vector2i::operator+(const Vector2i& vector) const{
    return Vector2i(x() + vector.x(), y() + vector.y());
}

/**
 * Substraction between two vectors
 * @param vector The second vertex
 * @return The substraction of the two vectors
*/
Vector2i Vector2i::operator-(const Vector2i& vector) const{
    return Vector2i(x() - vector.x(), y() - vector.y());
}

/**
 * Addition between two vectors
 * @param vector The second vector
*/
void Vector2i::operator+=(const Vector2i& vector){
    x(x() + vector.x());
    y(y() + vector.y());
}

/**
 * Substraction between two vectors
 * @param vector The second vector
*/
void Vector2i::operator-=(const Vector2i& vector){
    x(x() - vector.x());
    y(y() - vector.y());
}

/**
 * Multiplication with a scalar
 * @param val The scalar
*/
void Vector2i::operator*=(float scalar){
    x(x() * scalar);
    y(y() * scalar);
}

/**
 * Division with a scalar
 * @param val The scalar
*/
void Vector2i::operator/=(float scalar){
    if(scalar == 0){
        beCore::ErrorHandler::handle(beCore::ErrorCode::ZERO_DIVIDE, beCore::ErrorLevel::WARNING);
        return;
    }
    x(x() / scalar);
    y(y() / scalar);
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix2x2i Vector2i::operator*(const Matrix1x2i& matrix) const{
    return Matrix2x2i({
        {x() * matrix[0], x() * matrix[1]},
        {y() * matrix[0], y() * matrix[1]}
    });
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix2x3i Vector2i::operator*(const Matrix1x3i& matrix) const{
    return Matrix2x3f({
        {x() * matrix[0], x() * matrix[1], x() * matrix[2]},
        {y() * matrix[0], y() * matrix[1], y() * matrix[2]}
    });
}

/**
 * Multiplication with a matrix
 * @param matrix The matrix
*/
Matrix2x4i Vector2i::operator*(const Matrix1x4i& matrix) const{
    return Matrix2x4f({
        {x() * matrix[0], x() * matrix[1], x() * matrix[2], x() * matrix[3]},
        {y() * matrix[0], y() * matrix[1], y() * matrix[2], y() * matrix[3]}
    });
}

}