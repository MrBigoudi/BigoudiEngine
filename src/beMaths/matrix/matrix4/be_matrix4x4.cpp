#include "be_matrix4x4.hpp"

#include "be_matrix.hpp" // IWYU pragma: keep
#include "be_vector.hpp" // IWYU pragma: keep

#include "be_errorHandler.hpp"

namespace be{

/**
 * An empty constructor
*/
Matrix4x4::Matrix4x4()
    : Matrix4x4({
        std::array<float, 4>({0.f, 0.f, 0.f, 0.f}),
        std::array<float, 4>({0.f, 0.f, 0.f, 0.f}),
        std::array<float, 4>({0.f, 0.f, 0.f, 0.f}),
        std::array<float, 4>({0.f, 0.f, 0.f, 0.f})
    }){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Matrix4x4::Matrix4x4(float v)
    : Matrix4x4({
        std::array<float, 4>({v,v,v,v}),
        std::array<float, 4>({v,v,v,v}),
        std::array<float, 4>({v,v,v,v}),
        std::array<float, 4>({v,v,v,v})
    }){}

/**
 * Copy a matrix
 * @param matrix The matrix to cpoy
*/
Matrix4x4& Matrix4x4::operator=(const Matrix4x4& matrix){
    copy(matrix);
    return *this;
}

/**
 * A constructor by copy
 * @param matrix The matrix to copy
*/
Matrix4x4::Matrix4x4(const Matrix4x4& matrix){
    copy(matrix);
}

/**
 * Copy a matrix
 * @param matrix The matrix to copy
*/
void Matrix4x4::copy(const Matrix4x4& matrix){
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
Matrix4x4::Matrix4x4(const std::array<std::array<float, 4>, 4>& values){
    _Values = values;
}

/**
 * Create a matrix fill with ones
*/
Matrix4x4 Matrix4x4::ones(){
    return Matrix4x4(1.f);
}

/**
 * Create a matrix fill with zeros
*/
Matrix4x4 Matrix4x4::zeros(){
    return Matrix4x4();
}

/**
 * Create a diagonal matrix
 * @param v The value for all the elements in the diagonal
*/
Matrix4x4 Matrix4x4::diag(float v){
    Matrix4x4 newMat{};
    for(size_t i = 0; i<newMat._Values.size(); i++){
        newMat._Values[i][i] = v;
    }
    return newMat;
} 

/**
 * Create an identity matrix
*/
Matrix4x4 Matrix4x4::identity(){
    return diag(1.f);
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Matrix4x4::toString() const{
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
std::array<float, 4> Matrix4x4::operator[](int index) const{
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR, 
            "Index " + std::to_string(index) + " out of range for Matrix4x4!\n",
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
std::array<float, 4>& Matrix4x4::operator[](int index){
    if(index < 0 || index >= static_cast<int>(_Values.size())){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR, 
            "Index " + std::to_string(index) + " out of range for Matrix4x4!\n",
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
Matrix4x4 Matrix4x4::operator+(const Matrix4x4& matrix) const{
    Matrix4x4 newMat{};
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
Matrix4x4 Matrix4x4::operator-(const Matrix4x4& matrix) const{
    Matrix4x4 newMat{};
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
void Matrix4x4::operator+=(const Matrix4x4& matrix){
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
void Matrix4x4::operator-=(const Matrix4x4& matrix){
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
void Matrix4x4::operator*=(float scalar){
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
void Matrix4x4::operator/=(float scalar){
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
void Matrix4x4::operator*=(const Matrix4x4& matrix){
    Matrix4x4 newMat{};
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
Vector4 Matrix4x4::operator*(const Vector4& vector) const{
    Vector4 newVector{};
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
Matrix4x2 Matrix4x4::operator*(const Matrix4x2& matrix) const{
    Matrix4x2 newMat{};
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
Matrix4x3 Matrix4x4::operator*(const Matrix4x3& matrix) const{
    Matrix4x3 newMat{};
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
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& matrix) const{
    Matrix4x4 newMat{};
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
 * Transpose the current matrix
*/
void Matrix4x4::transpose(void){
    for(int i=0; i<4; i++){
        for(int j=0; j<i; j++){
            float oldValue = _Values[i][j];
            _Values[i][j] = _Values[j][i];
            _Values[j][i] = oldValue; 
        }
    }
}

/**
 * Transpose the current matrix
 * @param matrix The matrix to transpose
 * @return The transposed matrix
*/
Matrix4x4 Matrix4x4::transpose(const Matrix4x4& matrix){
    Matrix4x4 newMat(matrix);
    newMat.transpose();
    return newMat;
}

/**
 * Get the determinant of the current matrix
 * @return The determinant as a float
*/
float Matrix4x4::det() const {
    Matrix3x3 matA = Matrix3x3({
        std::array<float, 3>({_Values[1][1], _Values[1][2], _Values[1][3]}), 
        std::array<float, 3>({_Values[2][1], _Values[2][2], _Values[2][3]}),
        std::array<float, 3>({_Values[3][1], _Values[3][2], _Values[3][3]})
    });

    Matrix3x3 matB = Matrix3x3({
        std::array<float, 3>({_Values[1][0], _Values[1][2], _Values[1][3]}), 
        std::array<float, 3>({_Values[2][0], _Values[2][2], _Values[2][3]}),
        std::array<float, 3>({_Values[3][0], _Values[3][2], _Values[3][3]})
    });

    Matrix3x3 matC = Matrix3x3({
        std::array<float, 3>({_Values[1][0], _Values[1][1], _Values[1][3]}), 
        std::array<float, 3>({_Values[2][0], _Values[2][1], _Values[2][3]}),
        std::array<float, 3>({_Values[3][0], _Values[3][1], _Values[3][3]})
    });

    Matrix3x3 matD = Matrix3x3({
        std::array<float, 3>({_Values[1][0], _Values[1][1], _Values[1][2]}), 
        std::array<float, 3>({_Values[2][0], _Values[2][1], _Values[2][2]}),
        std::array<float, 3>({_Values[3][0], _Values[3][1], _Values[3][2]})
    });

    return _Values[0][0] * matA.det()
        - _Values[0][1] * matB.det()
        + _Values[0][2] * matC.det()
        - _Values[0][3] * matD.det();
}

/**
 * Get the determinant of the given matrix
 * @param matrix The given matrix
 * @return The determinant as a float
*/
float Matrix4x4::det(const Matrix4x4& matrix){
    return matrix.det();
}

/**
 * Inverse a the current matrix
*/
void Matrix4x4::inverse(){
    float A2323 = _Values[2][2] * _Values[3][3] - _Values[2][3] * _Values[3][2];
    float A1323 = _Values[2][1] * _Values[3][3] - _Values[2][3] * _Values[3][1];
    float A1223 = _Values[2][1] * _Values[3][2] - _Values[2][2] * _Values[3][1];
    float A0323 = _Values[2][0] * _Values[3][3] - _Values[2][3] * _Values[3][0];
    float A0223 = _Values[2][0] * _Values[3][2] - _Values[2][2] * _Values[3][0];
    float A0123 = _Values[2][0] * _Values[3][1] - _Values[2][1] * _Values[3][0];
    float A2313 = _Values[1][2] * _Values[3][3] - _Values[1][3] * _Values[3][2];
    float A1313 = _Values[1][1] * _Values[3][3] - _Values[1][3] * _Values[3][1];
    float A1213 = _Values[1][1] * _Values[3][2] - _Values[1][2] * _Values[3][1];
    float A2312 = _Values[1][2] * _Values[2][3] - _Values[1][3] * _Values[2][2];
    float A1312 = _Values[1][1] * _Values[2][3] - _Values[1][3] * _Values[2][1];
    float A1212 = _Values[1][1] * _Values[2][2] - _Values[1][2] * _Values[2][1];
    float A0313 = _Values[1][0] * _Values[3][3] - _Values[1][3] * _Values[3][0];
    float A0213 = _Values[1][0] * _Values[3][2] - _Values[1][2] * _Values[3][0];
    float A0312 = _Values[1][0] * _Values[2][3] - _Values[1][3] * _Values[2][0];
    float A0212 = _Values[1][0] * _Values[2][2] - _Values[1][2] * _Values[2][0];
    float A0113 = _Values[1][0] * _Values[3][1] - _Values[1][1] * _Values[3][0];
    float A0112 = _Values[1][0] * _Values[2][1] - _Values[1][1] * _Values[2][0];

    float det = this->det();
    det = 1.f / det;

    Matrix4x4 tmp{};
    tmp[0][0] = det *   ( _Values[1][1] * A2323 - _Values[1][2] * A1323 + _Values[1][3] * A1223 );
    tmp[0][1] = det * - ( _Values[0][1] * A2323 - _Values[0][2] * A1323 + _Values[0][3] * A1223 );
    tmp[0][2] = det *   ( _Values[0][1] * A2313 - _Values[0][2] * A1313 + _Values[0][3] * A1213 );
    tmp[0][3] = det * - ( _Values[0][1] * A2312 - _Values[0][2] * A1312 + _Values[0][3] * A1212 );
    tmp[1][0] = det * - ( _Values[1][0] * A2323 - _Values[1][2] * A0323 + _Values[1][3] * A0223 );
    tmp[1][1] = det *   ( _Values[0][0] * A2323 - _Values[0][2] * A0323 + _Values[0][3] * A0223 );
    tmp[1][2] = det * - ( _Values[0][0] * A2313 - _Values[0][2] * A0313 + _Values[0][3] * A0213 );
    tmp[1][3] = det *   ( _Values[0][0] * A2312 - _Values[0][2] * A0312 + _Values[0][3] * A0212 );
    tmp[2][0] = det *   ( _Values[1][0] * A1323 - _Values[1][1] * A0323 + _Values[1][3] * A0123 );
    tmp[2][1] = det * - ( _Values[0][0] * A1323 - _Values[0][1] * A0323 + _Values[0][3] * A0123 );
    tmp[2][2] = det *   ( _Values[0][0] * A1313 - _Values[0][1] * A0313 + _Values[0][3] * A0113 );
    tmp[2][3] = det * - ( _Values[0][0] * A1312 - _Values[0][1] * A0312 + _Values[0][3] * A0112 );
    tmp[3][0] = det * - ( _Values[1][0] * A1223 - _Values[1][1] * A0223 + _Values[1][2] * A0123 );
    tmp[3][1] = det *   ( _Values[0][0] * A1223 - _Values[0][1] * A0223 + _Values[0][2] * A0123 );
    tmp[3][2] = det * - ( _Values[0][0] * A1213 - _Values[0][1] * A0213 + _Values[0][2] * A0113 );
    tmp[3][3] = det *   ( _Values[0][0] * A1212 - _Values[0][1] * A0212 + _Values[0][2] * A0112 );

    copy(tmp);
}

/**
 * Inverse the given matrix
 * @param matrix The matrix to vinert
 * @return The inverted matrix
*/
Matrix4x4 Matrix4x4::inverse(const Matrix4x4& matrix){
    Matrix4x4 newMat(matrix);
    newMat.inverse();
    return newMat; 
}

}