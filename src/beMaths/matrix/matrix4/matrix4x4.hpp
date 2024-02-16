#pragma once

#include <array>
#include <string>

#include "mathsTypes.hpp"

namespace be{

/**
 * A class representing a 4x4 floating point matrix
*/
class Matrix4x4{
    private:
        /**
         * The array containing the values
        */
        std::array<std::array<float, 4>, 4> _Values = {
            std::array<float, 4>({0.f, 0.f, 0.f, 0.f}),
            std::array<float, 4>({0.f, 0.f, 0.f, 0.f}),
            std::array<float, 4>({0.f, 0.f, 0.f, 0.f}),
            std::array<float, 4>({0.f, 0.f, 0.f, 0.f})
        };

    public:
        /**
         * An empty constructor
        */
        Matrix4x4();

        /**
         * A basic constructor
         * @param v The value for all the elements in the matrix
        */
        Matrix4x4(float v);

        /**
         * A constructor by copy
         * @param matrix The matrix to copy
        */
        Matrix4x4(const Matrix4x4& matrix);

        /**
         * Copy a matrix
         * @param matrix The matrix to cpoy
        */
        Matrix4x4& operator=(const Matrix4x4& matrix);

        /**
         * Copy a matrix
         * @param matrix The matrix to copy
        */
        void copy(const Matrix4x4& matrix);

        /**
         * A basic constructor
         * @param values The array containing the values
        */
        Matrix4x4(const std::array<std::array<float, 4>, 4>& values);

        /**
         * Create a matrix fill with ones
        */
        static Matrix4x4 ones();

        /**
         * Create a matrix fill with zeros
        */
        static Matrix4x4 zeros();

        /**
         * Create a diagonal matrix
         * @param v The value for all the elements in the diagonal
        */
        static Matrix4x4 diag(float v);

        /**
         * Create an identity matrix
        */
        static Matrix4x4 identity();

        /**
         * Cast the matrix into a string
         * @return The string
        */
        const std::string toString() const;

        /**
         * Getter for the matrices elements
         * @param index The index of the element to access
         * @return The wanted value
        */
        std::array<float, 4> operator[](int index) const;

        /**
         * Setter for the matrices elements
         * @param index The index of the element to set
         * @return The wanted value
        */
        std::array<float, 4>& operator[](int index);
        
        /**
         * Addition between two matrices
         * @param matrix The second matrix
         * @return The sum of the two matrices
        */
        Matrix4x4 operator+(const Matrix4x4& matrix) const;

        /**
         * Substraction between two matrices
         * @param matrix The second matrix
         * @return The substraction of the two matrices
        */
        Matrix4x4 operator-(const Matrix4x4& matrix) const;

        /**
         * Addition between two matrices
         * @param matrix The second matrix
        */
        void operator+=(const Matrix4x4& matrix);

        /**
         * Substraction between two matrices
         * @param matrix The second matrix
        */
        void operator-=(const Matrix4x4& matrix);

        /**
         * Multiplication with a scalar
         * @param val The scalar
        */
        void operator*=(float scalar);

        /**
         * Division with a scalar
         * @param val The scalar
        */
        void operator/=(float scalar);

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
        */
        void operator*=(const Matrix4x4& matrix);

        /**
         * Multiplication with a vector
         * @param vector The vector
         * @return The resulting scalar
        */
        Vector4 operator*(const Vector4& vector) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix4x2 operator*(const Matrix4x2& matrix) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix4x3 operator*(const Matrix4x3& matrix) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix4x4 operator*(const Matrix4x4& matrix) const;

        /**
         * Transpose the current matrix
        */
        void transpose(void);

        /**
         * Transpose the current matrix
         * @return The transposed matrix
        */
        Matrix4x4 transpose(void) const;
};

}