#pragma once

#include "be_mathsTypes.hpp"

#include <array>
#include <string>

namespace be{

/**
 * A class representing a 2d vector of floating points 
*/
class Vector2{
    private:
        /**
         * The array containing the values
        */
        std::array<float, 2> _Values = {0.f, 0.f};

    public:
        /**
         * An empty constructor
        */
        Vector2();

        /**
         * A basic constructor
         * @param v The value for all the elements in the vector
        */
        Vector2(float v);

        /**
         * A basic constructor
         * @param x The first value
         * @param y The second value
        */
        Vector2(float x, float y);

        /**
         * Create a vector fill with ones
        */
        static Vector2 ones();

        /**
         * Create a vector fill with zeros
        */
        static Vector2 zeros();

        /**
         * Cast the vector into a string
         * @return The string
        */
        const std::string toString() const;

        /**
         * Get the x value
         * @return x
        */
        float x() const;

        /**
         * Get the y value
         * @return y
        */
        float y() const;

        /**
         * Set the x value
         * @param x The new value
        */
        void x(float x);

        /**
         * Set the y value
         * @param y The new value
        */
        void y(float y);

        /**
         * Get the r value
         * @return r
        */
        float r() const;

        /**
         * Get the g value
         * @return g
        */
        float g() const;

        /**
         * Set the r value
         * @param r The new value
        */
        void r(float r);

        /**
         * Set the g value
         * @param g The new value
        */
        void g(float g);

        /**
         * Get the u value
         * @return u
        */
        float u() const;

        /**
         * Get the v value
         * @return v
        */
        float v() const;

        /**
         * Set the u value
         * @param u The new value
        */
        void u(float u);

        /**
         * Set the v value
         * @param v The new value
        */
        void v(float v);
        

        /**
         * Getter for the vectors elements
         * @param index The index of the element to access
         * @return The wanted value
        */
        float operator[](int index) const;

        /**
         * Setter for the vectors elements
         * @param index The index of the element to set
         * @return The wanted value
        */
        float& operator[](int index);

        /**
         * Addition between two vectors
         * @param vector The second vertex
         * @return The sum of the two vectors
        */
        Vector2 operator+(const Vector2& vector) const;

        /**
         * Substraction between two vectors
         * @param vector The second vertex
         * @return The substraction of the two vectors
        */
        Vector2 operator-(const Vector2& vector) const;

        /**
         * Multiplication with a scalar
         * @param val The scalar
         * @return A new vector which is the results of the multiplication
        */
        Vector2 operator*(float scalar);

        /**
         * Division with a scalar
         * @param val The scalar
         * @return A new vector which is the results of the division
        */
        Vector2 operator/(float scalar);

        /**
         * Addition between two vectors
         * @param vector The second vector
        */
        void operator+=(const Vector2& vector);

        /**
         * Substraction between two vectors
         * @param vector The second vector
        */
        void operator-=(const Vector2& vector);

        /**
         * Negate a vector
         * @return The negated vector
        */
        Vector2 operator-(void);

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
         * Redefine the equality operator
         * @param vector The second vector
         * @return True if vectors are equal
        */
        bool operator==(const Vector2& vector) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix2x2 operator*(const Matrix1x2& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix2x3 operator*(const Matrix1x3& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix2x4 operator*(const Matrix1x4& matrix) const;

        /**
         * Dot product
         * @param v1 The first vector
         * @param v2 The second vector
         * @return The dot product
        */
        static float dot(const Vector2& v1, const Vector2& v2);

        /**
         * Dot product
         * @param vector
         * @return The dot product
        */
        float dot(const Vector2& vector) const;

        /**
         * Get the vector's norm
         * @return The norm
        */
        float getNorm() const;

        /**
         * Normalize the vector
         * @param vector The vector to normalize
         * @return The normalized vector
        */
        static Vector2 normalize(const Vector2& vector);

        /**
         * Normalize the vector
        */
        void normalize();
};

}