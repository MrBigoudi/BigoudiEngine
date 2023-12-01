#include "matrix1x2.hpp"
#include "errorHandler.hpp"

#include "vector2.hpp"
#include "matrix2x2.hpp"
#include "matrix2x3.hpp"
#include "matrix2x4.hpp"
#include "matrix1x3.hpp"
#include "matrix1x4.hpp"

#include <beCore.hpp>

namespace beMaths{

/**
 * An empty constructor
*/
Matrix1x2::Matrix1x2()
    : Matrix1x2({0.f,0.f}){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Matrix1x2::Matrix1x2(float v)
    : Matrix1x2({v,v}){}

/**
 * A basic constructor
 * @param values The array containing the values
*/
Matrix1x2::Matrix1x2(const std::array<float, 2>& values){
    _Values = values;
}

/**
 * Create a matrix fill with ones
*/
Matrix1x2 Matrix1x2::ones(){
    return Matrix1x2(1.f);
}

/**
 * Create a matrix fill with zeros
*/
Matrix1x2 Matrix1x2::zeros(){
    return Matrix1x2();
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Matrix1x2::toString() const {
    return "{{" 
        + std::to_string(_Values[0])
        + ","
        + std::to_string(_Values[1])
        + "}}"
    ; 
}

/**
 * Getter for the matrices elements
 * @param index The index of the element to access
 * @return The wanted value
*/
float Matrix1x2::operator[](int index) const{
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
float& Matrix1x2::operator[](int index){
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
Matrix1x2 Matrix1x2::operator+(const Matrix1x2& matrix) const{
    return Matrix1x2(
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
Matrix1x2 Matrix1x2::operator-(const Matrix1x2& matrix) const{
    return Matrix1x2(
        {
            _Values[0] - matrix._Values[0], 
            _Values[1] - matrix._Values[1]
        });
}

/**
 * Addition between two matrices
 * @param matrix The second matrix
*/
void Matrix1x2::operator+=(const Matrix1x2& matrix){
    _Values[0] += matrix._Values[0];
    _Values[1] += matrix._Values[1];
}

/**
 * Substraction between two matrices
 * @param matrix The second matrix
*/
void Matrix1x2::operator-=(const Matrix1x2& matrix){
    _Values[0] -= matrix._Values[0];
    _Values[1] -= matrix._Values[1];
}

/**
 * Multiplication with a scalar
 * @param val The scalar
*/
void Matrix1x2::operator*=(float scalar){
    _Values[0] *= scalar;
    _Values[1] *= scalar;
}

/**
 * Division with a scalar
 * @param val The scalar
*/
void Matrix1x2::operator/=(float scalar){
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
float Matrix1x2::operator*(const Vector2& vector) const{
    return _Values[0] * vector.x() + _Values[1] * vector.y();
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
*/
void Matrix1x2::operator*=(const Matrix2x2& matrix){
    float val0 = _Values[0] * matrix[0][0] + _Values[1] * matrix[1][0];
    float val1 = _Values[0] * matrix[0][1] + _Values[1] * matrix[1][1];
    _Values[0] = val0;
    _Values[1] = val1;
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
 * @return The resulting matrix
*/
Matrix1x2 Matrix1x2::operator*(const Matrix2x2& matrix) const{
    float val0 = _Values[0] * matrix[0][0] + _Values[1] * matrix[1][0];
    float val1 = _Values[0] * matrix[0][1] + _Values[1] * matrix[1][1];
    return Matrix1x2({val0, val1});
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
 * @return The resulting matrix
*/
Matrix1x3 Matrix1x2::operator*(const Matrix2x3& matrix) const{
    float val0 = _Values[0] * matrix[0][0] + _Values[1] * matrix[1][0] + _Values[2] * matrix[2][0];
    float val1 = _Values[0] * matrix[0][1] + _Values[1] * matrix[1][1] + _Values[2] * matrix[2][1];
    float val2 = _Values[0] * matrix[0][2] + _Values[1] * matrix[1][2] + _Values[2] * matrix[2][2];
    return Matrix1x3({val0, val1, val2});
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
 * @return The resulting matrix
*/
Matrix1x4 Matrix1x2::operator*(const Matrix2x4& matrix) const{
    float val0 = _Values[0] * matrix[0][0] + _Values[1] * matrix[1][0] + _Values[2] * matrix[2][0];
    float val1 = _Values[0] * matrix[0][1] + _Values[1] * matrix[1][1] + _Values[2] * matrix[2][1];
    float val2 = _Values[0] * matrix[0][2] + _Values[1] * matrix[1][2] + _Values[2] * matrix[2][2];
    return Matrix1x3({val0, val1, val2});
}

};