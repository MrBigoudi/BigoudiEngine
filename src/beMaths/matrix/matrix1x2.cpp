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
    : Matrix1x2f({0.f,0.f}){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Matrix1x2f::Matrix1x2f(float v)
    : Matrix1x2f({v,v}){}

/**
 * A basic constructor
 * @param values The array containing the values
*/
Matrix1x2f::Matrix1x2f(const std::array<float, 2>& values){
    _Values = values;
}

/**
 * Create a matrix fill with ones
*/
Matrix1x2f Matrix1x2f::ones(){
    return Matrix1x2f({1.f,1.f});
}

/**
 * Create a matrix fill with zeros
*/
Matrix1x2f Matrix1x2f::zeros(){
    return Matrix1x2f({0.f,0.f});
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Matrix1x2f::toString() const {
    return "{" 
        + std::to_string(_Values[0])
        + ","
        + std::to_string(_Values[1])
        + "}"
    ; 
}

/**
 * Getter for the matrices elements
 * @param index The index of the element to access
 * @return The wanted value
*/
float Matrix1x2f::operator[](int index) const{
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
float& Matrix1x2f::operator[](int index){
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        fprintf(stderr, "Index %d out of range for matrix1x2!\n", index);
        beCore::ErrorHandler::handle(beCore::ErrorCode::BAD_VALUE, beCore::ErrorLevel::WARNING);
    }
    return _Values[index];
}

/**
 * Addition between two matrices
 * @param matrix The second matrix
 * @return The sum of the two matrices
*/
Matrix1x2f Matrix1x2f::operator+(const Matrix1x2f& matrix) const{
    return Matrix1x2f(
        {
            _Values[0] + matrix._Values[0], 
            _Values[1] + matrix._Values[1]
        });
}

/**
 * Substraction between two matrices
 * @param matrix The second matrix
 * @return The substraction of the two matrices
*/
Matrix1x2f Matrix1x2f::operator-(const Matrix1x2f& matrix) const{
    return Matrix1x2f(
        {
            _Values[0] - matrix._Values[0], 
            _Values[1] - matrix._Values[1]
        });
}

/**
 * Addition between two matrices
 * @param matrix The second matrix
*/
void Matrix1x2f::operator+=(const Matrix1x2f& matrix){
    _Values[0] += matrix._Values[0];
    _Values[1] += matrix._Values[1];
}

/**
 * Substraction between two matrices
 * @param matrix The second matrix
*/
void Matrix1x2f::operator-=(const Matrix1x2f& matrix){
    _Values[0] -= matrix._Values[0];
    _Values[1] -= matrix._Values[1];
}

/**
 * Multiplication with a scalar
 * @param val The scalar
*/
void Matrix1x2f::operator*=(float scalar){
    _Values[0] *= scalar;
    _Values[1] *= scalar;
}

/**
 * Division with a scalar
 * @param val The scalar
*/
void Matrix1x2f::operator/=(float scalar){
    if(scalar == 0.f){
        beCore::ErrorHandler::handle(beCore::ErrorCode::ZERO_DIVIDE, beCore::ErrorLevel::WARNING);
        return;
    }
    _Values[0] /= scalar;
    _Values[1] /= scalar;
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