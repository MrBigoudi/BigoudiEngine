#pragma once

#include "mathsTypes.hpp"

#include <array>
#include <string>

namespace beMaths{

/**
 * A class representing a 3d vector of floating points 
*/
class Vector3{
    private:
        /**
         * The array containing the values
        */
        std::array<float, 3> _Values = {0.f, 0.f, 0.f};

    public:
        /**
         * An empty constructor
        */
        Vector3();

        /**
         * A basic constructor
         * @param v The value for all the elements in the vector
        */
        Vector3(float v);

        /**
         * A basic constructor
         * @param x The first value
         * @param y The second value
         * @param z The third value
        */
        Vector3(float x, float y, float z);

        /**
         * Create a vector fill with ones
        */
        static Vector3 ones();

        /**
         * Create a vector fill with zeros
        */
        static Vector3 zeros();

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
         * Get the z value
         * @return z
        */
        float z() const;

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
         * Set the z value
         * @param z The new value
        */
        void z(float z);

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
         * Get the b value
         * @return b
        */
        float b() const;

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
         * Set the b value
         * @param b The new value
        */
        void b(float b);        

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
        Vector3 operator+(const Vector3& vector) const;

        /**
         * Substraction between two vectors
         * @param vector The second vertex
         * @return The substraction of the two vectors
        */
        Vector3 operator-(const Vector3& vector) const;

        /**
         * Addition between two vectors
         * @param vector The second vector
        */
        void operator+=(const Vector3& vector);

        /**
         * Substraction between two vectors
         * @param vector The second vector
        */
        void operator-=(const Vector3& vector);

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
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix3x2 operator*(const Matrix1x2& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix3x3 operator*(const Matrix1x3& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix3x4 operator*(const Matrix1x4& matrix) const;

        /**
         * Dot product
         * @param v1 The first vector
         * @param v2 The second vector
         * @return The dot product
        */
        static float dot(const Vector3& v1, const Vector3& v2);

        /**
         * Dot product
         * @param vector
         * @return The dot product
        */
        float dot(const Vector3& vector) const;

        /**
         * Cross product
         * @param v1 The first vector
         * @param v2 The second vector
         * @return The new vector product
        */
        static Vector3 cross(const Vector3& v1, const Vector3& v2);

        /**
         * Cross product
         * @param vector
        */
        void cross(const Vector3& vector) const;

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
        static Vector3 normalize(const Vector3& vector);

        /**
         * Normalize the vector
        */
        void normalize();
};

}