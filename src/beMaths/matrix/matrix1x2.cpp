#include "matrix1x2.hpp"
#include "errorHandler.hpp"

#include "matrix1x3.hpp"
#include "matrix1x4.hpp"

#include <beCore.hpp>

namespace beMaths{

/**
 * An empty constructor
*/
Matrix1x2f::Matrix1x2f()
    : Matrix1x2f(0.f,0.f){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Matrix1x2f::Matrix1x2f(float v)
    : Matrix1x2f(v,v){}

/**
 * A basic constructor
 * @param x The first value
 * @param y The second value
*/
Matrix1x2f::Matrix1x2f(float x, float y){
    _Mat = {x,y};
}

/**
 * Create a matrix fill with ones
*/
Matrix1x2f Matrix1x2f::ones(){
    return Matrix1x2f(1.f,1.f);
}

/**
 * Create a matrix fill with zeros
*/
Matrix1x2f Matrix1x2f::zeros(){
    return Matrix1x2f(0.f,0.f);
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Matrix1x2f::toString() const {
    return "{" 
        + std::to_string(x())
        + ","
        + std::to_string(x())
        + "}"
    ; 
}

/**
 * Get the x value
 * @return x
*/
float Matrix1x2f::x() const{
    return _Mat[0];
}

/**
 * Get the y value
 * @return y
*/
float Matrix1x2f::y() const{
    return _Mat[1];
}

/**
 * Set the x value
 * @param x The new value
*/
void Matrix1x2f::x(float x){
    _Mat[0] = x;
}

/**
 * Set the y value
 * @param y The new value
*/
void Matrix1x2f::y(float y){
    _Mat[1] = y;
}

/**
 * Getter for the matrices elements
 * @param index The index of the element to access
 * @return The wanted value
*/
float Matrix1x2f::operator[](int index) const{
    if(index < 0 || index >= static_cast<int>(_Mat.size())){
        fprintf(stderr, "Index %d out of range for matrix1x2!\n", index);
        beCore::ErrorHandler::handle(beCore::ErrorCode::BAD_VALUE, beCore::ErrorLevel::WARNING);
    }
    return _Mat[index];
}

/**
 * Setter for the matrices elements
 * @param index The index of the element to set
 * @return The wanted value
*/
float& Matrix1x2f::operator[](int index){
    if(index < 0 || index >= static_cast<int>(_Mat.size())){
        fprintf(stderr, "Index %d out of range for matrix1x2!\n", index);
        beCore::ErrorHandler::handle(beCore::ErrorCode::BAD_VALUE, beCore::ErrorLevel::WARNING);
    }
    return _Mat[index];
}

/**
 * Addition between two matrices
 * @param matrix The second matrix
 * @return The sum of the two matrices
*/
Matrix1x2f Matrix1x2f::operator+(const Matrix1x2f& matrix) const{

}

/**
 * Substraction between two matrices
 * @param matrix The second matrix
 * @return The substraction of the two matrices
*/
Matrix1x2f Matrix1x2f::operator-(const Matrix1x2f& matrix) const{

}

/**
 * Addition between two matrices
 * @param matrix The second matrix
*/
void Matrix1x2f::operator+=(const Matrix1x2f& matrix){
}

/**
 * Substraction between two matrices
 * @param matrix The second matrix
*/
void Matrix1x2f::operator-=(const Matrix1x2f& matrix){
}

/**
 * Multiplication with a scalar
 * @param val The scalar
*/
void Matrix1x2f::operator*=(float scalar){
}

/**
 * Division with a scalar
 * @param val The scalar
*/
void Matrix1x2f::operator/=(float scalar){
}

/**
 * Multiplication with a vector
 * @param vector The vector
* @return The resulting scalar
*/
float Matrix1x2f::operator*(const Vector2f& vector) const{
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
*/
void Matrix1x2f::operator*=(const Matrix2x2f& matrix){
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
 * @return The resulting matrix
*/
Matrix1x2f Matrix1x2f::operator*(const Matrix2x2f& matrix) const{
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
 * @return The resulting matrix
*/
Matrix1x3f Matrix1x2f::operator*(const Matrix2x3f& matrix) const{
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
 * @return The resulting matrix
*/
Matrix1x4f Matrix1x2f::operator*(const Matrix2x4f& matrix) const{
}

};