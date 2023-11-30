#pragma once

#include "mathsTypes.hpp"

#include <array>
#include <string>

namespace beMaths{

/**
 * A class representing a 3d vector of floating points 
*/
class Vector4{
    private:
        /**
         * The array containing the values
        */
        std::array<float, 4> _Values = {0.f, 0.f, 0.f, 0.f};

    public:
        /**
         * An empty constructor
        */
        Vector4();

        /**
         * A basic constructor
         * @param v The value for all the elements in the vector
        */
        Vector4(float v);

        /**
         * A basic constructor
         * @param x The first value
         * @param y The second value
         * @param z The third value
         * @param w The fourth value
        */
        Vector4(float x, float y, float z, float w);

        /**
         * Create a vector fill with ones
        */
        static Vector4 ones();

        /**
         * Create a vector fill with zeros
        */
        static Vector4 zeros();

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
         * Get the w value
         * @return w
        */
        float w() const;

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
         * Set the w value
         * @param w The new value
        */
        void w(float w);

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
         * Get the a value
         * @return a
        */
        float a() const;

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
         * Set the a value
         * @param a The new value
        */
        void a(float a);        

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
        Vector4 operator+(const Vector4& vector) const;

        /**
         * Substraction between two vectors
         * @param vector The second vertex
         * @return The substraction of the two vectors
        */
        Vector4 operator-(const Vector4& vector) const;

        /**
         * Addition between two vectors
         * @param vector The second vector
        */
        void operator+=(const Vector4& vector);

        /**
         * Substraction between two vectors
         * @param vector The second vector
        */
        void operator-=(const Vector4& vector);

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
        Matrix4x2 operator*(const Matrix1x2& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix4x3 operator*(const Matrix1x3& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix4x4 operator*(const Matrix1x4& matrix) const;

        /**
         * Dot product
         * @param v1 The first vector
         * @param v2 The second vector
         * @return The dot product
        */
        static float dot(const Vector4& v1, const Vector4& v2);

        /**
         * Dot product
         * @param vector
         * @return The dot product
        */
        float dot(const Vector4& vector) const;

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
        static Vector4 normalize(const Vector4& vector);

        /**
         * Normalize the vector
        */
        void normalize();
};

}