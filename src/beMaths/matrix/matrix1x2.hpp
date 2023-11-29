#pragma once

#include <array>
#include <string>

#include "mathsTypes.hpp"

namespace beMaths{

/**
 * A class representing a 1x2 floating point matrix
*/
class Matrix1x2f{
    private:
        /**
         * The array containing the values
        */
        std::array<float, 2> _Values = {0.f, 0.f};

    public:
        /**
         * An empty constructor
        */
        Matrix1x2f();

        /**
         * A basic constructor
         * @param v The value for all the elements in the matrix
        */
        Matrix1x2f(float v);

        /**
         * A basic constructor
         * @param values The array containing the values
        */
        Matrix1x2f(const std::array<float, 2>& values);

        /**
         * Create a matrix fill with ones
        */
        static Matrix1x2f ones();

        /**
         * Create a matrix fill with zeros
        */
        static Matrix1x2f zeros();

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
        float operator[](int index) const;

        /**
         * Setter for the matrices elements
         * @param index The index of the element to set
         * @return The wanted value
        */
        float& operator[](int index);

        /**
         * Addition between two matrices
         * @param matrix The second matrix
         * @return The sum of the two matrices
        */
        Matrix1x2f operator+(const Matrix1x2f& matrix) const;

        /**
         * Substraction between two matrices
         * @param matrix The second matrix
         * @return The substraction of the two matrices
        */
        Matrix1x2f operator-(const Matrix1x2f& matrix) const;

        /**
         * Addition between two matrices
         * @param matrix The second matrix
        */
        void operator+=(const Matrix1x2f& matrix);

        /**
         * Substraction between two matrices
         * @param matrix The second matrix
        */
        void operator-=(const Matrix1x2f& matrix);

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
         * Multiplication with a vector
         * @param vector The vector
         * @return The resulting scalar
        */
        float operator*(const Vector2f& vector) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
        */
        void operator*=(const Matrix2x2f& matrix);

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix1x2f operator*(const Matrix2x2f& matrix) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix1x3f operator*(const Matrix2x3f& matrix) const;

        /**
         * Multiplication between two matrices
         * @param matrix The second matrix
         * @return The resulting matrix
        */
        Matrix1x4f operator*(const Matrix2x4f& matrix) const;
};

}