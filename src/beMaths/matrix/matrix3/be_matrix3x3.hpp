#pragma once

#include <array>
#include <string>

#include "be_mathsTypes.hpp"

namespace be{

/**
 * A class representing a 3x3 floating point matrix
*/
class Matrix3x3{
    private:
        /**
         * The array containing the values
        */
        std::array<std::array<float, 3>, 3> _Values = {
            std::array<float, 3>({0.f, 0.f, 0.f}),
            std::array<float, 3>({0.f, 0.f, 0.f}),
            std::array<float, 3>({0.f, 0.f, 0.f})
        };

    public:
        /**
         * An empty constructor
        */
        Matrix3x3();

        /**
         * A basic constructor
         * @param v The value for all the elements in the matrix
        */
        Matrix3x3(float v);

        /**
         * A constructor by copy
         * @param matrix The matrix to copy
        */
        Matrix3x3(const Matrix3x3& matrix);

        /**
         * Copy a matrix
         * @param matrix The matrix to cpoy
        */
        Matrix3x3& operator=(const Matrix3x3& matrix);

        /**
         * Copy a matrix
         * @param matrix The matrix to copy
        */
        void copy(const Matrix3x3& matrix);

        /**
         * A basic constructor
         * @param values The array containing the values
        */
        Matrix3x3(const std::array<std::array<float, 3>, 3>& values);

        /**
         * Create a matrix fill with ones
        */
        static Matrix3x3 ones();

        /**
         * Create a matrix fill with zeros
        */
        static Matrix3x3 zeros();

        /**
         * Create a diagonal matrix
         * @param v The value for all the elements in the diagonal
        */
        static Matrix3x3 diag(float v);

        /**
         * Create an identity matrix
        */
        static Matrix3x3 identity();

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
        std::array<float, 3> operator[](int index) const;

        /**
         * Setter for the matrices elements
         * @param index The index of the element to set
         * @return The wanted value
        */
        std::array<float, 3>& operator[](int index);
        
        /**
         * Addition between two matrices
         * @param matrix The second matrix
         * @return The sum of the two matrices
        */
        Matrix3x3 operator+(const Matrix3x3& matrix) const;

        /**
         * Substraction between two matrices
         * @param matrix The second matrix
         * @return The substraction of the two matrices
        */
        Matrix3x3 operator-(const Matrix3x3& matrix) const;

        /**
         * Addition between two matrices
         * @param matrix The second matrix
        */
        void operator+=(const Matrix3x3& matrix);

        /**
         * Substraction between two matrices
         * @param matrix The second matrix
        */
        void operator-=(const Matrix3x3& matrix);

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
        void operator*=(const Matrix3x3& matrix);

        /**
         * Multiplication with a vector
         * @param vector The vector
         * @return The resulting scalar
        */
        Vector3 operator*(const Vector3& vector) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix3x2 operator*(const Matrix3x2& matrix) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix3x3 operator*(const Matrix3x3& matrix) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix3x4 operator*(const Matrix3x4& matrix) const;

        /**
         * Get the determinant of the current matrix
         * @return The determinant as a float
        */
        float det() const;

        /**
         * Get the determinant of the given matrix
         * @param matrix The given matrix
         * @return The determinant as a float
        */
        static float det(const Matrix3x3& matrix);
};

}