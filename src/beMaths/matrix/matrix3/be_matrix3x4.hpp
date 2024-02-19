#pragma once

#include <array>
#include <string>

#include "be_mathsTypes.hpp"

namespace be{

/**
 * A class representing a 3x4 floating point matrix
*/
class Matrix3x4{
    private:
        /**
         * The array containing the values
        */
        std::array<std::array<float, 4>, 3> _Values = {
            std::array<float, 4>({0.f, 0.f, 0.f, 0.f}),
            std::array<float, 4>({0.f, 0.f, 0.f, 0.f}),
            std::array<float, 4>({0.f, 0.f, 0.f, 0.f})
        };

    public:
        /**
         * An empty constructor
        */
        Matrix3x4();

        /**
         * A basic constructor
         * @param v The value for all the elements in the matrix
        */
        Matrix3x4(float v);

        /**
         * A constructor by copy
         * @param matrix The matrix to copy
        */
        Matrix3x4(const Matrix3x4& matrix);

        /**
         * Copy a matrix
         * @param matrix The matrix to cpoy
        */
        Matrix3x4& operator=(const Matrix3x4& matrix);

        /**
         * Copy a matrix
         * @param matrix The matrix to copy
        */
        void copy(const Matrix3x4& matrix);

        /**
         * A basic constructor
         * @param values The array containing the values
        */
        Matrix3x4(const std::array<std::array<float, 4>, 3>& values);

        /**
         * Create a matrix fill with ones
        */
        static Matrix3x4 ones();

        /**
         * Create a matrix fill with zeros
        */
        static Matrix3x4 zeros();

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
        Matrix3x4 operator+(const Matrix3x4& matrix) const;

        /**
         * Substraction between two matrices
         * @param matrix The second matrix
         * @return The substraction of the two matrices
        */
        Matrix3x4 operator-(const Matrix3x4& matrix) const;

        /**
         * Addition between two matrices
         * @param matrix The second matrix
        */
        void operator+=(const Matrix3x4& matrix);

        /**
         * Substraction between two matrices
         * @param matrix The second matrix
        */
        void operator-=(const Matrix3x4& matrix);

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
        Vector3 operator*(const Vector4& vector) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix3x2 operator*(const Matrix4x2& matrix) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix3x3 operator*(const Matrix4x3& matrix) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix3x4 operator*(const Matrix4x4& matrix) const;
};

}