#include "be_matrix2x2.hpp"

#include "be_matrix.hpp" // IWYU pragma: keep
#include "be_vector.hpp" // IWYU pragma: keep

#include "be_errorHandler.hpp"

namespace be{

/**
 * An empty constructor
*/
Matrix2x2::Matrix2x2()
    : Matrix2x2({
        std::array<float, 2>({0.f, 0.f}),
        std::array<float, 2>({0.f, 0.f})
    }){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Matrix2x2::Matrix2x2(float v)
    : Matrix2x2({
        std::array<float, 2>({v,v}),
        std::array<float, 2>({v,v})
    }){}

/**
 * A constructor by copy
 * @param matrix The matrix to copy
*/
Matrix2x2::Matrix2x2(const Matrix2x2& matrix){
    copy(matrix);
}

/**
 * Copy a matrix
 * @param matrix The matrix to cpoy
*/
Matrix2x2& Matrix2x2::operator=(const Matrix2x2& matrix){
    copy(matrix);
    return *this;
}

/**
 * Copy a matrix
 * @param matrix The matrix to copy
*/
void Matrix2x2::copy(const Matrix2x2& matrix){
    for(size_t i = 0; i<matrix._Values.size(); i++){
        for(size_t j = 0; j<matrix._Values[0].size(); j++){
            _Values[i][j] = matrix._Values[i][j];
        }
    }
}

/**
 * A basic constructor
 * @param values The array containing the values
*/
Matrix2x2::Matrix2x2(const std::array<std::array<float, 2>, 2>& values){
    _Values = values;
}

/**
 * Create a matrix fill with ones
*/
Matrix2x2 Matrix2x2::ones(){
    return Matrix2x2(1.f);
}

/**
 * Create a matrix fill with zeros
*/
Matrix2x2 Matrix2x2::zeros(){
    return Matrix2x2();
}

/**
 * Create a diagonal matrix
 * @param v The value for all the elements in the diagonal
*/
Matrix2x2 Matrix2x2::diag(float v){
    Matrix2x2 newMat{};
    for(size_t i = 0; i<newMat._Values.size(); i++){
        newMat._Values[i][i] = v;
    }
    return newMat;
} 

/**
 * Create an identity matrix
*/
Matrix2x2 Matrix2x2::identity(){
    return diag(1.f);
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Matrix2x2::toString() const{
    std::string str = "{";
    for(size_t i = 0; i<_Values.size(); i++){
        str += "\n{" + std::to_string(_Values[i][0]);
        for(size_t j = 1; j<_Values[0].size(); j++){
            str += ", " + std::to_string(_Values[i][j]);
        }
        str += "}";
    }
    str += "\n}";
    return str;
}

/**
 * Getter for the matrices elements
 * @param index The index of the element to access
 * @return The wanted value
*/
std::array<float, 2> Matrix2x2::operator[](int index) const{
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR, 
            "Index " + std::to_string(index) + " out of range for Matrix2x2!\n",
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
std::array<float, 2>& Matrix2x2::operator[](int index){
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR, 
            "Index " + std::to_string(index) + " out of range for Matrix2x2!\n",
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
Matrix2x2 Matrix2x2::operator+(const Matrix2x2& matrix) const{
    Matrix2x2 newMat{};
    for(size_t i = 0; i<_Values.size(); i++){
        for(size_t j = 0; j<_Values[0].size(); j++){
            newMat._Values[i][j] = _Values[i][j] + matrix[i][j];
        }
    }
    return newMat;
}

/**
 * Substraction between two matrices
 * @param matrix The second matrix
 * @return The substraction of the two matrices
*/
Matrix2x2 Matrix2x2::operator-(const Matrix2x2& matrix) const{
    Matrix2x2 newMat{};
    for(size_t i = 0; i<_Values.size(); i++){
        for(size_t j = 0; j<_Values[0].size(); j++){
            newMat._Values[i][j] = _Values[i][j] - matrix[i][j];
        }
    }
    return newMat;
}

/**
 * Addition between two matrices
 * @param matrix The second matrix
*/
void Matrix2x2::operator+=(const Matrix2x2& matrix){
    for(size_t i = 0; i<_Values.size(); i++){
        for(size_t j = 0; j<_Values[0].size(); j++){
            _Values[i][j] += matrix[i][j];
        }
    }
}

/**
 * Substraction between two matrices
 * @param matrix The second matrix
*/
void Matrix2x2::operator-=(const Matrix2x2& matrix){
    for(size_t i = 0; i<_Values.size(); i++){
        for(size_t j = 0; j<_Values[0].size(); j++){
            _Values[i][j] -= matrix[i][j];
        }
    }
}

/**
 * Multiplication with a scalar
 * @param val The scalar
*/
void Matrix2x2::operator*=(float scalar){
    for(size_t i = 0; i<_Values.size(); i++){
        for(size_t j = 0; j<_Values[0].size(); j++){
            _Values[i][j] *= scalar;
        }
    }
}

/**
 * Division with a scalar
 * @param val The scalar
*/
void Matrix2x2::operator/=(float scalar){
    if(scalar == 0.f){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::ZERO_DIVIDE_ERROR, 
            "Cannot divide by 0!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    for(size_t i = 0; i<_Values.size(); i++){
        for(size_t j = 0; j<_Values[0].size(); j++){
            _Values[i][j] /= scalar;
        }
    }
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
*/
void Matrix2x2::operator*=(const Matrix2x2& matrix){
    Matrix2x2 newMat{};
    for(size_t i = 0; i<_Values.size(); i++){
        for(size_t j = 0; j<_Values[0].size(); j++){
            for (size_t k = 0; k < _Values.size(); k++) {
                newMat[i][j] += _Values[i][k] * matrix[k][j];
            }
        }
    }
    copy(newMat);
}

/**
 * Multiplication with a vector
 * @param vector The vector
 * @return The resulting scalar
*/
Vector2 Matrix2x2::operator*(const Vector2& vector) const{
    Vector2 newVector{};
    for(size_t i = 0; i<_Values.size(); i++){
        for(size_t j = 0; j<_Values[0].size(); j++){
            newVector[i] += _Values[i][j]*vector[j];
        }
    }
    return newVector;
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
 * @return The resulting matrix
*/
Matrix2x2 Matrix2x2::operator*(const Matrix2x2& matrix) const{
    Matrix2x2 newMat{};
    for(size_t i = 0; i<_Values.size(); i++){
        for(size_t j = 0; j<2; j++){
            for (size_t k = 0; k < _Values[0].size(); k++) {
                newMat[i][j] += _Values[i][k] * matrix[k][j];
            }
        }
    }
    return newMat;
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
 * @return The resulting matrix
*/
Matrix2x3 Matrix2x2::operator*(const Matrix2x3& matrix) const{
    Matrix2x3 newMat{};
    for(size_t i = 0; i<_Values.size(); i++){
        for(size_t j = 0; j<3; j++){
            for (size_t k = 0; k < _Values[0].size(); k++) {
                newMat[i][j] += _Values[i][k] * matrix[k][j];
            }
        }
    }
    return newMat;
}

/**
 * Multiplication between two matrices
 * @param matrix The second matrix
 * @return The resulting matrix
*/
Matrix2x4 Matrix2x2::operator*(const Matrix2x4& matrix) const{
    Matrix2x4 newMat{};
    for(size_t i = 0; i<_Values.size(); i++){
        for(size_t j = 0; j<4; j++){
            for (size_t k = 0; k < _Values[0].size(); k++) {
                newMat[i][j] += _Values[i][k] * matrix[k][j];
            }
        }
    }
    return newMat;
}

/**
 * Get the determinant of the current matrix
 * @return The determinant as a float
*/
float Matrix2x2::det() const {
    return _Values[0][0] * _Values[1][1]
        - _Values[0][1] * _Values[1][0];
}

/**
 * Get the determinant of the given matrix
 * @param matrix The given matrix
 * @return The determinant as a float
*/
float Matrix2x2::det(const Matrix2x2& matrix){
    return matrix.det();
}

}