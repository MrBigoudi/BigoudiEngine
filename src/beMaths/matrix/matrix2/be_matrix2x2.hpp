#pragma once

#include <array>
#include <string>

#include "be_mathsTypes.hpp"

namespace be{

/**
 * A class representing a 2x2 floating point matrix
*/
class Matrix2x2{
    private:
        /**
         * The array containing the values
        */
        std::array<std::array<float, 2>, 2> _Values = {
            std::array<float, 2>({0.f, 0.f}),
            std::array<float, 2>({0.f, 0.f})
        };

    public:
        /**
         * An empty constructor
        */
        Matrix2x2();

        /**
         * A basic constructor
         * @param v The value for all the elements in the matrix
        */
        Matrix2x2(float v);

        /**
         * A constructor by copy
         * @param matrix The matrix to copy
        */
        Matrix2x2(const Matrix2x2& matrix);

        /**
         * Copy a matrix
         * @param matrix The matrix to cpoy
        */
        Matrix2x2& operator=(const Matrix2x2& matrix);

        /**
         * Copy a matrix
         * @param matrix The matrix to copy
        */
        void copy(const Matrix2x2& matrix);

        /**
         * A basic constructor
         * @param values The array containing the values
        */
        Matrix2x2(const std::array<std::array<float, 2>, 2>& values);

        /**
         * Create a matrix fill with ones
        */
        static Matrix2x2 ones();

        /**
         * Create a matrix fill with zeros
        */
        static Matrix2x2 zeros();

        /**
         * Create a diagonal matrix
         * @param v The value for all the elements in the diagonal
        */
        static Matrix2x2 diag(float v);

        /**
         * Create an identity matrix
        */
        static Matrix2x2 identity();

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
        std::array<float, 2> operator[](int index) const;

        /**
         * Setter for the matrices elements
         * @param index The index of the element to set
         * @return The wanted value
        */
        std::array<float, 2>& operator[](int index);
        
        /**
         * Addition between two matrices
         * @param matrix The second matrix
         * @return The sum of the two matrices
        */
        Matrix2x2 operator+(const Matrix2x2& matrix) const;

        /**
         * Substraction between two matrices
         * @param matrix The second matrix
         * @return The substraction of the two matrices
        */
        Matrix2x2 operator-(const Matrix2x2& matrix) const;

        /**
         * Addition between two matrices
         * @param matrix The second matrix
        */
        void operator+=(const Matrix2x2& matrix);

        /**
         * Substraction between two matrices
         * @param matrix The second matrix
        */
        void operator-=(const Matrix2x2& matrix);

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
        void operator*=(const Matrix2x2& matrix);

        /**
         * Multiplication with a vector
         * @param vector The vector
         * @return The resulting scalar
        */
        Vector2 operator*(const Vector2& vector) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix2x2 operator*(const Matrix2x2& matrix) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix2x3 operator*(const Matrix2x3& matrix) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix2x4 operator*(const Matrix2x4& matrix) const;
};

}