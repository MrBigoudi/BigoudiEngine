#include "matrix1x3.hpp"

#include "matrix.hpp" // IWYU pragma: keep
#include "vector.hpp" // IWYU pragma: keep

#include "errorHandler.hpp"

namespace be{

/**
 * An empty constructor
*/
Matrix1x3::Matrix1x3()
    : Matrix1x3({0.f, 0.f, 0.f}){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Matrix1x3::Matrix1x3(float v)
    : Matrix1x3({v,v,v}){}

/**
 * A basic constructor
 * @param values The array containing the values
*/
Matrix1x3::Matrix1x3(const std::array<float, 3>& values){
    _Values = values;
}

/**
 * A constructor by copy
 * @param matrix The matrix to copy
*/
Matrix1x3::Matrix1x3(const Matrix1x3& matrix){
    copy(matrix);
}

/**
 * Copy a matrix
 * @param matrix The matrix to cpoy
*/
Matrix1x3& Matrix1x3::operator=(const Matrix1x3& matrix){
    copy(matrix);
    return *this;
}

/**
 * Copy a matrix
 * @param matrix The matrix to copy
*/
void Matrix1x3::copy(const Matrix1x3& matrix){
    _Values[0] = matrix._Values[0];
    _Values[1] = matrix._Values[1];
    _Values[2] = matrix._Values[2];
}

/**
 * Create a matrix fill with ones
*/
Matrix1x3 Matrix1x3::ones(){
    return Matrix1x3(1.f);
}

/**
 * Create a matrix fill with zeros
*/
Matrix1x3 Matrix1x3::zeros(){
    return Matrix1x3();
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Matrix1x3::toString() const{
    return "{{" 
        + std::to_string(_Values[0])
        + ","
        + std::to_string(_Values[1])
        + ","
        + std::to_string(_Values[2])
        + "}}"
    ;
}

/**
 * Getter for the matrices elements
 * @param index The index of the element to access
 * @return The wanted value
*/
float Matrix1x3::operator[](int index) const{
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR, 
            "Index " + std::to_string(index) + " out of range for Matrix1x3!\n",
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
float& Matrix1x3::operator[](int index){
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR, 
            "Index " + std::to_string(index) + " out of range for Matrix1x3!\n",
            ErrorLevel::WARNING
        );
    }
    return _Values[index];
}

/**
 * Addition between two matrices
 * @param matrix The second matrix
 * @return The sum of the two matrices
*/
Matrix1x3 Matrix1x3::operator+(const Matrix1x3& matrix) const{
    return Matrix1x3(
        {
            _Values[0] + matrix._Values[0], 
            _Values[1] + matrix._Values[1],
            _Values[2] + matrix._Values[2]
        });
}

/**
 * Substraction between two matrices
 * @param matrix The second matrix
 * @return The substraction of the two matrices
*/
Matrix1x3 Matrix1x3::operator-(const Matrix1x3& matrix) const{
    return Matrix1x3(
        {
            _Values[0] - matrix._Values[0], 
            _Values[1] - matrix._Values[1],
            _Values[2] - matrix._Values[2]
        });
}

/**
 * Addition between two matrices
 * @param matrix The second matrix
*/
void Matrix1x3::operator+=(const Matrix1x3& matrix){
    for(size_t i=0; i<_Values.size(); i++){
        _Values[i] += matrix._Values[i];
    }
}

/**
 * Substraction between two matrices
 * @param matrix The second matrix
*/
void Matrix1x3::operator-=(const Matrix1x3& matrix){
    for(size_t i=0; i<_Values.size(); i++){
        _Values[i] -= matrix._Values[i];
    }
}

/**
 * Multiplication with a scalar
 * @param val The scalar
*/
void Matrix1x3::operator*=(float scalar){
    for(size_t i=0; i<_Values.size(); i++){
        _Values[i] *= scalar;
    }
}

/**
 * Division with a scalar
 * @param val The scalar
*/
void Matrix1x3::operator/=(float scalar){
    if(scalar == 0.f){
        ErrorHandler::handle(
            ErrorCode::ZERO_DIVIDE_ERROR, 
            "Cannot divide by 0!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    for(size_t i=0; i<_Values.size(); i++){
        _Values[i] /= scalar;
    }
}

/**
 * Multiplication with a vector
 * @param vector The vector
 * @return The resulting scalar
*/
float Matrix1x3::operator*(const Vector3& vector) const{
    return _Values[0] * vector.x() + _Values[1] * vector.y() + _Values[2]*vector.z();
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
*/
void Matrix1x3::operator*=(const Matrix3x3& matrix){
    float val0 = _Values[0] * matrix[0][0] + _Values[1] * matrix[1][0] + _Values[2] * matrix[2][0];
    float val1 = _Values[0] * matrix[0][1] + _Values[1] * matrix[1][1] + _Values[2] * matrix[2][1];
    float val2 = _Values[0] * matrix[0][2] + _Values[1] * matrix[1][2] + _Values[2] * matrix[2][2];
    _Values[0] = val0;
    _Values[1] = val1;
    _Values[2] = val2;
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
 * @return The resulting matrix
*/
Matrix1x2 Matrix1x3::operator*(const Matrix3x2& matrix) const{
    float val0 = _Values[0] * matrix[0][0] + _Values[1] * matrix[1][0] + _Values[2] * matrix[2][0];
    float val1 = _Values[0] * matrix[0][1] + _Values[1] * matrix[1][1] + _Values[2] * matrix[2][1];
    return Matrix1x2({val0, val1});
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
 * @return The resulting matrix
*/
Matrix1x3 Matrix1x3::operator*(const Matrix3x3& matrix) const{
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
Matrix1x4 Matrix1x3::operator*(const Matrix3x4& matrix) const{
    float val0 = _Values[0] * matrix[0][0] + _Values[1] * matrix[1][0] + _Values[2] * matrix[2][0];
    float val1 = _Values[0] * matrix[0][1] + _Values[1] * matrix[1][1] + _Values[2] * matrix[2][1];
    float val2 = _Values[0] * matrix[0][2] + _Values[1] * matrix[1][2] + _Values[2] * matrix[2][2];
    float val3 = _Values[0] * matrix[0][3] + _Values[1] * matrix[1][3] + _Values[2] * matrix[2][3];
    return Matrix1x4({val0, val1, val2, val3});
}

}